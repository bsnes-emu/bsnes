#include <ws/ws.hpp>

namespace WonderSwan {

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "interrupt.cpp"
#include "dma.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  poll();
  exec();
}

auto CPU::step(uint clocks) -> void {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

auto CPU::wait(uint clocks) -> void {
  step(clocks);
}

auto CPU::read(uint20 addr) -> uint8 {
  return bus.read(addr);
}

auto CPU::write(uint20 addr, uint8 data) -> void {
  return bus.write(addr, data);
}

auto CPU::in(uint16 port) -> uint8 {
  return iomap[port]->portRead(port);
}

auto CPU::out(uint16 port, uint8 data) -> void {
  return iomap[port]->portWrite(port, data);
}

auto CPU::power() -> void {
  V30MZ::power();
  create(CPU::Enter, 3'072'000);

  iomap[0x00a0] = this;
  iomap[0x00b0] = this;
  iomap[0x00b2] = this;
  iomap[0x00b4] = this;
  iomap[0x00b6] = this;

  if(WSC() || SC()) {
    for(uint p = 0x0040; p <= 0x0049; p++) iomap[p] = this;
    iomap[0x0062] = this;
  }

  r.dmaSource = 0x00000;
  r.dmaTarget = 0x0000;
  r.dmaLength = 0x0000;
  r.dmaEnable = false;
  r.dmaMode = 0;
  r.interruptBase = 0x00;
  r.interruptEnable = 0x00;
  r.interruptStatus = 0x00;
}

}
