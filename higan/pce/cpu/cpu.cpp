#include <pce/pce.hpp>

namespace PCEngine {

CPU cpu;

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  #if 1
  static uint counter = 0;
  if(counter++ < 40) {
    print(disassemble(r.pc), "\n");
  }
  #endif

  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdc);
  synchronize(psg);
  for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::power() -> void {
  HuC6280::power();
  create(CPU::Enter, system.colorburst() * 6.0);

  r.pc.byte(0) = read(0x1ffe);
  r.pc.byte(1) = read(0x1fff);
}

auto CPU::read(uint21 addr) -> uint8 {
  if(!addr.bit(20)) return cartridge.read(addr);
  uint8 bank = addr.bits(13,20);
  addr = addr.bits(0,12);
  if(bank >= 0xf8 && bank <= 0xfb) return ram[addr];
  if(bank == 0xff) return 0x00;  //hardware
  return 0xff;
}

auto CPU::write(uint21 addr, uint8 data) -> void {
  if(!addr.bit(20)) return cartridge.write(addr, data);
  uint8 bank = addr.bits(13,20);
  addr = addr.bits(0,12);
  if(bank >= 0xf8 && bank <= 0xfb) { ram[addr] = data; return; }
  if(bank == 0xff) return;  //hardware
}

auto CPU::st(uint2 addr, uint8 data) -> void {
}

auto CPU::lastCycle() -> void {
}

}
