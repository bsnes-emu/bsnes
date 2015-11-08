auto Cartridge::MROM::read(unsigned mode, uint32 addr) -> uint32 {
  if(mode & Word) addr &= ~3;
  if(mode & Half) addr &= ~1;
  auto p = data + (addr & 0x01ff'ffff);
  if(mode & Word) return p[0] << 0 | p[1] << 8 | p[2] << 16 | p[3] << 24;
  if(mode & Half) return p[0] << 0 | p[1] << 8;
  if(mode & Byte) return p[0] << 0;
  return 0;  //should never occur
}

auto Cartridge::MROM::write(unsigned mode, uint32 addr, uint32 word) -> void {
}
