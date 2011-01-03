#include <gameboy/gameboy.hpp>

#define SCHEDULER_CPP
namespace GameBoy {

Scheduler scheduler;

void Scheduler::enter() {
  host_thread = co_active();
  co_switch(active_thread);
}

void Scheduler::exit() {
  active_thread = co_active();
  co_switch(host_thread);
}

void Scheduler::swapto(Processor &p) {
  active_thread = p.thread;
  co_switch(active_thread);
}

void Scheduler::init() {
  host_thread = co_active();
  active_thread = cpu.thread;
}

Scheduler::Scheduler() {
  host_thread = 0;
  active_thread = 0;
}

}
