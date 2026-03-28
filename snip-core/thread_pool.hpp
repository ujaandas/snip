#pragma once

#include "cc_queue.hpp"
#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

namespace snip::core {

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