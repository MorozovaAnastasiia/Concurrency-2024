#pragma once

#include <twist/ed/std/atomic.hpp>
#include <twist/ed/wait/futex.hpp>
#include <stdint.h>

class CondVar {
 public:
  template <class Mutex>
  void Wait(Mutex& mutex) {
    auto prev_value = counter_.load();
    mutex.unlock();
    twist::ed::futex::Wait(counter_, prev_value);
    mutex.lock();
  }

  void NotifyOne() {
    auto key = twist::ed::futex::PrepareWake(counter_);
    counter_.fetch_add(1);
    twist::ed::futex::WakeOne(key);
  }

  void NotifyAll() {
    auto key = twist::ed::futex::PrepareWake(counter_);
    counter_.fetch_add(1);
    twist::ed::futex::WakeAll(key);
  }

 private:
  twist::ed::std::atomic_uint32_t counter_{0};
};