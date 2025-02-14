#include <poutre/base/details/threading/poutreThreading.hpp>
#include <poutre/base/poutreConfig.hpp>

// #include <cstdlib> // getenv
#include <mutex>

namespace poutre::thread {
ScopedForceNbThreads::ScopedForceNbThreads(int nbThread) : nbThreadToRestore(POUTRE_NUM_THREADS)
{
  if (nbThread <= 0) { POUTRE_RUNTIME_ERROR("Number of thread must be a positive non zero value"); }
  const std::lock_guard<std::mutex> lock(m_mutex);
  POUTRE_NUM_THREADS = nbThread;
  // if( std::getenv("OMP_NUM_THREADS") == nullptr )
  // {
  omp_set_num_threads(POUTRE_NUM_THREADS);
  // }
}
ScopedForceNbThreads::~ScopedForceNbThreads()
{
  const std::lock_guard<std::mutex> lock(m_mutex);
  POUTRE_NUM_THREADS = nbThreadToRestore;
  // if( std::getenv("OMP_NUM_THREADS") == nullptr )
  // {
  omp_set_num_threads(POUTRE_NUM_THREADS);
  // }
}
}// namespace poutre::thread