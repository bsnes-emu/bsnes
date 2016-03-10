#include <sfc/sfc.hpp>

namespace SuperFamicom {

SuperDisc superdisc;
#include "nec.cpp"
#include "sony.cpp"

auto SuperDisc::Enter() -> void {
  while(true) scheduler.synchronize(), superdisc.main();
}

auto SuperDisc::main() -> void {
  cpu.regs.irq = 0;

  if(r.irqEnable.bit(3)) {
    cpu.regs.irq = 1;
    nec.data = necPollIRQ();
  }

  if(r.irqEnable.bit(2)) {
    cpu.regs.irq = 1;
    sony.data = sonyPollIRQ();
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
  r.irqEnable = 0x00;

  nec.command.reset();
  nec.data = 0x00;

  sony.command = 0x00;
  sony.data = 0x00;
}

auto SuperDisc::read(uint24 addr, uint8 data) -> uint8 {
  addr = 0x21e0 | (addr & 7);

  if(addr == 0x21e0) {
    data = 0x00;
  }

  if(addr == 0x21e1) {
    cpu.regs.irq = 0;
    data = necReadData();
  }

  if(addr == 0x21e2) {
    data = 0x00;
  }

  if(addr == 0x21e3) {
    cpu.regs.irq = 0;
    data = sonyReadData();
  }

  if(addr == 0x21e4) {
    data = r.irqEnable;
  }

  if(addr == 0x21e5) {
    data = 0x00;
  }

  return data;
}

auto SuperDisc::write(uint24 addr, uint8 data) -> void {
  addr = 0x21e0 | (addr & 7);

  if(addr == 0x21e0) {
  }

  if(addr == 0x21e1) {
    necWriteCommand(data.bits(0,3));
  }

  if(addr == 0x21e2) {
    sonyWriteCommand(data);
  }

  if(addr == 0x21e3) {
    sonyWriteData(data);
  }

  if(addr == 0x21e4) {
    r.irqEnable = data;
  }

  if(addr == 0x21e5) {
  }
}

}
