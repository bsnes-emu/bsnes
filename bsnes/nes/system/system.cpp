#include <nes/nes.hpp>

namespace Famicom {

#include "serialization.cpp"
System system;

void System::run() {
  scheduler.enter();
}

void System::runtosave() {
  scheduler.sync = Scheduler::SynchronizeMode::PPU;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cpu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = apu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cartridge.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent);
  }
}

void System::load() {
  serialize_init();
}

void System::power() {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  input.reset();
  scheduler.power();
  reset();
}

void System::reset() {
  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  input.reset();
  scheduler.reset();
}

void System::init() {
  assert(interface != 0);
  input.connect(0, Input::Device::Joypad);
  input.connect(1, Input::Device::None);
}

void System::term() {
}

}
