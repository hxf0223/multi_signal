#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace std {

class semaphore {
  mutex mtx_;
  condition_variable cv_;
  volatile int count_;

public:
  semaphore(int count) : count_(count) {}

  void wait(void) noexcept {
    unique_lock<mutex> lck(mtx_);
    cv_.wait(lck, [&] { return count_ >= 0; });
    // count_--;
  }

  void waitReset(int count) noexcept {
    unique_lock<mutex> lck(mtx_);
    cv_.wait(lck, [&] { return count_ >= 0; });
    count_ = count;
  }

  void release(void) noexcept {
    unique_lock<mutex> lck(mtx_);
    if (++count_ >= 0) {
      cv_.notify_all();
    }
  }

  void reset(int count) noexcept {
    unique_lock<mutex> lck(mtx_);
    count_ = count;
  }
};

} // namespace std