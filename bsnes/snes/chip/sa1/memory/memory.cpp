#ifdef SA1_CPP

uint8 SA1::bus_read(unsigned addr) {
  if((addr & 0x40fe00) == 0x002200) {  //$00-3f|80-bf:2200-23ff
    return mmio_read(addr);
  }

  if((addr & 0x408000) == 0x008000) {  //$00-3f|80-bf:8000-ffff
    return mmc_read(addr);
  }

  if((addr & 0xc00000) == 0xc00000) {  //$c0-ff:0000-ffff
    return mmc_read(addr);
  }

  if((addr & 0x40e000) == 0x006000) {  //$00-3f|80-bf:6000-7fff
    return mmc_sa1_read(addr);
  }

  if((addr & 0x40f800) == 0x000000) {  //$00-3f|80-bf:0000-07ff
    return memory::iram.read(addr & 2047);
  }

  if((addr & 0x40f800) == 0x003000) {  //$00-3f|80-bf:3000-37ff
    return memory::iram.read(addr & 2047);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    return memory::sa1bwram.read(addr & (memory::sa1bwram.size() - 1));
  }

  if((addr & 0xf00000) == 0x600000) {  //$60-6f:0000-ffff
    return memory::bitmapram.read(addr & (memory::bitmapram.size() - 1));
  }
}

void SA1::bus_write(unsigned addr, uint8 data) {
  if((addr & 0x40fe00) == 0x002200) {  //$00-3f|80-bf:2200-23ff
    return mmio_write(addr, data);
  }

  if((addr & 0x40e000) == 0x006000) {  //$00-3f|80-bf:6000-7fff
    return mmc_sa1_write(addr, data);
  }

  if((addr & 0x40f800) == 0x000000) {  //$00-3f|80-bf:0000-07ff
    return memory::iram.write(addr & 2047, data);
  }

  if((addr & 0x40f800) == 0x003000) {  //$00-3f|80-bf:3000-37ff
    return memory::iram.write(addr & 2047, data);
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    return memory::sa1bwram.write(addr & (memory::sa1bwram.size() - 1), data);
  }

  if((addr & 0xf00000) == 0x600000) {  //$60-6f:0000-ffff
    return memory::bitmapram.write(addr & (memory::bitmapram.size() - 1), data);
  }
}

//$230c (VDPL), $230d (VDPH) use this bus to read variable-length data.
//this is used both to keep VBR-reads from accessing MMIO registers, and
//to avoid syncing the S-CPU and SA-1*; as both chips are able to access
//these ports.
//(* eg, cartridge.ram is used directly, as memory::sa1bwram syncs to the S-CPU)
uint8 SA1::vbr_read(unsigned addr) {
  if((addr & 0x408000) == 0x008000) {  //$00-3f|80-bf:8000-ffff
    return mmc_read(addr);
  }

  if((addr & 0xc00000) == 0xc00000) {  //$c0-ff:0000-ffff
    return mmc_read(addr);
  }

  if((addr & 0x40e000) == 0x006000) {  //$00-3f|80-bf:6000-7fff
    return cartridge.ram.read(addr & (cartridge.ram.size() - 1));
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    return cartridge.ram.read(addr & (cartridge.ram.size() - 1));
  }

  if((addr & 0x40f800) == 0x000000) {  //$00-3f|80-bf:0000-07ff
    return memory::iram.read(addr & 2047);
  }

  if((addr & 0x40f800) == 0x003000) {  //$00-3f|80-bf:3000-37ff
    return memory::iram.read(addr & 0x2047);
  }
}

//ROM, I-RAM and MMIO registers are accessed at ~10.74MHz (2 clock ticks)
//BW-RAM is accessed at ~5.37MHz (4 clock ticks)
//tick() == 2 clock ticks
//note: bus conflict delays are not emulated at this time

void SA1::op_io() {
  tick();
}

uint8 SA1::op_read(unsigned addr) {
  tick();
  if(((addr & 0x40e000) == 0x006000) || ((addr & 0xd00000) == 0x400000)) tick();
  return bus_read(addr);
}

void SA1::op_write(unsigned addr, uint8 data) {
  tick();
  if(((addr & 0x40e000) == 0x006000) || ((addr & 0xd00000) == 0x400000)) tick();
  bus_write(addr, data);
}

