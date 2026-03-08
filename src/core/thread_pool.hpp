#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "cc_queue.hpp"
#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

class ThreadPool {
private:
  std::vector<std::thread> workers;
  CCQueue<std::function<void()>> tasks;

public:
  ThreadPool(size_t num_threads);
  void enqueue(std::function<void()> task);
  ~ThreadPool();
};

#endif // THREAD_POOL_H