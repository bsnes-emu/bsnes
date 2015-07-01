auto CPU::iwram_read(unsigned mode, uint32 addr) -> uint32 {
  if(regs.memory.control.disable) return cpu.pipeline.fetch.instruction;

  if(mode & Word) return iwram_read(Half, addr &~ 2) << 0 | iwram_read(Half, addr | 2) << 16;
  if(mode & Half) return iwram_read(Byte, addr &~ 1) << 0 | iwram_read(Byte, addr | 1) <<  8;

  return iwram[addr & 0x7fff];
}

auto CPU::iwram_write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(regs.memory.control.disable) return;

  if(mode & Word) {
    iwram_write(Half, addr &~2, word >>  0);
    iwram_write(Half, addr | 2, word >> 16);
    return;
  }

  if(mode & Half) {
    iwram_write(Byte, addr &~1, word >>  0);
    iwram_write(Byte, addr | 1, word >>  8);
    return;
  }

  iwram[addr & 0x7fff] = word;
}

auto CPU::ewram_read(unsigned mode, uint32 addr) -> uint32 {
  if(regs.memory.control.disable) return cpu.pipeline.fetch.instruction;
  if(!regs.memory.control.ewram) return iwram_read(mode, addr);

  if(mode & Word) return ewram_read(Half, addr &~ 2) << 0 | ewram_read(Half, addr | 2) << 16;
  if(mode & Half) return ewram_read(Byte, addr &~ 1) << 0 | ewram_read(Byte, addr | 1) <<  8;

  return ewram[addr & 0x3ffff];
}

auto CPU::ewram_write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(regs.memory.control.disable) return;
  if(!regs.memory.control.ewram) return iwram_write(mode, addr, word);

  if(mode & Word) {
    ewram_write(Half, addr &~2, word >>  0);
    ewram_write(Half, addr | 2, word >> 16);
    return;
  }

  if(mode & Half) {
    ewram_write(Byte, addr &~1, word >>  0);
    ewram_write(Byte, addr | 1, word >>  8);
    return;
  }

  ewram[addr & 0x3ffff] = word;
}
