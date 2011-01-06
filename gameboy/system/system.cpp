#include <gameboy/gameboy.hpp>

#define SYSTEM_CPP
namespace GameBoy {

#include "bootrom-dmg.cpp"
#include "bootrom-sgb.cpp"
System system;

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
  lcd.power();
  scheduler.init();

  for(unsigned n = 0x0000; n <= 0x00ff; n++) bus.mmio[n] = this;
  bus.mmio[0xff50] = this;

  system.clocks_executed = 0;
}

void System::run() {
  scheduler.enter();
}

}
