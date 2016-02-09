auto SuperFX::bus_read(unsigned addr) -> uint8 {
  if((addr & 0xc00000) == 0x000000) {  //$00-3f:0000-7fff, $00-3f:8000-ffff
    while(!regs.scmr.ron && !scheduler.synchronizing()) {
      step(6);
      synchronizeCPU();
    }
    return rom.read((((addr & 0x3f0000) >> 1) | (addr & 0x7fff)) & rom_mask);
  }

  if((addr & 0xe00000) == 0x400000) {  //$40-5f:0000-ffff
    while(!regs.scmr.ron && !scheduler.synchronizing()) {
      step(6);
      synchronizeCPU();
    }
    return rom.read(addr & rom_mask);
  }

  if((addr & 0xe00000) == 0x600000) {  //$60-7f:0000-ffff
    while(!regs.scmr.ran && !scheduler.synchronizing()) {
      step(6);
      synchronizeCPU();
    }
    return ram.read(addr & ram_mask);
  }
}

auto SuperFX::bus_write(unsigned addr, uint8 data) -> void {
  if((addr & 0xe00000) == 0x600000) {  //$60-7f:0000-ffff
    while(!regs.scmr.ran && !scheduler.synchronizing()) {
      step(6);
      synchronizeCPU();
    }
    return ram.write(addr & ram_mask, data);
  }
}

auto SuperFX::op_read(uint16 addr) -> uint8 {
  uint16 offset = addr - regs.cbr;
  if(offset < 512) {
    if(cache.valid[offset >> 4] == false) {
      unsigned dp = offset & 0xfff0;
      unsigned sp = (regs.pbr << 16) + ((regs.cbr + dp) & 0xfff0);
      for(unsigned n = 0; n < 16; n++) {
        step(regs.clsr ? 5 : 6);
        cache.buffer[dp++] = bus_read(sp++);
      }
      cache.valid[offset >> 4] = true;
    } else {
      step(regs.clsr ? 1 : 2);
    }
    return cache.buffer[offset];
  }

  if(regs.pbr <= 0x5f) {
    //$[00-5f]:[0000-ffff] ROM
    rombuffer_sync();
    step(regs.clsr ? 5 : 6);
    return bus_read((regs.pbr << 16) + addr);
  } else {
    //$[60-7f]:[0000-ffff] RAM
    rambuffer_sync();
    step(regs.clsr ? 5 : 6);
    return bus_read((regs.pbr << 16) + addr);
  }
}

auto SuperFX::peekpipe() -> uint8 {
  uint8 result = regs.pipeline;
  regs.pipeline = op_read(regs.r[15]);
  r15_modified = false;
  return result;
}

auto SuperFX::pipe() -> uint8 {
  uint8 result = regs.pipeline;
  regs.pipeline = op_read(++regs.r[15]);
  r15_modified = false;
  return result;
}

auto SuperFX::cache_flush() -> void {
  for(unsigned n = 0; n < 32; n++) cache.valid[n] = false;
}

auto SuperFX::cache_mmio_read(uint16 addr) -> uint8 {
  addr = (addr + regs.cbr) & 511;
  return cache.buffer[addr];
}

auto SuperFX::cache_mmio_write(uint16 addr, uint8 data) -> void {
  addr = (addr + regs.cbr) & 511;
  cache.buffer[addr] = data;
  if((addr & 15) == 15) cache.valid[addr >> 4] = true;
}

auto SuperFX::memory_reset() -> void {
  rom_mask = rom.size() - 1;
  ram_mask = ram.size() - 1;

  for(unsigned n = 0; n < 512; n++) cache.buffer[n] = 0x00;
  for(unsigned n = 0; n < 32; n++) cache.valid[n] = false;
  for(unsigned n = 0; n < 2; n++) {
    pixelcache[n].offset = ~0;
    pixelcache[n].bitpend = 0x00;
  }
}
