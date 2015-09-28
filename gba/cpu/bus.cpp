auto CPU::bus_idle() -> void {
  prefetch_step(1);
}

auto CPU::bus_read(unsigned mode, uint32 addr) -> uint32 {
  unsigned wait = bus_wait(mode, addr);
  unsigned word = pipeline.fetch.instruction;

  if(addr >= 0x1000'0000) {
    prefetch_step(wait);
  } else if(addr & 0x0800'0000) {
    if(mode & Prefetch && regs.wait.control.prefetch) {
      prefetch_sync(addr);
      word = prefetch_read();
      if(mode & Word) word |= prefetch_read() << 16;
    } else {
      if(!active.dma) prefetch_wait();
      step(wait - 1);
      word = addr < 0x0e00'0000 ? cartridge.rom_read(mode, addr) : cartridge.ram_read(mode, addr);
      step(1);
    }
  } else {
    prefetch_step(wait - 1);
         if(addr <  0x0200'0000) word = bios.read(mode, addr);
    else if(addr <  0x0300'0000) word = ewram_read(mode, addr);
    else if(addr <  0x0400'0000) word = iwram_read(mode, addr);
    else if(addr >= 0x0700'0000) word = ppu.oam_read(mode, addr);
    else if(addr >= 0x0600'0000) word = ppu.vram_read(mode, addr);
    else if(addr >= 0x0500'0000) word = ppu.pram_read(mode, addr);
    else if((addr & 0xffff'fc00) == 0x0400'0000) word = bus.mmio[addr & 0x3ff]->read(mode, addr);
    else if((addr & 0xff00'ffff) == 0x0400'0800) word = ((MMIO*)this)->read(mode, 0x0400'0800 | (addr & 3));
    prefetch_step(1);
  }

  return word;
}

auto CPU::bus_write(unsigned mode, uint32 addr, uint32 word) -> void {
  unsigned wait = bus_wait(mode, addr);

  if(addr >= 0x1000'0000) {
    prefetch_step(wait);
  } else if(addr & 0x0800'0000) {
    if(!active.dma) prefetch_wait();
    step(wait);
    addr < 0x0e00'0000 ? cartridge.rom_write(mode, addr, word) : cartridge.ram_write(mode, addr, word);
  } else {
    prefetch_step(wait);
         if(addr  < 0x0200'0000);
    else if(addr  < 0x0300'0000) ewram_write(mode, addr, word);
    else if(addr  < 0x0400'0000) iwram_write(mode, addr, word);
    else if(addr >= 0x0700'0000) ppu.oam_write(mode, addr, word);
    else if(addr >= 0x0600'0000) ppu.vram_write(mode, addr, word);
    else if(addr >= 0x0500'0000) ppu.pram_write(mode, addr, word);
    else if((addr & 0xffff'fc00) == 0x0400'0000) bus.mmio[addr & 0x3ff]->write(mode, addr, word);
    else if((addr & 0xff00'ffff) == 0x0400'0800) ((MMIO*)this)->write(mode, 0x0400'0800 | (addr & 3), word);
  }
}

auto CPU::bus_wait(unsigned mode, uint32 addr) -> unsigned {
  if(addr >= 0x1000'0000) return 1;  //unmapped
  if(addr <  0x0200'0000) return 1;
  if(addr <  0x0300'0000) return (16 - regs.memory.control.ewramwait) * (mode & Word ? 2 : 1);
  if(addr <  0x0500'0000) return 1;
  if(addr <  0x0700'0000) return mode & Word ? 2 : 1;
  if(addr <  0x0800'0000) return 1;

  static unsigned timings[] = {5, 4, 3, 9};
  unsigned n = timings[regs.wait.control.nwait[addr >> 25 & 3]];
  unsigned s = regs.wait.control.swait[addr >> 25 & 3];

  switch(addr & 0x0e00'0000) {
  case 0x0800'0000: s = s ? 2 : 3; break;
  case 0x0a00'0000: s = s ? 2 : 5; break;
  case 0x0c00'0000: s = s ? 2 : 9; break;
  case 0x0e00'0000: s = n; break;
  }

  bool sequential = (mode & Sequential);
  if((addr & 0x1fffe) == 0) sequential = false;  //N cycle on 16-bit ROM crossing 128KB page boundary (RAM S==N)

  unsigned clocks = sequential ? s : n;
  if(mode & Word) clocks += s;  //16-bit bus requires two transfers for words
  return clocks;
}
