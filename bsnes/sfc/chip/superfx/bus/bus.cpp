#ifdef SUPERFX_CPP

//ROM / RAM access from the S-CPU

unsigned SuperFX::CPUROM::size() const {
  return superfx.rom.size();
}

uint8 SuperFX::CPUROM::read(unsigned addr) {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ron) {
    static const uint8_t data[16] = {
      0x00, 0x01, 0x00, 0x01, 0x04, 0x01, 0x00, 0x01,
      0x00, 0x01, 0x08, 0x01, 0x00, 0x01, 0x0c, 0x01,
    };
    return data[addr & 15];
  }
  return superfx.rom.read(addr);
}

void SuperFX::CPUROM::write(unsigned addr, uint8 data) {
  superfx.rom.write(addr, data);
}

unsigned SuperFX::CPURAM::size() const {
  return superfx.ram.size();
}

uint8 SuperFX::CPURAM::read(unsigned addr) {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ran) return cpu.regs.mdr;
  return superfx.ram.read(addr);
}

void SuperFX::CPURAM::write(unsigned addr, uint8 data) {
  superfx.ram.write(addr, data);
}

#endif
