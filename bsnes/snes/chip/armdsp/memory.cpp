#ifdef ARMDSP_CPP

uint32 ArmDSP::bus_read(uint32 addr) {
  if(addr >= 0x00000000 && addr <= 0x0001ffff) {
    addr &= 0x0001ffff;
    return (programROM[addr + 0] <<  0)
         + (programROM[addr + 1] <<  8)
         + (programROM[addr + 2] << 16)
         + (programROM[addr + 3] << 24);
  }

  return 0x00000000;
}

void ArmDSP::bus_write(uint32 addr, uint32 data) {
}

#endif
