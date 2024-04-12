#include "coroutine.hpp"

Coroutine::Coroutine(Body func)
    : stack_(sure::Stack::AllocateBytes(4097)),
      func_(std::move(func)) {
  inside_context_.Setup(
      /*stack=*/stack_.MutView(),
      /*trampoline=*/this);

  twist::assist::NewFiber(&fiber_, stack_.MutView());
}

void Coroutine::Resume() {
  caller_fiber_ = twist::assist::SwitchToFiber(fiber_.Handle());
  outside_context_.SwitchTo(inside_context_);
}

void Coroutine::Suspend() {
  twist::assist::SwitchToFiber(caller_fiber_);
  inside_context_.SwitchTo(outside_context_);
}

bool Coroutine::IsCompleted() const {
  return done_.load();
}
