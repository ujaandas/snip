#ifndef CC_QUEUE
#define CC_QUEUE

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class CCQueue {
private:
  std::queue<T> queue;
  std::mutex mutex;
  std::condition_variable cv;

public:
  void ccpush(const T &item) {
    {
      std::unique_lock<std::mutex> lock(mutex);
      if (closed) {
        return;
      }
      queue.push(item);
    }
    cv.notify_one();
  }

  bool ccawait(T &item) {
    std::unique_lock<std::mutex> lock(mutex);

    while (queue.empty() && !closed) {
      cv.wait(lock);
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

  bool closed = false;
};

#endif // CC_QUEUE