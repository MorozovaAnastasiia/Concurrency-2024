#pragma once

#include <twist/ed/std/mutex.hpp>
#include <twist/ed/std/condition_variable.hpp>
#include <cstddef>

class CyclicBarrier {
 public:
  explicit CyclicBarrier(size_t participants)
      : counter_(0),
        thread_count_(participants) {
  }

  void ArriveAndWait() {
    std::unique_lock lock(mutex_);
    std::size_t curinst = inst_;
    ++counter_;
    if (counter_ == thread_count_) {
      counter_ = 0;
      ++inst_;
      cv_.notify_all();
    } else {
      cv_.wait(lock, [this, &curinst]() {
        return inst_ != curinst;
      });
    }
    lock.unlock();
  }

 private:
  size_t counter_{0};
  size_t thread_count_{0};
  size_t inst_{0};
  twist::ed::std::mutex mutex_;
  twist::ed::std::condition_variable cv_;
};
