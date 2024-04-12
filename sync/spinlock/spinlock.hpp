#pragma once

#include "atomic.hpp"

#include <twist/ed/wait/spin.hpp>

// Naive Test-and-Set (TAS) spinlock

class TASSpinLock {
 public:
  void Lock() {
    while (locked_.Exchange(1) == 1) {
      twist::ed::CpuRelax();
    }
  }

  bool TryLock() {
    // если 1 то false
    // 0 то поставить 1
    return !(locked_.Exchange(1) == 1);
  }

  void Unlock() {
    locked_.Store(0);
  }

 private:
  Atomic locked_{0};
};
