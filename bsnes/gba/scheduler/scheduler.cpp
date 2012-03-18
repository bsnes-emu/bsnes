#include <gba/gba.hpp>

#define SCHEDULER_CPP
namespace GBA {

Scheduler scheduler;

void Scheduler::enter() {
  host_thread = co_active();
  co_switch(active_thread);
}

void Scheduler::exit(ExitReason reason) {
  exit_reason = reason;
  active_thread = co_active();
  co_switch(host_thread);
}

void Scheduler::power() {
  host_thread = co_active();
  active_thread = cpu.thread;
}

Scheduler::Scheduler() {
  sync = SynchronizeMode::None;
  exit_reason = ExitReason::UnknownEvent;
  host_thread = nullptr;
  active_thread = nullptr;
}

}
