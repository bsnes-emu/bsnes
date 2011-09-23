#include <nes/nes.hpp>

namespace NES {

Scheduler scheduler;

void Scheduler::enter() {
  host_thread = co_active();
  co_switch(thread);
}

void Scheduler::exit(ExitReason reason) {
  exit_reason = reason;
  thread = co_active();
  co_switch(host_thread);
}

void Scheduler::power() {
  reset();
}

void Scheduler::reset() {
  host_thread = co_active();
  thread = cpu.thread;
  sync = SynchronizeMode::None;
  exit_reason = ExitReason::UnknownEvent;
}

}
