#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace snip::core {

/*
Concurrent-safe queue.
t
*/
template <typename T> class CCQueue {
private:
  bool async = false;
  bool closed = false;
  std::queue<T> queue;
  std::mutex mutex;
  std::condition_variable cv;

public:
  explicit CCQueue(bool isAsync) : async(isAsync) {};

  void ccpush(const T& item) {
    {
      std::unique_lock<std::mutex> lock(mutex);
      if (closed) {
        return;
      }
      queue.push(item);
    }
    cv.notify_one();
  }

  bool ccawait(T& item) {
    std::unique_lock<std::mutex> lock(mutex);

    if (!async) {
      cv.wait(lock, [this] { return closed || !queue.empty(); });
    }

    if (queue.empty()) {
      return false;
    }

    item = queue.front();
    queue.pop();
    return true;
  }

  void close() {
    {
      std::unique_lock<std::mutex> lock(mutex);
      closed = true;
    }
    cv.notify_all();
  }
};

} // namespace snip::core