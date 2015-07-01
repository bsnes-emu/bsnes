auto MMIO::read(unsigned mode, uint32 addr) -> uint32 {
  uint32 word = 0;

  if(mode & Word) {
    addr &= ~3;
    word |= read(addr + 0) <<  0;
    word |= read(addr + 1) <<  8;
    word |= read(addr + 2) << 16;
    word |= read(addr + 3) << 24;
  } else if(mode & Half) {
    addr &= ~1;
    word |= read(addr + 0) <<  0;
    word |= read(addr + 1) <<  8;
  } else if(mode & Byte) {
    word |= read(addr + 0) <<  0;
  }

  return word;
}

auto MMIO::write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(mode & Word) {
    addr &= ~3;
    write(addr + 0, word >>  0);
    write(addr + 1, word >>  8);
    write(addr + 2, word >> 16);
    write(addr + 3, word >> 24);
  } else if(mode & Half) {
    addr &= ~1;
    write(addr + 0, word >>  0);
    write(addr + 1, word >>  8);
  } else if(mode & Byte) {
    write(addr + 0, word >>  0);
  }
}
