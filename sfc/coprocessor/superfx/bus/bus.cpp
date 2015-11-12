//ROM / RAM access from the S-CPU

auto SuperFX::CPUROM::size() const -> unsigned {
  return superfx.rom.size();
}

auto SuperFX::CPUROM::read(unsigned addr) -> uint8 {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ron) {
    static const uint8_t data[16] = {
      0x00, 0x01, 0x00, 0x01, 0x04, 0x01, 0x00, 0x01,
      0x00, 0x01, 0x08, 0x01, 0x00, 0x01, 0x0c, 0x01,
    };
    return data[addr & 15];
  }
  return superfx.rom.read(addr);
}

auto SuperFX::CPUROM::write(unsigned addr, uint8 data) -> void {
  superfx.rom.write(addr, data);
}

auto SuperFX::CPURAM::size() const -> unsigned {
  return superfx.ram.size();
}

auto SuperFX::CPURAM::read(unsigned addr) -> uint8 {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ran) return cpu.regs.mdr;
  return superfx.ram.read(addr);
}

auto SuperFX::CPURAM::write(unsigned addr, uint8 data) -> void {
  superfx.ram.write(addr, data);
}
