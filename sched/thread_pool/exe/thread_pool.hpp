#pragma once

#include "queue.hpp"
#include "task.hpp"

#include <twist/ed/std/atomic.hpp>
#include <twist/ed/std/thread.hpp>

// Fixed-size pool of worker threads

class ThreadPool {
 public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Non-movable
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  void Start();

  void Submit(Task);

  static ThreadPool* Current();

  void Stop();

 private:
  void Worker();

  twist::ed::std::atomic_bool started_{false};
  size_t threads_ = twist::ed::std::thread::hardware_concurrency();
  std::vector<twist::ed::std::thread> workers_;
  UnboundedBlockingQueue<Task> tasks_;
};
