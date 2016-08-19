#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdp);
  synchronize(psg);
}

auto CPU::wait() -> void {
  step(1);
}

auto CPU::read(uint16 addr) -> uint8 {
  step(1);
  if(addr < 0xc000) return cartridge.read(addr);
  return ram[addr & 0x1fff];
}

auto CPU::write(uint16 addr, uint8 data) -> void {
  step(1);
  if(addr < 0xc000) return cartridge.write(addr, data);
  ram[addr & 0x1fff] = data;
}

auto CPU::power() -> void {
}

auto CPU::reset() -> void {
  create(CPU::Enter, system.colorburst());
}

}
