#ifdef ARMDSP_CPP

uint8 ArmDSP::bus_iread(uint32 addr) {
  if(addr >= 0x00000000 && addr <= 0x0001ffff) {
    return programROM[addr & 0x0001ffff];
  }

  if(addr >= 0x40000000 && addr <= 0x400000ff) {
    if(addr == 0x40000000) { return r40000000; }
    if(addr == 0x40000010) { return w3802; }
    if(addr == 0x40000020) { return (cputoarm << 3); }
    if(addr == 0x40000024) { return r40000024; }
    if(addr == 0x40000028) { return r40000028; }
    if(addr == 0x4000002c) { return r4000002c; }
  }

  if(addr >= 0xe0000000 && addr <= 0xe0003fff) {
    return programRAM[addr & 0x00003fff];
  }

  return 0x00;
}

void ArmDSP::bus_iwrite(uint32 addr, uint8 data) {
  if(addr >= 0x40000000 && addr <= 0x400000ff) {
    if(addr == 0x40000000) { w40000000 = data; }
    if(addr == 0x40000020) { w40000020 = data; }
    if(addr == 0x40000024) { w40000024 = data; }
    if(addr == 0x40000028) { w40000028 = data; }
    if(addr == 0x4000002c) { w4000002c = data; if(data & 2) r3804 = 0x80; }
  }

  if(addr >= 0xe0000000 && addr <= 0xe0003fff) {
    programRAM[addr & 0x00003fff] = data;
    return;
  }
}

template<unsigned size>
uint32 ArmDSP::bus_read(uint32 addr) {
  uint32 data = 0;
  data |= bus_iread(addr + 0) <<  0;
  data |= bus_iread(addr + 1) <<  8;
  data |= bus_iread(addr + 2) << 16;
  data |= bus_iread(addr + 3) << 24;

  if(addr >= 0x40000000 && addr <= 0x400000ff) {
    if(data) {
      if(size == 1) print("* ARM r", hex<8>(addr), " = ", hex<2>(data), "\n");
      if(size == 2) print("* ARM r", hex<8>(addr), " = ", hex<4>(data), "\n");
      if(size == 4) print("* ARM r", hex<8>(addr), " = ", hex<8>(data), "\n");
      usleep(20000);
    }
  }

  if(size == 1) return data & 0xff;
  if(size == 2) return data & 0xffff;
  return data;
}

template<unsigned size>
void ArmDSP::bus_write(uint32 addr, uint32 data) {
  if(addr >= 0x40000000 && addr <= 0x400000ff) {
    if(0||data) {
      if(size == 1) print("* ARM w", hex<8>(addr), " = ", hex<2>(data), "\n");
      if(size == 2) print("* ARM w", hex<8>(addr), " = ", hex<4>(data), "\n");
      if(size == 4) print("* ARM w", hex<8>(addr), " = ", hex<8>(data), "\n");
      usleep(20000);
    }
  }

  if(size == 1) data = (bus_read<1>(addr) & 0xffffff00) | (data & 0x000000ff);
  if(size == 2) data = (bus_read<2>(addr) & 0xffff0000) | (data & 0x0000ffff);
  bus_iwrite(addr + 0, data >>  0);
  bus_iwrite(addr + 1, data >>  8);
  bus_iwrite(addr + 2, data >> 16);
  bus_iwrite(addr + 3, data >> 24);
}

#endif
