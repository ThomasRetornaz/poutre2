
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

#if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

/**
 * @file   threading_helpers.hpp
 * @author Thomas Retornaz
 * @brief  Add threading capabilities
 *
 *
 */

#include <poutre/base/base.hpp>

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <omp.h>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

/**
 * @addtogroup threading_group THREAD facilities
 * @ingroup poutre_base_group
 *@{
 */
namespace poutre::thread {
// Use max threads-1 as a reasonable default
static int POUTRE_NUM_THREADS = std::max<int>(static_cast<int>(std::thread::hardware_concurrency()) - 2, 1);

//! helper class to change the number of thread used by poutre at scope level
//@warning We only force if OMP_NUM_THREADS environment variable is not set by user
struct BASE_API ScopedForceNbThreads
{
  explicit ScopedForceNbThreads(int nbThread);
  ScopedForceNbThreads(const ScopedForceNbThreads &) = delete;
  ScopedForceNbThreads operator=(const ScopedForceNbThreads &) = delete;
  ScopedForceNbThreads(ScopedForceNbThreads &&) = delete;
  ScopedForceNbThreads operator=(ScopedForceNbThreads &&) = delete;
  ~ScopedForceNbThreads();

private:
  int nbThreadToRestore;
  std::mutex m_mutex;
};
/**
 * RAII helpers to automatically joins on dtor
 */
class JoinThreads
{
public:
  /**
   * ctor.
   *
   * \param threads
   */
  explicit JoinThreads(std::vector<std::thread> &threads) : m_threads(threads) {}
  JoinThreads(const JoinThreads &) = delete;
  JoinThreads &operator=(const JoinThreads &) = delete;
  JoinThreads(JoinThreads &&) = delete;
  JoinThreads &operator=(JoinThreads &&) = delete;
  /**
   * dtor.
   *
   */
  ~JoinThreads()// NOLINT
  {
    for (auto &&t : m_threads) {
      if (t.joinable()) t.join();
    }
  }

private:
  std::vector<std::thread> &m_threads;
};

template<typename T> class ThreadSafeQueue
{
private:
  std::atomic_bool m_valid;
  mutable std::mutex m_mutex;
  std::queue<T> m_queue;
  std::condition_variable m_condition;

public:
  ThreadSafeQueue() : m_valid(true), m_mutex(), m_queue(), m_condition() {}

  ThreadSafeQueue(const ThreadSafeQueue &) = delete;
  ThreadSafeQueue operator=(const ThreadSafeQueue &) = delete;
  ThreadSafeQueue(ThreadSafeQueue &&) = delete;
  ThreadSafeQueue operator=(ThreadSafeQueue &&) = delete;

  /**
   * Destructor.
   */
  ~ThreadSafeQueue() { invalidate(); }

  /**
   * Check whether or not the queue is empty.
   */
  bool empty() const
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_queue.empty();
  }
  /**
   * Push a new value onto the queue.
   */
  void push(T new_value)
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_queue.push(std::move(new_value));
    m_condition.notify_one();
  }
  /**
   * Get the first value in the queue.
   * Will block until a value is available unless clear is called or the instance is destructed.
   * Returns true if a value was successfully written to the out parameter, false otherwise.
   */
  bool wait_and_pop(T &value)
  {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_condition.wait(lk, [this] { return !m_queue.empty() || !m_valid; });
    /*
     * Using the condition in the predicate ensures that spurious wakeups with a valid
     * but empty queue will not proceed, so only need to check for validity before proceeding.
     */
    if (!m_valid) { return false; }
    value = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }
  /**
   * Attempt to get the first value in the queue.
   * Returns true if a value was successfully written to the out parameter, false otherwise.
   */
  bool try_pop(T &res)
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    if (m_queue.empty() || !m_valid) { return false; }

    res = std::move(m_queue.front());
    m_queue.pop();
    return true;
  }

  /**
   * Clear all items from the queue.
   */
  void clear()
  {
    std::lock_guard<std::mutex> lock{ m_mutex };
    while (!m_queue.empty()) { m_queue.pop(); }
    m_condition.notify_all();
  }

  /**
   * Invalidate the queue.
   * Used to ensure no conditions are being waited on in waitPop when
   * a thread or the application is trying to exit.
   * The queue is invalid after calling this method and it is an error
   * to continue using a queue after this method has been called.
   */
  void invalidate()
  {
    std::lock_guard<std::mutex> lock{ m_mutex };
    m_valid = false;
    m_condition.notify_all();
  }

  /**
   * Returns whether or not this queue is valid.
   */
  bool isValid() const
  {
    std::lock_guard<std::mutex> lock{ m_mutex };
    return m_valid;
  }
  // todo add emplace back/front?
};

class IThreadTask
{
public:
  IThreadTask() = default;
  virtual ~IThreadTask() = default;

