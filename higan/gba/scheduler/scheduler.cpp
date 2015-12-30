#include <gba/gba.hpp>

namespace GameBoyAdvance {

Scheduler scheduler;

Scheduler::Scheduler() {
  sync = SynchronizeMode::None;
  exit_reason = ExitReason::UnknownEvent;
  host = nullptr;
  active = nullptr;
}

auto Scheduler::enter() -> void {
  host = co_active();
  co_switch(active);
}

auto Scheduler::exit(ExitReason reason) -> void {
  exit_reason = reason;
  active = co_active();
  co_switch(host);
}

auto Scheduler::power() -> void {
  host = co_active();
  active = cpu.thread;
}

}
