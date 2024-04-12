#pragma once

#include <twist/ed/std/mutex.hpp>
#include <twist/ed/std/condition_variable.hpp>

#include <cstdlib>

class Semaphore {
 public:
  explicit Semaphore(size_t tokens)
      : token_count_(tokens) {
  }

  void Acquire() {
    std::unique_lock lock(mutex_);

    while (token_count_ == 0) {
      cv_.wait(lock);
    }
    --token_count_;
  }

  void Release() {
    std::unique_lock lock(mutex_);

    ++token_count_;
    cv_.notify_one();
  }

 private:
  size_t token_count_;
  twist::ed::std::mutex mutex_;
  twist::ed::std::condition_variable cv_;
};