  IThreadTask(const IThreadTask &rhs) = delete;
  IThreadTask &operator=(const IThreadTask &rhs) = delete;

  IThreadTask(IThreadTask &&other) = default;
  IThreadTask &operator=(IThreadTask &&other) = default;

  /**
   * Run the task.
   */
  virtual void execute() = 0;
};

template<typename Func> class ThreadTask : public IThreadTask
{
public:
  explicit ThreadTask(Func &&func) : m_func{ std::move(func) } {}

  ~ThreadTask() override = default;

  ThreadTask(const ThreadTask &rhs) = delete;
  ThreadTask &operator=(const ThreadTask &rhs) = delete;

  ThreadTask(ThreadTask &&other) noexcept = default;
  ThreadTask &operator=(ThreadTask &&other) noexcept = default;

  /**
   * Run the task.
   */
  void execute() override { m_func(); }

private:
  Func m_func;
};

/**
 * A wrapper around a std::future that adds the behavior of futures returned from std::async.
 * Specifically, this object will block and wait for execution to finish before going out of
 * scope.
 */
template<typename T> class TaskFuture
{
public:
  explicit TaskFuture(std::future<T> &&future) : m_future{ std::move(future) } {}

  TaskFuture(const TaskFuture &rhs) = delete;
  TaskFuture &operator=(const TaskFuture &rhs) = delete;
  TaskFuture(TaskFuture &&other) noexcept = default;
  TaskFuture &operator=(TaskFuture &&other) noexcept = default;

  ~TaskFuture()
  {
    if (m_future.valid()) { m_future.get(); }
  }

  auto get() { return m_future.get(); }

private:
  std::future<T> m_future;
};

class BASE_API TreadPool
{// https://codereview.stackexchange.com/questions/221626/c17-thread-pool
private:
  ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_pool_work_queue;
  std::atomic_bool m_done;
  std::vector<std::thread> m_threads;
  JoinThreads m_joiner;
  int m_thread_count;

  /**
   * Invalidates the queue and joins all running threads.
   */
  void destroy()
  {
    m_done = true;
    m_pool_work_queue.invalidate();
    for (auto &thread : m_threads) {
      if (thread.joinable()) { thread.join(); }
    }
  }

  /**
   * Constantly running function each thread uses to acquire work items from the queue.
   */
  void worker_thread()
  {
    while (!m_done) {
      std::unique_ptr<IThreadTask> pTask{ nullptr };
      if (m_pool_work_queue.wait_and_pop(pTask)) { pTask->execute(); }
    }
  }

public:
  explicit TreadPool(int threadCount)
    : m_pool_work_queue(), m_done(false), m_threads(), m_joiner(m_threads),
      m_thread_count(std::min(POUTRE_NUM_THREADS, threadCount))
  {
    if (0U == m_thread_count) { m_thread_count = 1; }
    m_threads.reserve(static_cast<size_t>(m_thread_count));
    try {
      for (size_t i = 0; i < static_cast<size_t>(m_thread_count); ++i) {
        m_threads.emplace_back(&TreadPool::worker_thread, this);
      }
    } catch (...) {
      destroy();
      throw;
    }
  }

  TreadPool() : TreadPool(POUTRE_NUM_THREADS) {}

  TreadPool(const TreadPool &rhs) = delete;
  TreadPool &operator=(const TreadPool &rhs) = delete;
  TreadPool(TreadPool &&other) noexcept = delete;
  TreadPool &operator=(TreadPool &&other) noexcept = delete;

  ~TreadPool() { destroy(); }

  size_t capacity() const { return m_threads.size(); }

  /**
   * Submit a job to be run by the thread pool.
   */
  template<typename Func, typename... Args> auto submit(Func &&func, Args &&...args)
  {
    auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    using ResultType = std::invoke_result_t<Func, Args...>;
    using PackagedTask = std::packaged_task<ResultType()>;
    using TaskType = ThreadTask<PackagedTask>;

    PackagedTask task{ std::move(boundTask) };// NOLINT
    TaskFuture<ResultType> result{ task.get_future() };// NOLINT
    m_pool_work_queue.push(std::make_unique<TaskType>(std::move(task)));
    return result;
  }

  // rest as before
};

namespace DefaultThreadPool {
  /**
   * Get the default thread pool for the application.
   * This pool is created with std::thread::hardware_concurrency() - 1 threads.
   */
  inline TreadPool &getThreadPool()
  {
    static TreadPool defaultPool;
    return defaultPool;
  }

  /**
   * Submit a job to the default thread pool.
   */
  template<typename Func, typename... Args> inline auto submitJob(Func &&func, Args &&...args)
  {
    return getThreadPool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
  }
}// namespace DefaultThreadPool
}// namespace poutre::thread
 //! @} doxygroup: threading_group
 #if defined(POUTRE_IS_GCC) || defined(POUTRE_IS_CLANG)
 #pragma GCC diagnostic pop
 #endif