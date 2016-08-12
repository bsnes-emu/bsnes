#include <md/md.hpp>

namespace MegaDrive {

CPU cpu;

auto CPU::Enter() -> void {
  cpu.boot();
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::boot() -> void {
  r.a[7] = readWord(0) << 16 | readWord(2) << 0;
  r.pc   = readWord(4) << 16 | readWord(6) << 0;
}

auto CPU::main() -> void {
  #if 0
  static file fp;
  if(!fp) fp.open({Path::user(), "Desktop/trace.log"}, file::mode::write);
  fp.print(pad(disassemble(r.pc), -60, ' '), " ", disassembleRegisters().replace("\n", " "), "\n");
  #endif

  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(apu);
  synchronize(vdp);
  synchronize(psg);
  synchronize(ym2612);
}

auto CPU::power() -> void {
  M68K::power();

  for(auto& byte : ram) byte = 0x00;
}

auto CPU::reset() -> void {
  M68K::reset();
  create(CPU::Enter, system.colorburst() * 15.0 / 7.0);
  cycles = 0;
}

auto CPU::readByte(uint24 addr) -> uint8 {
  if(addr < 0x400000) return cartridge.readByte(addr);
  if(addr < 0xc00000) return 0x00;
  if(addr < 0xe00000) return vdp.readByte(addr);
  return ram[addr & 0xffff];
}

auto CPU::readWord(uint24 addr) -> uint16 {
  if(addr < 0x400000) return cartridge.readWord(addr);
  if(addr < 0xc00000) return 0x0000;
  if(addr < 0xe00000) return vdp.readWord(addr);
  uint16 data = ram[addr + 0 & 65535] << 8;
  return data | ram[addr + 1 & 65535] << 0;
}

auto CPU::writeByte(uint24 addr, uint8 data) -> void {
  if(addr < 0x400000) return cartridge.writeByte(addr, data);
  if(addr < 0xc00000) return;
  if(addr < 0xe00000) return vdp.writeByte(addr, data);
  ram[addr & 0xffff] = data;
}

auto CPU::writeWord(uint24 addr, uint16 data) -> void {
  if(addr < 0x400000) return cartridge.writeWord(addr, data);
  if(addr < 0xc00000) return;
  if(addr < 0xe00000) return vdp.writeWord(addr, data);
  ram[addr + 0 & 0xffff] = data >> 8;
  ram[addr + 1 & 0xffff] = data >> 0;
}

}
