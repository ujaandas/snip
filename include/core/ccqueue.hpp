#ifndef CC_QUEUE_H
#define CC_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class CCQueue {
private:
  std::atomic<bool> closed = false;
  std::queue<T> queue;
  std::mutex mutex;
  std::condition_variable cv;

public:
  void ccpush(const T &item) {
    {
      std::unique_lock<std::mutex> lock(mutex);
      if (closed.load()) {
        return;
      }
      queue.push(item);
    }
    cv.notify_one();
  }

  bool ccawait(T &item) {
    std::unique_lock<std::mutex> lock(mutex);

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
      closed.store(true);
    }
    cv.notify_all();
  }
};

#endif // CC_QUEUE_H