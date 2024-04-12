#pragma once
#include <chrono>
#include <deque>
#include <mutex>
#include <thread>
#include <twist/ed/std/mutex.hpp>
#include <twist/ed/std/condition_variable.hpp>

#include <optional>

// Unbounded blocking multi-producers/multi-consumers (MPMC) queue

template <typename T>
// unbounded - неорганиченная очередь
class UnboundedBlockingQueue {
 public:
  // producer
  void Push(T item) {
    std::lock_guard guard(mutex_);
    buffer_.push_back(std::move(item));
    is_empty_.notify_one();
  }
  // consumer
  std::optional<T> Pop() {
    std::unique_lock lock{mutex_};

    while (buffer_.empty() && is_running_) {
      is_empty_.wait(lock);
    }

    if (!is_running_ && buffer_.empty()) {
      return std::nullopt;
    }
    return TakeLocked();
  }

  void Close() {
    std::lock_guard lock(mutex_);
    is_running_ = false;
    // ждать больше не надо, мы закончились
    is_empty_.notify_all();
  }

 private:
  T TakeLocked() {
    T front = std::move(buffer_.front());
    buffer_.pop_front();
    return std::move(front);
  }
  bool is_running_ = true;
  twist::ed::std::condition_variable is_empty_;
  std::deque<T> buffer_;
  twist::ed::std::mutex mutex_;
};
