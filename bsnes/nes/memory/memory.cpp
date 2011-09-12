#include <nes/nes.hpp>

namespace NES {

Bus bus;

//$0000-07ff = RAM (2KB)
//$0800-1fff = RAM (mirror)
//$2000-2007 = PPU
//$2008-3fff = PPU (mirror)
//$4000-4017 = APU + I/O
//$4018-ffff = Cartridge

uint8 Bus::read(uint16 addr) {
  uint8 data = cartridge.prg_read(addr);
  if(addr <= 0x1fff) return cpu.ram_read(addr);
  if(addr <= 0x3fff) return ppu.read(addr);
  if(addr <= 0x4017) return cpu.read(addr);
  return data;
}

void Bus::write(uint16 addr, uint8 data) {
  cartridge.prg_write(addr, data);
  if(addr <= 0x1fff) return cpu.ram_write(addr, data);
  if(addr <= 0x3fff) return ppu.write(addr, data);
  if(addr <= 0x4017) return cpu.write(addr, data);
}

}
