//20000-fffff
auto Cartridge::romRead(uint20 addr) -> uint8 {
  if(!rom.data) return 0x00;
  uint28 offset;
  switch(addr.byte(2)) {
  case 2:  offset = r.bank_rom0 << 16 | addr.bits(0,15); break;  //20000-2ffff
  case 3:  offset = r.bank_rom1 << 16 | addr.bits(0,15); break;  //30000-3ffff
  default: offset = r.bank_rom2 << 20 | addr.bits(0,19); break;  //40000-fffff
  }
  return rom.data[offset & rom.mask];
}

auto Cartridge::romWrite(uint20 addr, uint8 data) -> void {
}

//10000-1ffff
auto Cartridge::ramRead(uint20 addr) -> uint8 {
  if(!ram.data) return 0x00;
  uint24 offset = r.bank_sram << 16 | addr.bits(0,15);
  return ram.data[offset & ram.mask];
}

auto Cartridge::ramWrite(uint20 addr, uint8 data) -> void {
  if(!ram.data) return;
  uint24 offset = r.bank_sram << 16 | addr.bits(0,15);
  ram.data[offset & ram.mask] = data;
}
