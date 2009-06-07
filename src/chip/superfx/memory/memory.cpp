uint8_t SuperFX::op_read(uint16_t addr) {
  uint16_t offset = addr - regs.cbr;
  if(offset < 512) {
    if(cache.valid[offset >> 4] == false) {
      unsigned dp = offset & 0xfff0;
      unsigned sp = (regs.pbr << 16) + ((regs.cbr + dp) & 0xfff0);
      for(unsigned n = 0; n < 16; n++) {
        add_clocks(memory_access_speed);
        cache.buffer[dp++] = superfxbus.read(sp++);
      }
      cache.valid[offset >> 4] = true;
    } else {
      add_clocks(cache_access_speed);
    }
    return cache.buffer[offset];
  }

  if(regs.pbr <= 0x5f) {
    //$[00-5f]:[0000-ffff] ROM
    rombuffer_sync();
    add_clocks(memory_access_speed);
    return superfxbus.read((regs.pbr << 16) + addr);
  } else {
    //$[60-7f]:[0000-ffff] RAM
    rambuffer_sync();
    add_clocks(memory_access_speed);
    return superfxbus.read((regs.pbr << 16) + addr);
  }
}

uint8_t SuperFX::peekpipe() {
  uint8_t result = regs.pipeline;
  regs.pipeline = op_read(regs.r[15]);
  r15_modified = false;
  return result;
}

uint8_t SuperFX::pipe() {
  uint8_t result = regs.pipeline;
  regs.pipeline = op_read(++regs.r[15]);
  r15_modified = false;
  return result;
}

void SuperFX::cache_flush() {
  for(unsigned n = 0; n < 32; n++) cache.valid[n] = false;
}

uint8_t SuperFX::cache_mmio_read(uint16_t addr) {
  addr = (addr + regs.cbr) & 511;
  return cache.buffer[addr];
}

void SuperFX::cache_mmio_write(uint16_t addr, uint8_t data) {
  addr = (addr + regs.cbr) & 511;
  cache.buffer[addr] = data;
  if((addr & 15) == 15) cache.valid[addr >> 4] = true;
}

void SuperFX::memory_reset() {
  for(unsigned n = 0; n < 512; n++) cache.buffer[n] = 0x00;
  for(unsigned n = 0; n < 32; n++) cache.valid[n] = false;
  for(unsigned n = 0; n < 2; n++) {
    pixelcache[n].offset = ~0;
    pixelcache[n].bitpend = 0x00;
  }
}
