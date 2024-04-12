#pragma once

#include <sure/context.hpp>
#include <sure/stack.hpp>
#include <twist/ed/std/atomic.hpp>
#include <twist/assist/fiber.hpp>

#include <function2/function2.hpp>
#include "sure/trampoline.hpp"

class Coroutine : public sure::ITrampoline {
 public:
  class SuspendContext {
    friend class Coroutine;

   public:
    void Suspend() {
      coro_->Suspend();
    }

   private:
    explicit SuspendContext(Coroutine* coro)
        : coro_(coro) {
    }

   private:
    Coroutine* coro_;
  };

  using Body = fu2::unique_function<void(SuspendContext)>;

 public:
  void Run() noexcept override {
    func_(SuspendContext(this));
    done_.store(true);
    inside_context_.ExitTo(outside_context_);
  }

  explicit Coroutine(Body);

  void Resume();

  bool IsCompleted() const;

 private:
  void Suspend();

 private:
  sure::Stack stack_;
  sure::ExecutionContext inside_context_;
  sure::ExecutionContext outside_context_;
  Coroutine::Body func_;
  // флажок сделанности
  twist::ed::std::atomic_bool done_{false};

  twist::assist::Fiber fiber_;
  twist::assist::FiberHandle caller_fiber_;
};
