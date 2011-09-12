//TODO: this naming nomenclature does not work.
//AOROM and CNROM use this same chip in different pin configurations.
//this emulates AOROM currently ...

SN74HC161N sn74hc161n;

uint8 SN74HC161N::prg_read(uint16 addr) {
  if(addr & 0x8000) {
    unsigned rom_addr = (prg_bank << 15) | (addr & 0x7fff);
    return cartridge.prg_data[mirror(rom_addr, cartridge.prg_size)];
  }

  return cpu.mdr();
}

void SN74HC161N::prg_write(uint16 addr, uint8 data) {
  if(addr & 0x8000) {
    prg_bank = data & 0x0f;
    mirror_select = data & 0x10;
  }
}

uint8 SN74HC161N::chr_read(uint16 addr) {
  return cartridge.chr_data[mirror(addr, cartridge.chr_size)];
}

void SN74HC161N::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  cartridge.chr_data[mirror(addr, cartridge.chr_size)] = data;
}

uint8 SN74HC161N::ciram_read(uint13 addr) {
  return ppu.ciram_read((mirror_select << 10) | (addr & 0x03ff));
}

void SN74HC161N::ciram_write(uint13 addr, uint8 data) {
  return ppu.ciram_write((mirror_select << 10) | (addr & 0x03ff), data);
}

void SN74HC161N::power() {
  reset();
}

void SN74HC161N::reset() {
  prg_bank = 0x0f;
  mirror_select = 0;
}
