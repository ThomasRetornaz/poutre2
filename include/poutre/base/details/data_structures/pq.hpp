
//==============================================================================
//                  Copyright (c) 2015 - Thomas Retornaz                      //
//                     thomas.retornaz@mines-paris.org                        //
//          Distributed under the Boost Software License, Version 1.0.        //
//                 See accompanying file LICENSE.txt or copy at               //
//                     http://www.boost.org/LICENSE_1_0.txt                   //
//==============================================================================

#pragma once

/**
 * @file pq.hpp
 * @author thomas.retornaz@mines-paris.org
 * @brief Define common pqueue data structures
 * @version 0.1
 * @date 2020-05-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <poutre/base/poutreConfig.hpp>

#include <queue>
#include <utility>
#include <vector>

namespace poutre::details {
/**
 * @addtogroup image_processing_pqueue_group Priority queue facilities
 * @ingroup image_processing_group
 *@{
 */

template<class T> struct stable_element
{
  stable_element(T &&obj, std::size_t ord) : object_(std::move(obj)), insertion_order_(ord) {}
  stable_element(const T &obj, std::size_t ord) : object_(obj), insertion_order_(ord) {}
  explicit operator T() const { return object_; }

  T object_;
  std::size_t insertion_order_;
};

template<class T> bool operator<(const stable_element<T> &lhs, const stable_element<T> &rhs)
{
  return (lhs.object_ < rhs.object_) || (!(rhs.object_ < lhs.object_) && (rhs.insertion_order_ < lhs.insertion_order_));
}

template<class T,
  class Container = std::vector<stable_element<T>>,
  class Compare = std::less<typename Container::value_type>>
class stable_priority_queue : public std::priority_queue<stable_element<T>, Container, Compare>
{
  using stableT = stable_element<T>;
  using std::priority_queue<stableT, Container, Compare>::priority_queue;

public:
  const T &top() { return this->c.front().object_; }
  void push(const T &value)
  {
    this->c.push_back(stableT(value, counter_++));
    std::push_heap(this->c.begin(), this->c.end(), this->comp);
  }
  void push(T &&value)
  {
    this->c.push_back(stableT(std::move(value), counter_++));
    std::push_heap(this->c.begin(), this->c.end(), this->comp);
  }
  template<class... Args> void emplace(Args &&...args)
  {
    this->c.emplace_back(T(std::forward<Args>(args)...), counter_++);
    std::push_heap(this->c.begin(), this->c.end(), this->comp);
  }
  void pop()
  {
    std::pop_heap(this->c.begin(), this->c.end(), this->comp);
    this->c.pop_back();
    if (this->empty()) { counter_ = 0; }
  }

protected:
  std::size_t counter_ = 0;
};

template<typename key, typename value> struct lesserKey
{
  bool operator()(const std::pair<key, value> &lhs, const std::pair<key, value> &rhs) const
  {
    return std::less<key>()(lhs.first, rhs.first);
  }
};

template<typename key, typename value> struct greaterKey
{
  bool operator()(const std::pair<key, value> &lhs, const std::pair<key, value> &rhs) const
  {
    return std::greater<key>()(lhs.first, rhs.first);
  }
};

template<class key,
  class value,
  class Container = std::vector<std::pair<key, value>>,
  class order = lesserKey<key, value>,
  typename = void>
class PriorityQueue : public std::priority_queue<std::pair<key, value>, Container, order>
{
public:
  using base = std::priority_queue<std::pair<key, value>, Container, order>;

  explicit PriorityQueue(size_t SizeReserve = 0) : base() { (this->c).reserve(SizeReserve); }

  // see this later
  PriorityQueue(const PriorityQueue &rhs) = delete;
  PriorityQueue &operator=(const PriorityQueue &rhs) = delete;
  // see this later
  PriorityQueue(PriorityQueue &&other) = delete;
  PriorityQueue &operator=(PriorityQueue &&other) = delete;
  ~PriorityQueue() = default;
};

template<typename key, typename value>
bool operator<(const stable_element<std::pair<key, value>> &lhs, const stable_element<std::pair<key, value>> &rhs)
{
  return (lhs.object_.first < rhs.object_.first)
         || (!(rhs.object_.first < lhs.object_.first) && (rhs.insertion_order_ < lhs.insertion_order_));
}

template<typename key, typename value>
bool operator>(const stable_element<std::pair<key, value>> &lhs, const stable_element<std::pair<key, value>> &rhs)
{
  return (lhs.object_.first > rhs.object_.first)
         || (!(rhs.object_.first > lhs.object_.first) && (rhs.insertion_order_ < lhs.insertion_order_));
}

template<class key,
  class value,
  class container = std::vector<stable_element<std::pair<key, value>>>,
  class order = std::less<typename container::value_type>,
  typename = void>
class PriorityQueueStable : public stable_priority_queue<std::pair<key, value>, container, order>
{
public:
  using base = stable_priority_queue<std::pair<key, value>, container, order>;

  explicit PriorityQueueStable(size_t SizeReserve = 0) : base() { (this->c).reserve(SizeReserve); }

  // see this later
  PriorityQueueStable(const PriorityQueueStable &rhs) = delete;
  PriorityQueueStable &operator=(const PriorityQueueStable &rhs) = delete;
  // see this later
  PriorityQueueStable(PriorityQueueStable &&other) = delete;
  PriorityQueueStable &operator=(PriorityQueueStable &&other) = delete;
  ~PriorityQueueStable() = default;
};

template<typename key, typename value> using poutre_pq = PriorityQueue<key, value>;

template<typename key, typename value>
using poutre_rpq = PriorityQueue<key, value, std::vector<std::pair<key, value>>, greaterKey<key, value>>;

template<typename key, typename value> using poutre_pq_stable = PriorityQueueStable<key, value>;

template<typename key, typename value>
using poutre_rpq_stable = PriorityQueueStable<key,
  value,
  std::vector<stable_element<std::pair<key, value>>>,
  std::greater<stable_element<std::pair<key, value>>>>;


// TODO specialize for sizeof(T) <16bits

//! @} doxygroup: image_processing_pqueue_group
}// namespace poutre::details
