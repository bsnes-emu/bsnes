#include <gba/gba.hpp>

namespace GameBoyAdvance {

CPU cpu;
#include "prefetch.cpp"
#include "bus.cpp"
#include "io.cpp"
#include "memory.cpp"
#include "dma.cpp"
#include "timer.cpp"
#include "keypad.cpp"
#include "serialization.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  ARM7TDMI::irq = irq.ime && (irq.enable & irq.flag);

  if(stopped()) {
    if(!(irq.enable & irq.flag & Interrupt::Keypad)) return step(16);
    context.stopped = false;
  }

  if(halted()) {
    if(!(irq.enable & irq.flag)) return step(16);
    context.halted = false;
  }

  instruction();
}

auto CPU::step(uint clocks) -> void {
  for(auto& dma : this->dma) {
    dma.waiting = max(0, dma.waiting - (int)clocks);
  }

  if(!context.dmaActive) {
    context.dmaActive = true;
    while(true) {
      bool transferred = false;
      for(auto& dma : this->dma) transferred |= dma.run();
      if(!transferred) break;
    }
    context.dmaActive = false;
  }

  for(auto _ : range(clocks)) {
    for(auto& timer : this->timer) timer.run();
    context.clock++;
  }

  Thread::step(clocks);
  synchronize(ppu);
  synchronize(apu);
}

auto CPU::power() -> void {
  ARM7TDMI::power();
  create(CPU::Enter, system.frequency());

  for(auto& byte : iwram) byte = 0x00;
  for(auto& byte : ewram) byte = 0x00;

  for(auto n : range(4)) dma[n] = {n};
  for(auto n : range(4)) timer[n] = {n};
  serial = {};
  keypad = {};
  joybus = {};
  irq = {};
  wait = {};
  memory = {};
  prefetch = {};
  context = {};

  dma[0].source.resize(27); dma[0].latch.source.resize(27);
  dma[0].target.resize(27); dma[0].latch.target.resize(27);
  dma[0].length.resize(14); dma[0].latch.length.resize(14);

  dma[1].source.resize(28); dma[1].latch.source.resize(28);
  dma[1].target.resize(27); dma[1].latch.target.resize(27);
  dma[1].length.resize(14); dma[1].latch.length.resize(14);

  dma[2].source.resize(28); dma[2].latch.source.resize(28);
  dma[2].target.resize(27); dma[2].latch.target.resize(27);
  dma[2].length.resize(14); dma[2].latch.length.resize(14);

  dma[3].source.resize(28); dma[3].latch.source.resize(28);
  dma[3].target.resize(28); dma[3].latch.target.resize(28);
  dma[3].length.resize(16); dma[3].latch.length.resize(16);

  for(uint n = 0x0b0; n <= 0x0df; n++) bus.io[n] = this;  //DMA
  for(uint n = 0x100; n <= 0x10f; n++) bus.io[n] = this;  //Timers
  for(uint n = 0x120; n <= 0x12b; n++) bus.io[n] = this;  //Serial
  for(uint n = 0x130; n <= 0x133; n++) bus.io[n] = this;  //Keypad
  for(uint n = 0x134; n <= 0x159; n++) bus.io[n] = this;  //Serial
  for(uint n = 0x200; n <= 0x209; n++) bus.io[n] = this;  //System
  for(uint n = 0x300; n <= 0x301; n++) bus.io[n] = this;  //System
  //0x080-0x083 mirrored via gba/memory/memory.cpp        //System
}

}
