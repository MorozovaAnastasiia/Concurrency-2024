#pragma once

template <typename Mutex>
class LockGuard {
 public:
  explicit LockGuard(Mutex& mutex)
      : inner_mutex_(mutex) {
    inner_mutex_.lock();
  }

  ~LockGuard() {
    inner_mutex_.unlock();
  }

  // Non-copyable
  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;

  // Non-movable
  LockGuard(LockGuard&&) = delete;
  LockGuard& operator=(LockGuard&&) = delete;

 private:
  Mutex& inner_mutex_;
};
