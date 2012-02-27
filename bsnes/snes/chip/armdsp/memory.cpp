#ifdef ARMDSP_CPP

uint8 ArmDSP::bus_iread(uint32 addr) {
  if(addr >= 0x00000000 && addr <= 0x0001ffff) {
    return programROM[addr & 0x0001ffff];
  }

  if(addr >= 0x40000020 && addr <= 0x4000002f) {
    if(addr == 0x40000020) return cpuport[0];
    if(addr == 0x40000024) return cpuport[1];
    if(addr == 0x40000028) return cpuport[2];
    if(addr == 0x4000002c) return cpuport[3];
  }

  if(addr >= 0xe0000000 && addr <= 0xe0003fff) {
    return programRAM[addr & 0x00003fff];
  }

//print("* ARM r", hex<8>(addr), "\n");
  return 0x00;
}

void ArmDSP::bus_iwrite(uint32 addr, uint8 data) {
  if(addr >= 0x40000020 && addr <= 0x4000002f) {
    if(addr == 0x40000020) armport[0] = data;
    if(addr == 0x40000024) armport[1] = data;
    if(addr == 0x40000028) armport[2] = data;
    if(addr == 0x4000002c) armport[3] = data;
if(data) print("* ARM w", hex<8>(addr), " = ", hex<2>(data), "\n");
  }

  if(addr >= 0xe0000000 && addr <= 0xe0003fff) {
    programRAM[addr & 0x00003fff] = data;
    return;
  }

//print("* ARM w", hex<8>(addr), " = ", hex<2>(data), "\n");
}

template<unsigned size>
uint32 ArmDSP::bus_read(uint32 addr) {
  uint32 result = 0;
  result |= bus_iread(addr + 0) <<  0;
  result |= bus_iread(addr + 1) <<  8;
  result |= bus_iread(addr + 2) << 16;
  result |= bus_iread(addr + 3) << 24;
  if(size == 1) return result & 0xff;
  if(size == 2) return result & 0xffff;
  return result;
}

template<unsigned size>
void ArmDSP::bus_write(uint32 addr, uint32 data) {
  if(size == 1) data = (bus_read<1>(addr) & 0xffffff00) | (data & 0x000000ff);
  if(size == 2) data = (bus_read<2>(addr) & 0xffff0000) | (data & 0x0000ffff);
  bus_iwrite(addr + 0, data >>  0);
  bus_iwrite(addr + 1, data >>  8);
  bus_iwrite(addr + 2, data >> 16);
  bus_iwrite(addr + 3, data >> 24);
}

#endif
