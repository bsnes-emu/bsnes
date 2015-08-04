#include <sfc/sfc.hpp>

#define CONTROLLER_CPP
namespace SuperFamicom {

#include "gamepad/gamepad.cpp"
#include "multitap/multitap.cpp"
#include "mouse/mouse.cpp"
#include "superscope/superscope.cpp"
#include "justifier/justifier.cpp"
#include "usart/usart.cpp"

Controller::Controller(bool port) : port(port) {
  if(!thread) create(Controller::Enter, 1);
}

auto Controller::Enter() -> void {
  if(co_active() == input.port1->thread) input.port1->enter();
  if(co_active() == input.port2->thread) input.port2->enter();
}

auto Controller::enter() -> void {
  while(true) step(1);
}

auto Controller::step(unsigned clocks) -> void {
  clock += clocks * (uint64)cpu.frequency;
  synchronize_cpu();
}

auto Controller::synchronize_cpu() -> void {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.enter();
  }
}

auto Controller::iobit() -> bool {
  switch(port) {
  case Controller::Port1: return cpu.pio() & 0x40;
  case Controller::Port2: return cpu.pio() & 0x80;
  }
}

auto Controller::iobit(bool data) -> void {
  switch(port) {
  case Controller::Port1: bus.write(0x4201, (cpu.pio() & ~0x40) | (data << 6)); break;
  case Controller::Port2: bus.write(0x4201, (cpu.pio() & ~0x80) | (data << 7)); break;
  }
}

}
