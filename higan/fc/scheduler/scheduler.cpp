#include <fc/fc.hpp>

namespace Famicom {

Scheduler scheduler;

auto Scheduler::enter() -> void {
  host_thread = co_active();
  co_switch(thread);
}

auto Scheduler::exit(ExitReason reason) -> void {
  exit_reason = reason;
  thread = co_active();
  co_switch(host_thread);
}

auto Scheduler::power() -> void {
}

auto Scheduler::reset() -> void {
  host_thread = co_active();
  thread = cpu.thread;
  sync = SynchronizeMode::None;
  exit_reason = ExitReason::UnknownEvent;
}

}
