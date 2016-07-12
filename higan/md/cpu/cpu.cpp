#include <md/md.hpp>

namespace MegaDrive {

CPU cpu;

auto CPU::Enter() -> void {
  cpu.boot();
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::boot() -> void {
  r.ssp = readLong(0);
  r.pc  = readLong(4);
}

auto CPU::main() -> void {
  instruction();
}

auto CPU::step(uint clocks) -> void {
  clock += clocks;
  if(clock >= frequency / 60) {
    clock -= frequency / 60;
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto CPU::power() -> void {
  M68K::power();
}

auto CPU::reset() -> void {
  M68K::reset();
  create(CPU::Enter, system.colorburst() * 15.0 / 7.0);
}

auto CPU::read(uint32 addr) -> uint8 {
  addr = (uint24)addr;

  if(addr < 0x400000) return cartridge.read(addr);
  return 0x00;
}

auto CPU::write(uint32 addr, uint8 data) -> void {
  addr = (uint24)addr;

  if(addr < 0x400000) return cartridge.write(addr, data);
}

}
