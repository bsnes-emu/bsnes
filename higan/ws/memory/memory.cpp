#include <ws/ws.hpp>

namespace WonderSwan {

IO* iomap[64 * 1024] = {nullptr};
InternalRAM iram;
Bus bus;

auto IO::power() -> void {
  static IO unmapped;
  for(auto& n : iomap) n = &unmapped;
}

auto IO::portRead(uint16 addr) -> uint8 {
//print("[", hex(addr, 4L), "]: port unmapped\n");
  return 0x00;
}

auto IO::portWrite(uint16 addr, uint8 data) -> void {
//print("[", hex(addr, 4L), "] = ", hex(data, 2L), ": port unmapped\n");
}

auto InternalRAM::power() -> void {
  for(auto& byte : memory) byte = 0x00;
}

auto InternalRAM::read(uint16 addr, uint size) -> uint32 {
  if(size == Long) return read(addr + 0, Word) << 0 | read(addr + 2, Word) << 16;
  if(size == Word) return read(addr + 0, Byte) << 0 | read(addr + 1, Byte) <<  8;

  if(addr >= 0x4000 && !system.r.depth) return 0x90;
  return memory[addr];
}

auto InternalRAM::write(uint16 addr, uint8 data) -> void {
  if(addr >= 0x4000 && !system.r.depth) return;
  memory[addr] = data;
}

auto Bus::read(uint20 addr) -> uint8 {
  if(addr.bits(16,19) == 0) return iram.read(addr);
  if(addr.bits(16,19) == 1) return cartridge.ramRead(addr);
  if(addr.bits(16,19) >= 2) return cartridge.romRead(addr);
  unreachable;
}

auto Bus::write(uint20 addr, uint8 data) -> void {
  if(addr.bits(16,19) == 0) return iram.write(addr, data);
  if(addr.bits(16,19) == 1) return cartridge.ramWrite(addr, data);
  if(addr.bits(16,19) >= 2) return cartridge.romWrite(addr, data);
}

}
