#include <sfc/sfc.hpp>

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
  while(true) {
    if(co_active() == device.controllerPort1->thread) device.controllerPort1->main();
    if(co_active() == device.controllerPort2->thread) device.controllerPort2->main();
  }
}

auto Controller::main() -> void {
  step(1);
}

auto Controller::step(uint clocks) -> void {
  clock += clocks * (uint64)cpu.frequency;
  synchronizeCPU();
}

auto Controller::synchronizeCPU() -> void {
  if(CPU::Threaded) {
    if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.main();
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