uint8 SA1::mmc_read(unsigned addr) {
  if((addr & 0xffffe0) == 0x00ffe0) {
    if(addr == 0xffea && sa1.mmio.cpu_nvsw) return sa1.mmio.snv >> 0;
    if(addr == 0xffeb && sa1.mmio.cpu_nvsw) return sa1.mmio.snv >> 8;
    if(addr == 0xffee && sa1.mmio.cpu_ivsw) return sa1.mmio.siv >> 0;
    if(addr == 0xffef && sa1.mmio.cpu_ivsw) return sa1.mmio.siv >> 8;
  }

  static auto read = [](unsigned addr) {
    return cartridge.rom.read(bus.mirror(addr, cartridge.rom.size()));
  };

  if((addr & 0xe08000) == 0x008000) {  //$00-1f:8000-ffff
    addr = ((addr & 0x1f0000) >> 1) | (addr & 0x007fff);
    if(mmio.cbmode == 0) return read(0x000000 | addr);
    return read((mmio.cb << 20) | addr);
  }

  if((addr & 0xe08000) == 0x208000) {  //$20-3f:8000-ffff
    addr = ((addr & 0x1f0000) >> 1) | (addr & 0x007fff);
    if(mmio.dbmode == 0) return read(0x100000 | addr);
    return read((mmio.db << 20) | addr);
  }

  if((addr & 0xe08000) == 0x808000) {  //$80-9f:8000-ffff
    addr = ((addr & 0x1f0000) >> 1) | (addr & 0x007fff);
    if(mmio.ebmode == 0) return read(0x200000 | addr);
    return read((mmio.eb << 20) | addr);
  }

  if((addr & 0xe08000) == 0xa08000) {  //$a0-bf:8000-ffff
    addr = ((addr & 0x1f0000) >> 1) | (addr & 0x007fff);
    if(mmio.fbmode == 0) return read(0x300000 | addr);
    return read((mmio.fb << 20) | addr);
  }

  if((addr & 0xf00000) == 0xc00000) {  //$c0-cf:0000-ffff
    return read((mmio.cb << 20) | (addr & 0x0fffff));
  }

  if((addr & 0xf00000) == 0xd00000) {  //$d0-df:0000-ffff
    return read((mmio.db << 20) | (addr & 0x0fffff));
  }

  if((addr & 0xf00000) == 0xe00000) {  //$e0-ef:0000-ffff
    return read((mmio.eb << 20) | (addr & 0x0fffff));
  }

  if((addr & 0xf00000) == 0xf00000) {  //$f0-ff:0000-ffff
    return read((mmio.fb << 20) | (addr & 0x0fffff));
  }

  return 0x00;
}

void SA1::mmc_write(unsigned addr, uint8 data) {
}

uint8 SA1::mmc_cpu_read(unsigned addr) {
  cpu.synchronize_coprocessor();
  addr = bus.mirror(mmio.sbm * 0x2000 + (addr & 0x1fff), memory::cc1bwram.size());
  return memory::cc1bwram.read(addr);
}

void SA1::mmc_cpu_write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessor();
  addr = bus.mirror(mmio.sbm * 0x2000 + (addr & 0x1fff), memory::cc1bwram.size());
  memory::cc1bwram.write(addr, data);
}

uint8 SA1::mmc_sa1_read(unsigned addr) {
  synchronize_cpu();
  if(mmio.sw46 == 0) {
    //$40-43:0000-ffff x  32 projection
    addr = bus.mirror((mmio.cbm & 0x1f) * 0x2000 + (addr & 0x1fff), memory::sa1bwram.size());
    return memory::sa1bwram.read(addr);
  } else {
    //$60-6f:0000-ffff x 128 projection
    addr = bus.mirror(mmio.cbm * 0x2000 + (addr & 0x1fff), memory::bitmapram.size());
    return memory::bitmapram.read(addr);
  }
}

void SA1::mmc_sa1_write(unsigned addr, uint8 data) {
  synchronize_cpu();
  if(mmio.sw46 == 0) {
    //$40-43:0000-ffff x  32 projection
    addr = bus.mirror((mmio.cbm & 0x1f) * 0x2000 + (addr & 0x1fff), memory::sa1bwram.size());
    memory::sa1bwram.write(addr, data);
  } else {
    //$60-6f:0000-ffff x 128 projection
    addr = bus.mirror(mmio.cbm * 0x2000 + (addr & 0x1fff), memory::bitmapram.size());
    memory::bitmapram.write(addr, data);
  }
}

#endif
