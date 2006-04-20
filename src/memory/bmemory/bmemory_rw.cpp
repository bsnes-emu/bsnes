uint8 bMemBus::read_unmapped(uint32 addr) {
  return r_cpu->regs.mdr;
}

void bMemBus::write_unmapped(uint32 addr, uint8 data) {
}

//

uint8 bMemBus::read_mmio(uint32 addr) {
  return mmio[(addr - 0x2000) & 0x3fff]->mmio_read(addr & 0x7fff);
}

void bMemBus::write_mmio(uint32 addr, uint8 data) {
  mmio[(addr - 0x2000) & 0x3fff]->mmio_write(addr & 0x7fff, data);
}

//

uint8 bMemBus::read_rom(uint32 addr) {
  return page_handle[addr >> 8][addr & 0xff];
}

void bMemBus::write_rom(uint32 addr, uint8 data) {
  if(cart_write_protect_enabled == true)return;
  page_handle[addr >> 8][addr & 0xff] = data;
}

//

uint8 bMemBus::read_ram(uint32 addr) {
  return page_handle[addr >> 8][addr & 0xff];
}

void bMemBus::write_ram(uint32 addr, uint8 data) {
  page_handle[addr >> 8][addr & 0xff] = data;
}

//

uint8 bMemBus::read_sdd1(uint32 addr) {
  addr = sdd1->offset(addr) % cartridge.cart.rom_size;
  return cartridge.rom[addr];
}

void bMemBus::write_sdd1(uint32 addr, uint8 data) {
  if(cart_write_protect_enabled == true)return;
  addr = sdd1->offset(addr) % cartridge.cart.rom_size;
  cartridge.rom[addr] = data;
}

//

uint8 bMemBus::read_c4 (uint32 addr) { return c4->read(addr); }
void  bMemBus::write_c4(uint32 addr, uint8 data) { c4->write(addr, data); }

uint8 bMemBus::read_dsp2 (uint32 addr) { return dsp2->read(addr); }
void  bMemBus::write_dsp2(uint32 addr, uint8 data) { dsp2->write(addr, data); }

uint8 bMemBus::read_obc1 (uint32 addr) { return obc1->read(addr); }
void  bMemBus::write_obc1(uint32 addr, uint8 data) { obc1->write(addr, data); }
