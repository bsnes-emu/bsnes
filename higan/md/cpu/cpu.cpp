#include <md/md.hpp>

namespace MegaDrive {

CPU cpu;

auto CPU::Enter() -> void {
  cpu.boot();
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::boot() -> void {
  r.a[7] = read(1, 0) << 16 | read(1, 2) << 0;
  r.pc   = read(1, 4) << 16 | read(1, 6) << 0;
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

auto CPU::read(bool word, uint24 addr) -> uint16 {
  if(addr < 0x400000) return cartridge.read(word, addr);
  return 0x0000;
}

auto CPU::write(bool word, uint24 addr, uint16 data) -> void {
  if(addr < 0x400000) return cartridge.write(word, addr, data);
}

}
