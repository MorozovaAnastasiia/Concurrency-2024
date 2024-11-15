#include <wheels/test/framework.hpp>

#include "../barrier.hpp"

// https://gitlab.com/Lipovsky/tinyfiber
#include <tf/sched/spawn.hpp>
#include <tf/sched/yield.hpp>
#include <tf/sync/mutex.hpp>
#include <tf/sync/wait_group.hpp>

using tf::Mutex;
using tf::Spawn;
using tf::WaitGroup;
using tf::Yield;

void TwoFibersDeadLock() {
  Mutex a;
  Mutex b;

  auto first = [&] {
    a.Lock();
    Yield();
    b.Lock();

    a.Unlock();
    b.Unlock();
  };

  auto second = [&] {
    b.Lock();
    Yield();
    a.Lock();

    b.Unlock();
    a.Unlock();
  };

  // No deadlock with one fiber

  // No deadlock expected here
  // Run routine twice to check that
  // routine leaves mutexes in unlocked state
  Spawn(first).Join();
  Spawn(first).Join();

  // Same for `second`
  Spawn(second).Join();
  Spawn(second).Join();

  ReadyToDeadLock();

  // Deadlock with two fibers
  WaitGroup wg;
  wg.Spawn(first).Spawn(second).Wait();

  // We do not expect to reach this line
  FAIL_TEST("No deadlock =(");
}
