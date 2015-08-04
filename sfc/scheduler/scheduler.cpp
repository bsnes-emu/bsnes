#ifdef SYSTEM_CPP

Scheduler scheduler;

auto Scheduler::init() -> void {
  host_thread = co_active();
  thread = cpu.thread;
  sync = SynchronizeMode::None;
}

auto Scheduler::enter() -> void {
  host_thread = co_active();
  co_switch(thread);
}

auto Scheduler::exit(ExitReason reason) -> void {
  exit_reason = reason;
  thread = co_active();
  co_switch(host_thread);
}

auto Scheduler::debug() -> void {
  exit(ExitReason::DebuggerEvent);
}

#endif
