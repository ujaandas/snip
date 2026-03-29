#pragma once

#include "cc_queue.hpp"
#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

namespace snip::core {

/*
  ThreadPool manages a collection of worker threads that execute tasks from a shared queue.
  Tasks are enqueued via the enqueue() method, and worker threads continuously pull and execute
  tasks until the ThreadPool is destroyed.
*/
class ThreadPool {
private:
  std::vector<std::thread> workers;
  CCQueue<std::function<void()>> tasks;

public:
  explicit ThreadPool(size_t num_threads);
  void enqueue(std::function<void()> task);
  ~ThreadPool();
};

} // namespace snip::core