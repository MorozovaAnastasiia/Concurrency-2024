#include <exe/thread_pool.hpp>

#include <twist/ed/static/thread_local/ptr.hpp>

#include <wheels/core/panic.hpp>
TWISTED_STATIC_THREAD_LOCAL_PTR(ThreadPool, current_pool_);

ThreadPool::ThreadPool(size_t threads)
    : threads_(threads) {
}

void ThreadPool::Start() {
  started_.store(true);
  for (size_t i = 0; i < threads_; ++i) {
    workers_.emplace_back([this] {
      current_pool_ = this;
      Worker();
    });
  }
}

ThreadPool::~ThreadPool() {
  assert(!started_.load());
}

void ThreadPool::Submit(Task task) {
  tasks_.Push(std::move(task));
}

ThreadPool* ThreadPool::Current() {
  return current_pool_;
}

void ThreadPool::Stop() {
  tasks_.Close();
  started_.store(false);
  for (size_t i = 0; i < threads_; ++i) {
    workers_[i].join();
  }
}

void ThreadPool::Worker() {
  while (started_.load()) {
    std::optional<Task> cur_task = tasks_.Pop();
    if (cur_task.has_value()) {
      try {
        cur_task.value()();
      } catch (...) {
        std::abort();
      }
    }
  }
}
