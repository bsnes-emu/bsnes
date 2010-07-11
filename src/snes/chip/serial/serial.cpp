#include <snes/snes.hpp>

#define SERIAL_CPP
namespace SNES {

Serial serial;

static void snesserial_tick(unsigned clocks) { serial.add_clocks(clocks); }
static uint8 snesserial_read() { return serial.read(); }
static void snesserial_write(uint8 data) { serial.write(data); }

void Serial::enter() {
  scheduler.clock.cop_freq = cartridge.serial_baud_rate() * 4;
  latch = 0;
  add_clocks(256 * 4);
  if(snesserial_main) snesserial_main(snesserial_tick, snesserial_read, snesserial_write);
  while(true) add_clocks(scheduler.clock.cop_freq);
}

void Serial::add_clocks(unsigned clocks) {
  scheduler.addclocks_cop(clocks);
  scheduler.sync_copcpu();
}

uint8 Serial::read() {
  while(cpu.joylatch() == 0) add_clocks(1);
  while(cpu.joylatch() == 1) add_clocks(1);
  add_clocks(2);

  uint8 data = 0;
  for(unsigned i = 0; i < 8; i++) {
    add_clocks(4);
    data = (cpu.joylatch() << 7) | (data >> 1);
  }

  return data;
}

void Serial::write(uint8 data) {
  latch = 1;
  add_clocks(4);

  for(unsigned i = 0; i < 8; i++) {
    latch = (data & 0x01) ^ 1;
    data >>= 1;
    add_clocks(4);
  }

  latch = 0;
  add_clocks(4);
}

void Serial::init() {
}

void Serial::enable() {
  if(opened()) close();
  if(open("snesserial")) {
    snesserial_main = sym("snesserial_main");
  }
}

void Serial::power() {
}

void Serial::reset() {
}

}
