auto CPU::_idle() -> void {
  prefetchStep(1);
}

auto CPU::_read(uint mode, uint32 addr) -> uint32 {
  uint clocks = _wait(mode, addr);
  uint word = pipeline.fetch.instruction;

  if(addr >= 0x1000'0000) {
    prefetchStep(clocks);
  } else if(addr & 0x0800'0000) {
    if(mode & Prefetch && wait.prefetch) {
      prefetchSync(addr);
      word = prefetchRead();
      if(mode & Word) word |= prefetchRead() << 16;
    } else {
      if(!context.dmaActive) prefetchWait();
      step(clocks - 1);
      word = cartridge.read(mode, addr);
      step(1);
    }
  } else {
    prefetchStep(clocks - 1);
         if(addr <  0x0200'0000) word = bios.read(mode, addr);
    else if(addr <  0x0300'0000) word = readEWRAM(mode, addr);
    else if(addr <  0x0400'0000) word = readIWRAM(mode, addr);
    else if(addr >= 0x0700'0000) word = ppu.readOAM(mode, addr);
    else if(addr >= 0x0600'0000) word = ppu.readVRAM(mode, addr);
    else if(addr >= 0x0500'0000) word = ppu.readPRAM(mode, addr);
    else if((addr & 0xffff'fc00) == 0x0400'0000) word = bus.io[addr & 0x3ff]->readIO(mode, addr);
    else if((addr & 0xff00'ffff) == 0x0400'0800) word = ((IO*)this)->readIO(mode, 0x0400'0800 | (addr & 3));
    prefetchStep(1);
  }

  return word;
}

auto CPU::_write(uint mode, uint32 addr, uint32 word) -> void {
  uint clocks = _wait(mode, addr);

  if(addr >= 0x1000'0000) {
    prefetchStep(clocks);
  } else if(addr & 0x0800'0000) {
    if(!context.dmaActive) prefetchWait();
    step(clocks);
    cartridge.write(mode, addr, word);
  } else {
    prefetchStep(clocks);
         if(addr  < 0x0200'0000);
    else if(addr  < 0x0300'0000) writeEWRAM(mode, addr, word);
    else if(addr  < 0x0400'0000) writeIWRAM(mode, addr, word);
    else if(addr >= 0x0700'0000) ppu.writeOAM(mode, addr, word);
    else if(addr >= 0x0600'0000) ppu.writeVRAM(mode, addr, word);
    else if(addr >= 0x0500'0000) ppu.writePRAM(mode, addr, word);
    else if((addr & 0xffff'fc00) == 0x0400'0000) bus.io[addr & 0x3ff]->writeIO(mode, addr, word);
    else if((addr & 0xff00'ffff) == 0x0400'0800) ((IO*)this)->writeIO(mode, 0x0400'0800 | (addr & 3), word);
  }
}

auto CPU::_wait(uint mode, uint32 addr) -> uint {
  if(addr >= 0x1000'0000) return 1;  //unmapped
  if(addr <  0x0200'0000) return 1;
  if(addr <  0x0300'0000) return (16 - memory.ewramWait) * (mode & Word ? 2 : 1);
  if(addr <  0x0500'0000) return 1;
  if(addr <  0x0700'0000) return mode & Word ? 2 : 1;
  if(addr <  0x0800'0000) return 1;

  static uint timings[] = {5, 4, 3, 9};
  uint n = timings[wait.nwait[addr >> 25 & 3]];
  uint s = wait.swait[addr >> 25 & 3];

  switch(addr & 0x0e00'0000) {
  case 0x0800'0000: s = s ? 2 : 3; break;
  case 0x0a00'0000: s = s ? 2 : 5; break;
  case 0x0c00'0000: s = s ? 2 : 9; break;
  case 0x0e00'0000: s = n; break;
  }

  bool sequential = (mode & Sequential);
  if((addr & 0x1fffe) == 0) sequential = false;  //N cycle on 16-bit ROM crossing 128KB page boundary (RAM S==N)

  uint clocks = sequential ? s : n;
  if(mode & Word) clocks += s;  //16-bit bus requires two transfers for words
  return clocks;
}
