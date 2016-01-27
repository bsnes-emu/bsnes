#include <ws/ws.hpp>

namespace WonderSwan {

Scheduler scheduler;

auto Scheduler::enter() -> void {
  host = co_active();
  co_switch(active);
}

auto Scheduler::exit(ExitReason reason) -> void {
  exitReason = reason;
  active = co_active();
  co_switch(host);
}

auto Scheduler::power() -> void {
  host = co_active();
  active = cpu.thread;
}

}
