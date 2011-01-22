#ifdef BSX_CPP

BSXCart bsxcart;

void BSXCart::init() {
}

void BSXCart::enable() {
}

void BSXCart::power() {
  reset();
}

void BSXCart::reset() {
  for(unsigned i = 0; i < 16; i++) r[i] = 0x00;
  r[0x07] = 0x80;
  r[0x08] = 0x80;
  mmio_commit();
}

uint8 BSXCart::memory_access(bool write, Memory &memory, unsigned addr, uint8 data) {
  if(write == 0) return memory_read(memory, addr);
  memory_write(memory, addr, data);
}

uint8 BSXCart::memory_read(Memory &memory, unsigned addr) {
  addr = bus.mirror(addr, memory.size());
  return memory.read(addr);
}

void BSXCart::memory_write(Memory &memory, unsigned addr, uint8 data) {
  addr = bus.mirror(addr, memory.size());
  return memory.write(addr, data);
}

//mcu_access() allows mcu_read() and mcu_write() to share decoding logic
uint8 BSXCart::mcu_access(bool write, unsigned addr, uint8 data) {
  if((addr & 0xe08000) == 0x008000) {  //$00-1f:8000-ffff
    if(r07 == 1) {
      addr = ((addr & 0x1f0000) >> 1) | (addr & 0x7fff);
      return memory_access(write, memory::cartrom, addr, data);
    }
  }

  if((addr & 0xe08000) == 0x808000) {  //$80-9f:8000-ffff
    if(r08 == 1) {
      addr = ((addr & 0x1f0000) >> 1) | (addr & 0x7fff);
      return memory_access(write, memory::cartrom, addr, data);
    }
  }

  if((addr & 0xe0e000) == 0x206000) {  //$20-3f:6000-7fff
    return memory_access(write, memory::bsxpram, addr, data);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    if(r05 == 0) return memory_access(write, memory::bsxpram, addr & 0x0fffff, data);
  }

  if((addr & 0xf00000) == 0x500000) {  //$50-5f:0000-ffff
    if(r06 == 0) return memory_access(write, memory::bsxpram, addr & 0x0fffff, data);
  }

  if((addr & 0xf00000) == 0x600000) {  //$60-6f:0000-ffff
    if(r03 == 1) return memory_access(write, memory::bsxpram, addr & 0x0fffff, data);
  }

  if((addr & 0xf80000) == 0x700000) {  //$70-77:0000-ffff
    return memory_access(write, memory::bsxpram, addr & 0x07ffff, data);
  }

  if(((addr & 0xc08000) == 0x008000)  //$00-3f:8000-ffff
  || ((addr & 0xc00000) == 0x400000)  //$40-7f:0000-ffff
  || ((addr & 0xc08000) == 0x808000)  //$80-bf:8000-ffff
  || ((addr & 0xc00000) == 0xc00000)  //$c0-ff:0000-ffff
  ) {
    if(r02 == 0) addr = ((addr & 0x7f0000) >> 1) | (addr & 0x7fff);
    Memory &memory = (r01 == 0 ? (Memory&)bsxflash : (Memory&)memory::bsxpram);
    return memory_access(write, memory, addr & 0x7fffff, data);
  }

  return cpu.regs.mdr;
}

uint8 BSXCart::mcu_read(unsigned addr) {
  return mcu_access(0, addr);
}

void BSXCart::mcu_write(unsigned addr, uint8 data) {
  mcu_access(1, addr, data);
}

uint8 BSXCart::mmio_read(unsigned addr) {
  if((addr & 0xf0ffff) == 0x005000) {  //$00-0f:5000  MMIO
    uint8 n = (addr >> 16) & 15;
    return r[n];
  }

  if((addr & 0xf8f000) == 0x105000) {  //$10-17:5000-5fff  SRAM
    return memory_read(memory::bsxram, ((addr >> 16) & 7) * 0x1000 + (addr & 0xfff));
  }

  return 0x00;
}

void BSXCart::mmio_write(unsigned addr, uint8 data) {
  if((addr & 0xf0ffff) == 0x005000) {  //$00-0f:5000  MMIO
    uint8 n = (addr >> 16) & 15;
    r[n] = data;
    if(n == 0x0e && data & 0x80) mmio_commit();
    return;
  }

  if((addr & 0xf8f000) == 0x105000) {  //$10-17:5000-5fff  SRAM
    return memory_write(memory::bsxram, ((addr >> 16) & 7) * 0x1000 + (addr & 0xfff), data);
  }
}

void BSXCart::mmio_commit() {
  r00 = r[0x00] & 0x80;
  r01 = r[0x01] & 0x80;
  r02 = r[0x02] & 0x80;
  r03 = r[0x03] & 0x80;
  r04 = r[0x04] & 0x80;
  r05 = r[0x05] & 0x80;
  r06 = r[0x06] & 0x80;
  r07 = r[0x07] & 0x80;
  r08 = r[0x08] & 0x80;
  r09 = r[0x09] & 0x80;
  r0a = r[0x0a] & 0x80;
  r0b = r[0x0b] & 0x80;
  r0c = r[0x0c] & 0x80;
  r0d = r[0x0d] & 0x80;
  r0e = r[0x0e] & 0x80;
  r0f = r[0x0f] & 0x80;
}

BSXCart::BSXCart() {
}

BSXCart::~BSXCart() {
}

#endif
