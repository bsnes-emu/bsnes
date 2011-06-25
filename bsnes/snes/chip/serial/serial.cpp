#include <snes/snes.hpp>

#define SERIAL_CPP
namespace SNES {

Serial serial;

#include "serialization.cpp"

static void snesserial_tick(unsigned clocks) { serial.add_clocks(clocks * 8); }
static uint8 snesserial_read() { return serial.read(); }
static void snesserial_write(uint8 data) { serial.write(data); }

void Serial::Enter() { serial.enter(); }

void Serial::enter() {
  data1 = 0;
  data2 = 0;
  add_clocks(256 * 8);  //warm-up
  if(flowcontrol()) data2 = 1;
  if(main) main(snesserial_tick, snesserial_read, snesserial_write);
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
  if(flowcontrol()) while(cpu.pio() & 0x80) add_clocks(1);
  add_clocks(8);

  data1 = 1;
  add_clocks(8);

  for(unsigned i = 0; i < 8; i++) {
    data1 = (data & 1) ^ 1;
    data >>= 1;
    add_clocks(8);
  }

  data1 = 0;
  add_clocks(8);
}

uint8 Serial::mmio_read(unsigned addr) {
  cpu.synchronize_coprocessors();
  switch(addr & 1) { default:
    case 0: return cpu.mmio_read(addr);
    case 1: return cpu.mmio_read(addr);
  }
}

void Serial::mmio_write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessors();
  switch(addr & 1) { default:
    case 0: cpu.mmio_write(addr, data); break;
    case 1: cpu.mmio_write(addr, data); break;
  }
}

void Serial::init() {
}

void Serial::load() {
  if(opened()) close();
  string basename = system.interface->path(Cartridge::Slot::Base, "");
  string name = notdir(basename);
  string path = dir(basename);
  if(open(name, path)) {
    baudrate = sym("snesserial_baudrate");
    flowcontrol = sym("snesserial_flowcontrol");
    main = sym("snesserial_main");
  }

  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x4016, 0x4017, { &Serial::mmio_read, &serial }, { &Serial::mmio_write, &serial });
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x4016, 0x4017, { &Serial::mmio_read, &serial }, { &Serial::mmio_write, &serial });
}

void Serial::unload() {
  if(opened()) close();
}

void Serial::power() {
  reset();
}

void Serial::reset() {
  create(Serial::Enter, baudrate() * 8);
}

}
