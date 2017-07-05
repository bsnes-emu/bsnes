auto CPU::readIWRAM(uint mode, uint32 addr) -> uint32 {
  if(memory.disable) return cpu.pipeline.fetch.instruction;

  if(mode & Word) return readIWRAM(Half, addr &~ 2) << 0 | readIWRAM(Half, addr | 2) << 16;
  if(mode & Half) return readIWRAM(Byte, addr &~ 1) << 0 | readIWRAM(Byte, addr | 1) <<  8;

  return iwram[addr & 0x7fff];
}

auto CPU::writeIWRAM(uint mode, uint32 addr, uint32 word) -> void {
  if(memory.disable) return;

  if(mode & Word) {
    writeIWRAM(Half, addr &~2, word >>  0);
    writeIWRAM(Half, addr | 2, word >> 16);
    return;
  }

  if(mode & Half) {
    writeIWRAM(Byte, addr &~1, word >>  0);
    writeIWRAM(Byte, addr | 1, word >>  8);
    return;
  }

  iwram[addr & 0x7fff] = word;
}

auto CPU::readEWRAM(uint mode, uint32 addr) -> uint32 {
  if(memory.disable) return cpu.pipeline.fetch.instruction;
  if(!memory.ewram) return readIWRAM(mode, addr);

  if(mode & Word) return readEWRAM(Half, addr &~ 2) << 0 | readEWRAM(Half, addr | 2) << 16;
  if(mode & Half) return readEWRAM(Byte, addr &~ 1) << 0 | readEWRAM(Byte, addr | 1) <<  8;

  return ewram[addr & 0x3ffff];
}

auto CPU::writeEWRAM(uint mode, uint32 addr, uint32 word) -> void {
  if(memory.disable) return;
  if(!memory.ewram) return writeIWRAM(mode, addr, word);

  if(mode & Word) {
    writeEWRAM(Half, addr &~2, word >>  0);
    writeEWRAM(Half, addr | 2, word >> 16);
    return;
  }

  if(mode & Half) {
    writeEWRAM(Byte, addr &~1, word >>  0);
    writeEWRAM(Byte, addr | 1, word >>  8);
    return;
  }

  ewram[addr & 0x3ffff] = word;
}
