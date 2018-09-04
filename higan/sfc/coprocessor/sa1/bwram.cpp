auto SA1::BWRAM::conflict() const -> bool {
  if(configuration.hacks.coprocessors.delayedSync) return false;

  if(!cpu.r.rwb) return false;
  if((cpu.r.mar & 0x40e000) == 0x006000) return true;  //00-3f,80-bf:6000-7fff
  if((cpu.r.mar & 0xf00000) == 0x400000) return true;  //40-4f:0000-ffff
  return false;
}

auto SA1::BWRAM::read(uint24 address, uint8 data) -> uint8 {
  if(!size()) return data;
  address = bus.mirror(address, size());
  return WritableMemory::read(address, data);
}

auto SA1::BWRAM::write(uint24 address, uint8 data) -> void {
  if(!size()) return;
  address = bus.mirror(address, size());
  return WritableMemory::write(address, data);
}

//note: addresses are translated prior to invoking this function:
//00-3f,80-bf:6000-7fff size=0x2000 => 00:0000-1fff
//40-4f:0000-ffff => untranslated
auto SA1::BWRAM::readCPU(uint24 address, uint8 data) -> uint8 {
  cpu.synchronize(sa1);

  if(address < 0x2000) {  //$00-3f,80-bf:6000-7fff
    address = sa1.mmio.sbm * 0x2000 + (address & 0x1fff);
  }

  if(dma) return sa1.dmaCC1Read(address);
  return read(address, data);
}

auto SA1::BWRAM::writeCPU(uint24 address, uint8 data) -> void {
  cpu.synchronize(sa1);

  if(address < 0x2000) {  //$00-3f,80-bf:6000-7fff
    address = sa1.mmio.sbm * 0x2000 + (address & 0x1fff);
  }

  return write(address, data);
}

auto SA1::BWRAM::readSA1(uint24 address, uint8 data) -> uint8 {
  if(sa1.mmio.sw46 == 0) {
    //$40-43:0000-ffff x  32 projection
    address = (sa1.mmio.cbm & 0x1f) * 0x2000 + (address & 0x1fff);
    return readLinear(address, data);
  } else {
    //$60-6f:0000-ffff x 128 projection
    address = sa1.mmio.cbm * 0x2000 + (address & 0x1fff);
    return readBitmap(address, data);
  }
}

auto SA1::BWRAM::writeSA1(uint24 address, uint8 data) -> void {
  if(sa1.mmio.sw46 == 0) {
    //$40-43:0000-ffff x  32 projection
    address = (sa1.mmio.cbm & 0x1f) * 0x2000 + (address & 0x1fff);
    return writeLinear(address, data);
  } else {
    //$60-6f:0000-ffff x 128 projection
    address = sa1.mmio.cbm * 0x2000 + (address & 0x1fff);
    return writeBitmap(address, data);
  }
}

auto SA1::BWRAM::readLinear(uint24 address, uint8 data) -> uint8 {
  return read(address, data);
}

auto SA1::BWRAM::writeLinear(uint24 address, uint8 data) -> void {
  return write(address, data);
}

auto SA1::BWRAM::readBitmap(uint20 address, uint8 data) -> uint8 {
  if(sa1.mmio.bbf == 0) {
    //4bpp
    uint shift = address & 1;
    address >>= 1;
    switch(shift) {
    case 0: return read(address).bits(0,3);
    case 1: return read(address).bits(4,7);
    }
  } else {
    //2bpp
    uint shift = address & 3;
    address >>= 2;
    switch(shift) {
    case 0: return read(address).bits(0,1);
    case 1: return read(address).bits(2,3);
    case 2: return read(address).bits(4,5);
    case 3: return read(address).bits(6,7);
    }
  }
  unreachable;
}

auto SA1::BWRAM::writeBitmap(uint20 address, uint8 data) -> void {
  if(sa1.mmio.bbf == 0) {
    //4bpp
    uint shift = address & 1;
    address >>= 1;
    switch(shift) {
    case 0: data = read(address) & 0xf0 | data.bits(0,3) << 0; break;
    case 1: data = read(address) & 0x0f | data.bits(0,3) << 4; break;
    }
  } else {
    //2bpp
    uint shift = address & 3;
    address >>= 2;
    switch(shift) {
    case 0: data = read(address) & 0xfc | data.bits(0,1) << 0; break;
    case 1: data = read(address) & 0xf3 | data.bits(0,1) << 2; break;
    case 2: data = read(address) & 0xcf | data.bits(0,1) << 4; break;
    case 3: data = read(address) & 0x3f | data.bits(0,1) << 6; break;
    }
  }
  write(address, data);
}
