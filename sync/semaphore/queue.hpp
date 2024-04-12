#pragma once

#include "tagged_semaphore.hpp"

#include <deque>

// Bounded Blocking Multi-Producer/Multi-Consumer (MPMC) Queue

template <typename T>
class BoundedBlockingQueue {
 public:
  explicit BoundedBlockingQueue(size_t capacity)
      : basic_semaphore_(capacity),
        semaphore_for_tags_(0),
        mutex_semaphore_(1) {
  }

  void Put(T job) {
    auto acquired_token = basic_semaphore_.Acquire();
    auto mutex_token = mutex_semaphore_.Acquire();

    queue_.push_back(std::move(job));
    semaphore_for_tags_.Release(std::move(acquired_token));
    mutex_semaphore_.Release(std::move(mutex_token));
  }

  T Take() {
    auto acquired_tag = semaphore_for_tags_.Acquire();
    auto mutex_token = mutex_semaphore_.Acquire();

    auto result = std::move(queue_.front());
    queue_.pop_front();

    basic_semaphore_.Release(std::move(acquired_tag));

    mutex_semaphore_.Release(std::move(mutex_token));
    return result;
  }

 private:
  struct FirstTag {};
  struct SecondTag {};

 private:
  std::deque<T> queue_;
  TaggedSemaphore<FirstTag> basic_semaphore_;
  TaggedSemaphore<FirstTag> semaphore_for_tags_;
  TaggedSemaphore<SecondTag> mutex_semaphore_;
};
