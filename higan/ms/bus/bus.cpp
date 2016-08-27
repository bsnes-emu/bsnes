#include <ms/ms.hpp>

namespace MasterSystem {

Bus bus;

auto Bus::read(uint16 addr) -> uint8 {
  if(addr < 0xc000) return cartridge.read(addr);
  return ram[addr & 0x1fff];
}

auto Bus::write(uint16 addr, uint8 data) -> void {
  if(addr < 0xc000) return cartridge.write(addr, data);
  ram[addr & 0x1fff] = data;
}

auto Bus::in(uint8 addr) -> uint8 {
  switch(addr) {
  case 0x7e: return vdp.in(addr);
  case 0x7f: return vdp.in(addr);
  }
  return 0x00;
}

auto Bus::out(uint8 addr, uint8 data) -> void {
}

}
