#include <fc/fc.hpp>

namespace Famicom {

#include "memory.cpp"
#include "timing.cpp"
#include "serialization.cpp"
CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  if(io.interruptPending) return interrupt();
  instruction();
}

auto CPU::step(uint clocks) -> void {
  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);

  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  cartridge.clock -= clocks;
  if(cartridge.clock < 0) co_switch(cartridge.thread);

  for(auto peripheral : peripherals) {
    peripheral->clock -= clocks * (uint64)peripheral->frequency;
    if(peripheral->clock < 0) co_switch(peripheral->thread);
  }
}

auto CPU::power() -> void {
  R6502::power();

  for(auto addr : range(0x0800)) ram[addr] = 0xff;
  ram[0x0008] = 0xf7;
  ram[0x0009] = 0xef;
  ram[0x000a] = 0xdf;
  ram[0x000f] = 0xbf;
}

auto CPU::reset() -> void {
  R6502::reset();
  create(CPU::Enter, 21'477'272);

  regs.pc  = bus.read(0xfffc) << 0;
  regs.pc |= bus.read(0xfffd) << 8;

  memory::fill(&io, sizeof(IO));
  io.rdyLine = 1;
}

}
