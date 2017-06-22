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
  Thread::step(clocks);
  synchronize(apu);
  synchronize(ppu);
  synchronize(cartridge);
  for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::power() -> void {
  MOS6502::BCD = 0;
  MOS6502::power();
  create(CPU::Enter, system.frequency());

  for(auto addr : range(0x0800)) ram[addr] = 0xff;
  ram[0x0008] = 0xf7;
  ram[0x0009] = 0xef;
  ram[0x000a] = 0xdf;
  ram[0x000f] = 0xbf;

  r.pc.byte(0) = bus.read(0xfffc);
  r.pc.byte(1) = bus.read(0xfffd);

  memory::fill(&io, sizeof(IO));
  io.rdyLine = 1;
}

}
