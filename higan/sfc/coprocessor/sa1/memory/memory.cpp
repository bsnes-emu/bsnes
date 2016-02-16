auto SA1::bus_read(uint24 addr, uint8 data) -> uint8 {
  if((addr & 0x40fe00) == 0x002200) {  //$00-3f,80-bf:2200-23ff
    return mmio_read(addr, data);
  }

  if((addr & 0x408000) == 0x008000) {  //$00-3f,80-bf:8000-ffff
    addr = ((addr & 0x800000) >> 2) | ((addr & 0x3f0000) >> 1) | (addr & 0x7fff);
    return mmcrom_read(addr, data);
  }

  if((addr & 0xc00000) == 0xc00000) {  //$c0-ff:0000-ffff
    return mmcrom_read(addr, data);
  }

  if((addr & 0x40e000) == 0x006000) {  //$00-3f,80-bf:6000-7fff
    return mmc_sa1_read(addr, data);
  }

  if((addr & 0x40f800) == 0x000000) {  //$00-3f,80-bf:0000-07ff
    synchronizeCPU();
    return iram.read(addr & 2047, data);
  }

  if((addr & 0x40f800) == 0x003000) {  //$00-3f,80-bf:3000-37ff
    synchronizeCPU();
    return iram.read(addr & 2047, data);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    synchronizeCPU();
    return bwram.read(addr & (bwram.size() - 1), data);
  }

  if((addr & 0xf00000) == 0x600000) {  //$60-6f:0000-ffff
    synchronizeCPU();
    return bitmap_read(addr & 0x0fffff, data);
  }

  //unmapped region
  return data;
}

auto SA1::bus_write(uint24 addr, uint8 data) -> void {
  if((addr & 0x40fe00) == 0x002200) {  //$00-3f,80-bf:2200-23ff
    return mmio_write(addr, data);
  }

  if((addr & 0x40e000) == 0x006000) {  //$00-3f,80-bf:6000-7fff
    return mmc_sa1_write(addr, data);
  }

  if((addr & 0x40f800) == 0x000000) {  //$00-3f,80-bf:0000-07ff
    synchronizeCPU();
    return iram.write(addr & 2047, data);
  }

  if((addr & 0x40f800) == 0x003000) {  //$00-3f,80-bf:3000-37ff
    synchronizeCPU();
    return iram.write(addr & 2047, data);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    synchronizeCPU();
    return bwram.write(addr & (bwram.size() - 1), data);
  }

  if((addr & 0xf00000) == 0x600000) {  //$60-6f:0000-ffff
    synchronizeCPU();
    return bitmap_write(addr & 0x0fffff, data);
  }
}

//$230c (VDPL), $230d (VDPH) use this bus to read variable-length data.
//this is used both to keep VBR-reads from accessing MMIO registers, and
//to avoid syncing the S-CPU and SA-1*; as both chips are able to access
//these ports.
auto SA1::vbr_read(uint24 addr, uint8 data) -> uint8 {
  if((addr & 0x408000) == 0x008000) {  //$00-3f,80-bf:8000-ffff
    addr = ((addr & 0x800000) >> 2) | ((addr & 0x3f0000) >> 1) | (addr & 0x7fff);
    return mmcrom_read(addr, data);
  }

  if((addr & 0xc00000) == 0xc00000) {  //$c0-ff:0000-ffff
    return mmcrom_read(addr, data);
  }

  if((addr & 0x40e000) == 0x006000) {  //$00-3f,80-bf:6000-7fff
    return bwram.read(addr & (bwram.size() - 1), data);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    return bwram.read(addr & (bwram.size() - 1), data);
  }

  if((addr & 0x40f800) == 0x000000) {  //$00-3f,80-bf:0000-07ff
    return iram.read(addr & 2047, data);
  }

  if((addr & 0x40f800) == 0x003000) {  //$00-3f,80-bf:3000-37ff
    return iram.read(addr & 2047, data);
  }

  return 0x00;
}

//ROM, I-RAM and MMIO registers are accessed at ~10.74MHz (2 clock ticks)
//BW-RAM is accessed at ~5.37MHz (4 clock ticks)
//tick() == 2 clock ticks
//note: bus conflict delays are not emulated at this time

auto SA1::op_io() -> void {
  tick();
}

auto SA1::op_read(uint24 addr) -> uint8 {
  tick();
  if(((addr & 0x40e000) == 0x006000) || ((addr & 0xd00000) == 0x400000)) tick();
  return bus_read(addr, regs.mdr);
}

auto SA1::op_write(uint24 addr, uint8 data) -> void {
  tick();
  if(((addr & 0x40e000) == 0x006000) || ((addr & 0xd00000) == 0x400000)) tick();
  bus_write(addr, regs.mdr = data);
}

