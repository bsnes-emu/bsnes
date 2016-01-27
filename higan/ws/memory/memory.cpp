#include <ws/ws.hpp>

namespace WonderSwan {

uint8 iram[64 * 1024] = {0};
IO* io[256];
Bus bus;

auto IO::power() -> void {
  static IO unmapped;
  for(auto& n : io) n = &unmapped;
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
