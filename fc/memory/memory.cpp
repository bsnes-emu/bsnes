#include <fc/fc.hpp>

namespace Famicom {

Bus bus;

//$0000-07ff = RAM (2KB)
//$0800-1fff = RAM (mirror)
//$2000-2007 = PPU
//$2008-3fff = PPU (mirror)
//$4000-4017 = APU + I/O
//$4018-ffff = Cartridge

uint8 Bus::read(uint16 addr) {
  uint8 data = cartridge.prg_read(addr);
       if(addr <= 0x1fff) data = cpu.ram_read(addr);
  else if(addr <= 0x3fff) data = ppu.read(addr);
  else if(addr <= 0x4017) data = cpu.read(addr);

  if(cheat.enable()) {
    if(auto result = cheat.find(addr, data)) return result();
  }

  return data;
}

void Bus::write(uint16 addr, uint8 data) {
  cartridge.prg_write(addr, data);
  if(addr <= 0x1fff) return cpu.ram_write(addr, data);
  if(addr <= 0x3fff) return ppu.write(addr, data);
  if(addr <= 0x4017) return cpu.write(addr, data);
}

}
