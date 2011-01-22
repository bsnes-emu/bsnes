#include <gameboy/gameboy.hpp>

#define SYSTEM_CPP
namespace GameBoy {

#include "bootrom-dmg.cpp"
#include "bootrom-sgb.cpp"
#include "serialization.cpp"
System system;

void System::run() {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
  }
}

void System::runtosave() {
  scheduler.sync = Scheduler::SynchronizeMode::CPU;
  runthreadtosave();

  scheduler.active_thread = lcd.thread;
  runthreadtosave();
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    }
  }
}

uint8 System::mmio_read(uint16 addr) {
  if((addr & 0xff00) == 0x0000) {
    return BootROM::sgb[addr];
  }
  return 0x00;
}

void System::mmio_write(uint16 addr, uint8 data) {
  if(addr == 0xff50) {
    if(data == 0x01) cartridge.map();
  }
}

void System::init(Interface *interface_) {
  interface = interface_;
}

void System::power() {
  bus.power();
  cartridge.power();
  cpu.power();
  apu.power();
  lcd.power();
  scheduler.init();

  for(unsigned n = 0x0000; n <= 0x00ff; n++) bus.mmio[n] = this;
  bus.mmio[0xff50] = this;

  clocks_executed = 0;
  serialize_init();
}

}
