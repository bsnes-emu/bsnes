auto Cartridge::SRAM::read(unsigned mode, uint32 addr) -> uint32 {
  if(mode & Word) addr &= ~3;
  if(mode & Half) addr &= ~1;
  uint32 word = data[addr & mask];
  word = ror(word, 8 * (addr & 3));
  if(mode & Word) { word &= 0xff; word |= word << 8; word |= word << 16; }
  if(mode & Half) { word &= 0xff; word |= word << 8; }
  return word;
}

auto Cartridge::SRAM::write(unsigned mode, uint32 addr, uint32 word) -> void {
  data[addr & mask] = ror(word, 8 * (addr & 3));
}

auto Cartridge::SRAM::serialize(serializer& s) -> void {
  s.array(data, size);
}
