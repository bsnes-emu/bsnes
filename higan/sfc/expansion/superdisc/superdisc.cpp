#include <sfc/sfc.hpp>

namespace SuperFamicom {

SuperDisc superdisc;

auto SuperDisc::Enter() -> void {
  while(true) scheduler.synchronize(), superdisc.main();
}

auto SuperDisc::main() -> void {
  if(r21e4 & 0x04) {
    cpu.regs.irq = 1;
    r21e1 = 0x81;
  } else {
    cpu.regs.irq = 0;
    r21e1 = 0x00;
  }

  step(1);
  synchronizeCPU();
}

auto SuperDisc::init() -> void {
}

auto SuperDisc::load() -> void {
  bus.map({&SuperDisc::read, &superdisc}, {&SuperDisc::write, &superdisc}, 0x00, 0x3f, 0x21e0, 0x21e5);
  bus.map({&SuperDisc::read, &superdisc}, {&SuperDisc::write, &superdisc}, 0x80, 0xbf, 0x21e0, 0x21e5);
}

auto SuperDisc::unload() -> void {
}

auto SuperDisc::power() -> void {
  create(&SuperDisc::Enter, 75);
}

auto SuperDisc::reset() -> void {
  r21e0 = 0x00;
  r21e1 = 0x00;
  r21e2 = 0x00;
  r21e3 = 0x00;
  r21e4 = 0x00;
  r21e5 = 0x00;
}

auto SuperDisc::read(uint24 addr, uint8 data) -> uint8 {
  addr = 0x21e0 | (addr & 7);

  if(addr == 0x21e0) {
    data = r21e0;
  }

  if(addr == 0x21e1) {
    data = r21e1;
  }

  if(addr == 0x21e2) {
    data = r21e2;
  }

  if(addr == 0x21e3) {
    if(r21e2 == 0x01) data = 0x10;
    else data = 0x00;
    r21e2++;
  }

  if(addr == 0x21e4) {
    data = r21e4;
  }

  if(addr == 0x21e5) {
    data = r21e5;
  }

  return data;
}

auto SuperDisc::write(uint24 addr, uint8 data) -> void {
  addr = 0x21e0 | (addr & 7);

  if(addr == 0x21e0) {
    r21e0 = data;
  }

  if(addr == 0x21e1) {
    r21e1 = data;
  }

  if(addr == 0x21e2) {
    r21e2 = data;
  }

  if(addr == 0x21e3) {
    r21e2++;
    r21e3 = data;
  }

  if(addr == 0x21e4) {
    r21e4 = data;
  }

  if(addr == 0x21e5) {
    r21e5 = data;
  }
}

}