//note: addresses are translated prior to invoking this function:
//$00-3f,80-bf:8000-ffff mask=0x408000 => $00-3f:0000-ffff
//$c0-ff:0000-ffff mask=0
auto SA1::mmcrom_read(uint24 addr, uint8) -> uint8 {
  //reset vector overrides
  if((addr & 0xffffe0) == 0x007fe0) {  //$00:ffe0-ffef
    if(addr == 0x7fea && sa1.mmio.cpu_nvsw) return sa1.mmio.snv >> 0;
    if(addr == 0x7feb && sa1.mmio.cpu_nvsw) return sa1.mmio.snv >> 8;
    if(addr == 0x7fee && sa1.mmio.cpu_ivsw) return sa1.mmio.siv >> 0;
    if(addr == 0x7fef && sa1.mmio.cpu_ivsw) return sa1.mmio.siv >> 8;
  }

  static auto read = [](uint addr) {
    return sa1.rom.read(bus.mirror(addr, sa1.rom.size()));
  };

  bool lo = addr < 0x400000;  //*bmode==0 only applies to $00-3f,80-bf:8000-ffff
  addr &= 0x3fffff;

  if(addr < 0x100000) {  //$00-1f,8000-ffff; $c0-cf:0000-ffff
    if(lo && mmio.cbmode == 0) return read(addr);
    return read((mmio.cb << 20) | (addr & 0x0fffff));
  }

  if(addr < 0x200000) {  //$20-3f,8000-ffff; $d0-df:0000-ffff
    if(lo && mmio.dbmode == 0) return read(addr);
    return read((mmio.db << 20) | (addr & 0x0fffff));
  }

  if(addr < 0x300000) {  //$80-9f,8000-ffff; $e0-ef:0000-ffff
    if(lo && mmio.ebmode == 0) return read(addr);
    return read((mmio.eb << 20) | (addr & 0x0fffff));
  }

  if(addr < 0x400000) {  //$a0-bf,8000-ffff; $f0-ff:0000-ffff
    if(lo && mmio.fbmode == 0) return read(addr);
    return read((mmio.fb << 20) | (addr & 0x0fffff));
  }

  return 0x00;
}

auto SA1::mmcrom_write(uint24 addr, uint8 data) -> void {
}

auto SA1::mmcbwram_read(uint24 addr, uint8 data) -> uint8 {
  if(addr < 0x2000) {  //$00-3f,80-bf:6000-7fff
    cpu.synchronizeCoprocessors();
    addr = bus.mirror(mmio.sbm * 0x2000 + (addr & 0x1fff), cpubwram.size());
    return cpubwram.read(addr);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    return cpubwram.read(addr & 0x0fffff);
  }

  return data;
}

auto SA1::mmcbwram_write(uint24 addr, uint8 data) -> void {
  if(addr < 0x2000) {  //$00-3f,80-bf:6000-7fff
    cpu.synchronizeCoprocessors();
    addr = bus.mirror(mmio.sbm * 0x2000 + (addr & 0x1fff), cpubwram.size());
    return cpubwram.write(addr, data);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    return cpubwram.write(addr & 0x0fffff, data);
  }
}

auto SA1::mmc_sa1_read(uint addr, uint8 data) -> uint8 {
  synchronizeCPU();
  if(mmio.sw46 == 0) {
    //$40-43:0000-ffff x  32 projection
    addr = bus.mirror((mmio.cbm & 0x1f) * 0x2000 + (addr & 0x1fff), bwram.size());
    return bwram.read(addr, data);
  } else {
    //$60-6f:0000-ffff x 128 projection
    addr = bus.mirror(mmio.cbm * 0x2000 + (addr & 0x1fff), 0x100000);
    return bitmap_read(addr, data);
  }
}

auto SA1::mmc_sa1_write(uint addr, uint8 data) -> void {
  synchronizeCPU();
  if(mmio.sw46 == 0) {
    //$40-43:0000-ffff x  32 projection
    addr = bus.mirror((mmio.cbm & 0x1f) * 0x2000 + (addr & 0x1fff), bwram.size());
    bwram.write(addr, data);
  } else {
    //$60-6f:0000-ffff x 128 projection
    addr = bus.mirror(mmio.cbm * 0x2000 + (addr & 0x1fff), 0x100000);
    bitmap_write(addr, data);
  }
}

auto SA1::bitmap_read(uint addr, uint8 data) -> uint8 {
  if(mmio.bbf == 0) {
    //4bpp
    uint shift = addr & 1;
    addr = (addr >> 1) & (bwram.size() - 1);
    switch(shift) {
    case 0: return (bwram.read(addr) >> 0) & 15;
    case 1: return (bwram.read(addr) >> 4) & 15;
    }
  } else {
    //2bpp
    uint shift = addr & 3;
    addr = (addr >> 2) & (bwram.size() - 1);
    switch(shift) {
    case 0: return (bwram.read(addr) >> 0) & 3;
    case 1: return (bwram.read(addr) >> 2) & 3;
    case 2: return (bwram.read(addr) >> 4) & 3;
    case 3: return (bwram.read(addr) >> 6) & 3;
    }
  }
}

auto SA1::bitmap_write(uint addr, uint8 data) -> void {
  if(mmio.bbf == 0) {
    //4bpp
    uint shift = addr & 1;
    addr = (addr >> 1) & (bwram.size() - 1);
    switch(shift) {
    case 0: data = (bwram.read(addr) & 0xf0) | ((data & 15) << 0); break;
    case 1: data = (bwram.read(addr) & 0x0f) | ((data & 15) << 4); break;
    }
  } else {
    //2bpp
    uint shift = addr & 3;
    addr = (addr >> 2) & (bwram.size() - 1);
    switch(shift) {
    case 0: data = (bwram.read(addr) & 0xfc) | ((data &  3) << 0); break;
    case 1: data = (bwram.read(addr) & 0xf3) | ((data &  3) << 2); break;
    case 2: data = (bwram.read(addr) & 0xcf) | ((data &  3) << 4); break;
    case 3: data = (bwram.read(addr) & 0x3f) | ((data &  3) << 6); break;
    }
  }

  bwram.write(addr, data);
}
