#pragma once

#include <mutex>
#include <twist/ed/std/mutex.hpp>

#include <utility>
template <typename T, class Mutex = twist::ed::std::mutex>
class OwnerRef {
 private:
  T* ptr_ = nullptr;
  Mutex& mutex_;
  std::lock_guard<Mutex> lock_;

 public:
  OwnerRef(T* ptr, Mutex& mutex)
      : ptr_(ptr),
        mutex_(mutex),
        lock_(mutex_) {
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }
};

template <typename T, class Mutex = twist::ed::std::mutex>
class Mutexed {
 public:
  template <typename... Args>
  explicit Mutexed(Args&&... args)
      : object_(std::forward<Args>(args)...) {
  }

  OwnerRef<T> Acquire() {
    return OwnerRef<T, Mutex>(&object_, mutex_);
  }

 private:
  T object_;
  Mutex mutex_;  // Guards access to object_
};

template <typename T>
auto Acquire(Mutexed<T>& object) {
  return object.Acquire();
}
