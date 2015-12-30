#include <gb/gb.hpp>

namespace GameBoy {

Scheduler scheduler;

auto Scheduler::init() -> void {
  host_thread = co_active();
  active_thread = cpu.thread;
}

auto Scheduler::enter() -> void {
  host_thread = co_active();
  co_switch(active_thread);
}

auto Scheduler::exit(ExitReason reason) -> void {
  exit_reason = reason;
  active_thread = co_active();
  co_switch(host_thread);
}

}
