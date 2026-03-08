#include "../include/core/threadpool.hpp"

// Start all workers
ThreadPool::ThreadPool(size_t num_threads) : tasks(false) {
  for (size_t i = 0; i < num_threads; i++) {
    workers.emplace_back([this]() {
      while (true) {
        std::function<void()> task;

        // Block until a task is available OR the queue is closed
        if (!tasks.ccawait(task)) {
          return; // Exit thread
        }

        task();
      }
    });
  }
}

void ThreadPool::enqueue(std::function<void()> task) { tasks.ccpush(task); }

ThreadPool::~ThreadPool() {
  // Stop accepting items and wake up all sleeping workers
  tasks.close();

  // Wait for everyone to finish their current tasks
  for (std::thread &worker : workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}
