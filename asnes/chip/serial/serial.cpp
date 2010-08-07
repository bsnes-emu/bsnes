#include <snes.hpp>

#define SERIAL_CPP
namespace SNES {

Serial serial;

#include "serialization.cpp"

static void snesserial_tick(unsigned clocks) { serial.add_clocks(clocks * 8); }
static uint8 snesserial_read() { return serial.read(); }
static void snesserial_write(uint8 data) { serial.write(data); }

void Serial::Enter() { serial.enter(); }

void Serial::enter() {
  latch = 0;
  add_clocks(256 * 8);  //warm-up
  if(snesserial_main) snesserial_main(snesserial_tick, snesserial_read, snesserial_write);
  while(true) add_clocks(frequency);  //snesserial_main() fallback
}

void Serial::add_clocks(unsigned clocks) {
  step(clocks);
  synchronize_cpu();
}

uint8 Serial::read() {
  while(cpu.joylatch() == 0) add_clocks(1);
  while(cpu.joylatch() == 1) add_clocks(1);
  add_clocks(4);

  uint8 data = 0;
  for(unsigned i = 0; i < 8; i++) {
    add_clocks(8);
    data = (cpu.joylatch() << 7) | (data >> 1);
  }

  return data;
}

void Serial::write(uint8 data) {
  latch = 1;
  add_clocks(8);

  for(unsigned i = 0; i < 8; i++) {
    latch = (data & 1) ^ 1;
    data >>= 1;
    add_clocks(8);
  }

  latch = 0;
  add_clocks(8);
}

uint8 Serial::mmio_read(unsigned addr) {
  cpu.synchronize_coprocessor();
  switch(addr & 1) { default:
    case 0: return r4016->mmio_read(addr);
    case 1: return r4017->mmio_read(addr);
  }
}

void Serial::mmio_write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessor();
  switch(addr & 1) { default:
    case 0: r4016->mmio_write(addr, data); break;
    case 1: r4017->mmio_write(addr, data); break;
  }
}

void Serial::init() {
}

void Serial::enable() {
  r4016 = memory::mmio.mmio[0x4016 - 0x2000];
  r4017 = memory::mmio.mmio[0x4017 - 0x2000];
  memory::mmio.mmio[0x4016 - 0x2000] = this;
  memory::mmio.mmio[0x4017 - 0x2000] = this;

  if(opened()) close();
  string name = notdir(cartridge.basename());
  string path = dir(cartridge.basename());
  if(open(name, path)) {
    snesserial_main = sym("snesserial_main");
  }
}

void Serial::power() {
  reset();
}

void Serial::reset() {
  create(Serial::Enter, cartridge.serial_baud_rate() * 8);
}

}
