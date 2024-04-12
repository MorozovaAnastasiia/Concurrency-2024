#pragma once

#include <twist/ed/std/atomic.hpp>
#include <twist/ed/wait/futex.hpp>
#include <iostream>
#include <cstdint>

class Mutex {
 public:
  void Lock() {
    uint32_t idk = 0;
    if (counter_.compare_exchange_strong(idk, 1)) {
      return;
    }
    while (counter_.exchange(2) != 0) {
      twist::ed::futex::Wait(counter_, 2);
    }
  }

  void Unlock() {
    auto wake_key = twist::ed::futex::PrepareWake(counter_);
    if (counter_.exchange(0) == 2) {
      twist::ed::futex::WakeOne(wake_key);
    }
  }

  // BasicLockable
  // https://en.cppreference.com/w/cpp/named_req/BasicLockable

  void lock() {  // NOLINT
    Lock();
  }

  void unlock() {  // NOLINT
    Unlock();
  }

 private:
  twist::ed::std::atomic<uint32_t> counter_{0};
  twist::ed::std::atomic<int> threads_{0};
};
