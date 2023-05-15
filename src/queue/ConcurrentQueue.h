#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

namespace queue {
template <class T>
class ConcurrentQueue {
 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::atomic_bool stop_ = false;

 public:
  ConcurrentQueue() = default;
  ConcurrentQueue(const ConcurrentQueue&) = delete;
  ConcurrentQueue(ConcurrentQueue&&) = delete;

  ~ConcurrentQueue() {
    if (!stop_) {
      stop();
    }
  }

  void push(const T& item) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      queue_.emplace(item);
    }
    cv_.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() { return stop_ || !queue_.empty(); });
    if (stop_ && queue_.empty()) {
      return {};
    }
    auto item = std::move(queue_.front());
    queue_.pop();
    return item;
  }
  void stop() {
    stop_ = true;
    cv_.notify_one();
  }
};
}  // namespace queue