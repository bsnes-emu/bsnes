#include <ws/ws.hpp>

namespace WonderSwan {

uint8 iram[64 * 1024];
IO* iomap[64 * 1024] = {nullptr};
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
  if(addr.bits(16,19) == 0) return cpu.ramRead(addr);
  if(addr.bits(16,19) == 1) return cartridge.ramRead(addr);
  if(addr.bits(16,19) >= 2) return cartridge.romRead(addr);
  unreachable;
}

auto Bus::write(uint20 addr, uint8 data) -> void {
  if(addr.bits(16,19) == 0) return cpu.ramWrite(addr, data);
  if(addr.bits(16,19) == 1) return cartridge.ramWrite(addr, data);
  if(addr.bits(16,19) >= 2) return cartridge.romWrite(addr, data);
}

}
