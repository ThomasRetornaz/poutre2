#include <poutre/base/details/threading/threading_helpers.hpp>
#include <poutre/base/config.hpp>
#ifdef _OPENMP
  #include <omp.h>
#endif
// #include <cstdlib> // getenv
#include <mutex>

namespace poutre::thread {

TreadPool::TreadPool() : TreadPool(POUTRE_NUM_THREADS)
{
}

TreadPool::~TreadPool() { destroy(); }


ScopedForceNbThreads::ScopedForceNbThreads(int nbThread) : nbThreadToRestore(POUTRE_NUM_THREADS)
{
  if (nbThread <= 0) { POUTRE_RUNTIME_ERROR("Number of thread must be a positive non zero value"); }
  const std::lock_guard<std::mutex> lock(m_mutex);
  POUTRE_NUM_THREADS = nbThread;
  // if( std::getenv("OMP_NUM_THREADS") == nullptr )
  // {
#ifdef _OPENMP
  omp_set_num_threads(POUTRE_NUM_THREADS);
#endif
  // }
}
ScopedForceNbThreads::~ScopedForceNbThreads()
{
  const std::lock_guard<std::mutex> lock(m_mutex);
  POUTRE_NUM_THREADS = nbThreadToRestore;
  // if( std::getenv("OMP_NUM_THREADS") == nullptr )
  // {
#ifdef _OPENMP
  omp_set_num_threads(POUTRE_NUM_THREADS);
#endif
  // }
}
}// namespace poutre::thread