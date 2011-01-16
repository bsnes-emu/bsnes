#ifdef SUPERFX_CPP

namespace memory {
  SuperFXCPUROM fxrom;
  SuperFXCPURAM fxram;
}

//ROM / RAM access from the S-CPU

unsigned SuperFXCPUROM::size() const {
  return memory::cartrom.size();
}

uint8 SuperFXCPUROM::read(unsigned addr) {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ron) {
    static const uint8_t data[16] = {
      0x00, 0x01, 0x00, 0x01, 0x04, 0x01, 0x00, 0x01,
      0x00, 0x01, 0x08, 0x01, 0x00, 0x01, 0x0c, 0x01,
    };
    return data[addr & 15];
  }
  return memory::cartrom.read(addr);
}

void SuperFXCPUROM::write(unsigned addr, uint8 data) {
  memory::cartrom.write(addr, data);
}

unsigned SuperFXCPURAM::size() const {
  return memory::cartram.size();
}

uint8 SuperFXCPURAM::read(unsigned addr) {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ran) return cpu.regs.mdr;
  return memory::cartram.read(addr);
}

void SuperFXCPURAM::write(unsigned addr, uint8 data) {
  memory::cartram.write(addr, data);
}

#endif
