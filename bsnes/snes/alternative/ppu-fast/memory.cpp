#ifdef PPU_CPP

void PPU::latch_counters() {
}

uint16 PPU::get_vram_addr() {
  uint16 addr = regs.vram_addr;
  switch(regs.vram_mapping) {
    case 0: break;
    case 1: addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | ((addr >> 5) & 7); break;
    case 2: addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | ((addr >> 6) & 7); break;
    case 3: addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | ((addr >> 7) & 7); break;
  }
  return (addr << 1);
}

uint8 PPU::vram_read(unsigned addr) {
  if(regs.display_disabled == true) return memory::vram[addr];
  if(cpu.vcounter() >= regs.height) return memory::vram[addr];
  return 0x00;
}

void PPU::vram_write(unsigned addr, uint8 data) {
  if(regs.display_disabled == true) { memory::vram[addr] = data; return; }
  if(cpu.vcounter() >= regs.height) { memory::vram[addr] = data; return; }
}

uint8 PPU::oam_read(unsigned addr) {
  if(regs.display_disabled == true) return memory::oam[addr];
  if(cpu.vcounter() >= regs.height) return memory::oam[addr];
  return memory::oam[0x0218];
}

void PPU::oam_write(unsigned addr, uint8 data) {
  if(regs.display_disabled == true) { memory::oam[addr] = data; return; }
  if(cpu.vcounter() >= regs.height) { memory::oam[addr] = data; return; }
  memory::oam[0x0218] = data;
}

uint8 PPU::cgram_read(unsigned addr) {
  return memory::cgram[addr];
}

void PPU::cgram_write(unsigned addr, uint8 data) {
  memory::cgram[addr] = data;
}

#endif
