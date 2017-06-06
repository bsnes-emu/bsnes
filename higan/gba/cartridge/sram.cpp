auto Cartridge::SRAM::read(uint mode, uint32 addr) -> uint32 {
  uint32 word = data[addr & mask];
  word |= word <<  8;
  word |= word << 16;
  return word;
}

auto Cartridge::SRAM::write(uint mode, uint32 addr, uint32 word) -> void {
  data[addr & mask] = word;
}
