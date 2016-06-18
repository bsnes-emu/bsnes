#include <fc/fc.hpp>

namespace Famicom {

Scheduler scheduler;

auto Scheduler::reset() -> void {
  host = co_active();
  resume = cpu.thread;
}

auto Scheduler::enter(Mode mode_) -> Event {
  mode = mode_;
  host = co_active();
  co_switch(resume);
  if(event == Event::Frame) ppu.refresh();
  return event;
}

auto Scheduler::exit(Event event_) -> void {
  event = event_;
  resume = co_active();
  co_switch(host);
}

auto Scheduler::synchronize(cothread_t thread) -> void {
  if(thread == cpu.thread) {
    while(enter(Mode::SynchronizeCPU) != Event::Synchronize);
  } else {
    resume = thread;
    while(enter(Mode::SynchronizeAll) != Event::Synchronize);
  }
}

auto Scheduler::synchronize() -> void {
  if(co_active() == cpu.thread && mode == Mode::SynchronizeCPU) return exit(Event::Synchronize);
  if(co_active() != cpu.thread && mode == Mode::SynchronizeAll) return exit(Event::Synchronize);
}

auto Scheduler::synchronizing() const -> bool {
  return mode == Mode::SynchronizeAll;
}

}
