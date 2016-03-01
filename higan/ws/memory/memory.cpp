#include <ws/ws.hpp>

namespace WonderSwan {

uint8 iram[64 * 1024] = {0};
IO* iomap[64 * 1024] = {0};
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

auto Bus::read(uint20 addr) -> uint8 {
  if(addr < 0x10000) return cpu.ramRead(addr);
  if(addr < 0x20000) return cartridge.ramRead(addr);
  return cartridge.romRead(addr);
}

auto Bus::write(uint20 addr, uint8 data) -> void {
  if(addr < 0x10000) return cpu.ramWrite(addr, data);
  if(addr < 0x20000) return cartridge.ramWrite(addr, data);
  return cartridge.romWrite(addr, data);
}

}
