#pragma once

#include <cstddef>

#include <twist/ed/std/condition_variable.hpp>
#include <twist/ed/std/mutex.hpp>
#include <twist/ed/std/atomic.hpp>

class WaitGroup {
 public:
  void Add(size_t count) {
    jobs_cnt_.fetch_add(count);
  }

  void Done() {
    std::unique_lock lock{mutex_};
    jobs_cnt_.fetch_sub(1);
    if (jobs_cnt_ == 0 && is_anyone_waiting_) {
      is_anyone_waiting_.store(false);
      cv_.notify_all();
    }
  }

  void Wait() {
    std::unique_lock lock{mutex_};
    if (jobs_cnt_ != 0) {
      is_anyone_waiting_.store(true);
      cv_.wait(lock, [&] {
        return jobs_cnt_ == 0;
      });
    }
  }

 private:
  twist::ed::std::atomic_size_t jobs_cnt_{0};
  twist::ed::std::mutex mutex_;
  twist::ed::std::condition_variable cv_;
  twist::ed::std::atomic_bool is_anyone_waiting_{false};
};
