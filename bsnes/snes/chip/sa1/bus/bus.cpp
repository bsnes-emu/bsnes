#ifdef SA1_CPP

VBRBus vbrbus;
SA1Bus sa1bus;

namespace memory {
  StaticRAM iram(2048);
                        //accessed by:
  CPUIRAM cpuiram;      //S-CPU
  SA1IRAM sa1iram;      //SA-1
  SA1BWRAM sa1bwram;    //SA-1
  CC1BWRAM cc1bwram;    //S-CPU
  BitmapRAM bitmapram;  //SA-1
}

//$230c (VDPL), $230d (VDPH) use this bus to read variable-length data.
//this is used both to keep VBR-reads from accessing MMIO registers, and
//to avoid syncing the S-CPU and SA-1*; as both chips are able to access
//these ports.
//(* eg, memory::cartram is used directly, as memory::sa1bwram syncs to the S-CPU)
void VBRBus::init() {
  map(MapMode::Direct, 0x00, 0x3f, 0x8000, 0xffff, { &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });
  map(MapMode::Direct, 0x80, 0xbf, 0x8000, 0xffff, { &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });
  map(MapMode::Direct, 0xc0, 0xff, 0x0000, 0xffff, { &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });

  map(MapMode::Linear, 0x00, 0x3f, 0x6000, 0x7fff, { &MappedRAM::read, &memory::cartram }, { &MappedRAM::write, &memory::cartram }, 0, memory::cartram.size());
  map(MapMode::Linear, 0x80, 0xbf, 0x6000, 0x7fff, { &MappedRAM::read, &memory::cartram }, { &MappedRAM::write, &memory::cartram }, 0, memory::cartram.size());
  map(MapMode::Linear, 0x40, 0x4f, 0x0000, 0xffff, { &MappedRAM::read, &memory::cartram }, { &MappedRAM::write, &memory::cartram }, 0, memory::cartram.size());

  map(MapMode::Linear, 0x00, 0x3f, 0x0000, 0x07ff, { &StaticRAM::read, &memory::iram }, { &StaticRAM::write, &memory::iram }, 0, 2048);
  map(MapMode::Linear, 0x00, 0x3f, 0x3000, 0x37ff, { &StaticRAM::read, &memory::iram }, { &StaticRAM::write, &memory::iram }, 0, 2048);
  map(MapMode::Linear, 0x80, 0xbf, 0x0000, 0x07ff, { &StaticRAM::read, &memory::iram }, { &StaticRAM::write, &memory::iram }, 0, 2048);
  map(MapMode::Linear, 0x80, 0xbf, 0x3000, 0x37ff, { &StaticRAM::read, &memory::iram }, { &StaticRAM::write, &memory::iram }, 0, 2048);
}

void SA1Bus::init() {
  map(MapMode::Direct, 0x00, 0x3f, 0x2200, 0x23ff, { &SA1::mmio_read, &sa1 }, { &SA1::mmio_write, &sa1 });
  map(MapMode::Direct, 0x80, 0xbf, 0x2200, 0x23ff, { &SA1::mmio_read, &sa1 }, { &SA1::mmio_write, &sa1 });

  map(MapMode::Direct, 0x00, 0x3f, 0x8000, 0xffff, { &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });
  map(MapMode::Direct, 0x80, 0xbf, 0x8000, 0xffff, { &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });
  map(MapMode::Direct, 0xc0, 0xff, 0x0000, 0xffff, { &SA1::mmc_read, &sa1 }, { &SA1::mmc_write, &sa1 });

  map(MapMode::Linear, 0x00, 0x3f, 0x6000, 0x7fff, { &SA1::mmc_sa1_read, &sa1 }, { &SA1::mmc_sa1_write, &sa1 });
  map(MapMode::Linear, 0x80, 0xbf, 0x6000, 0x7fff, { &SA1::mmc_sa1_read, &sa1 }, { &SA1::mmc_sa1_write, &sa1 });

  map(MapMode::Linear, 0x00, 0x3f, 0x0000, 0x07ff, { &SA1IRAM::read, &memory::sa1iram }, { &SA1IRAM::write, &memory::sa1iram }, 0, 2048);
  map(MapMode::Linear, 0x00, 0x3f, 0x3000, 0x37ff, { &SA1IRAM::read, &memory::sa1iram }, { &SA1IRAM::write, &memory::sa1iram }, 0, 2048);
  map(MapMode::Linear, 0x80, 0xbf, 0x0000, 0x07ff, { &SA1IRAM::read, &memory::sa1iram }, { &SA1IRAM::write, &memory::sa1iram }, 0, 2048);
  map(MapMode::Linear, 0x80, 0xbf, 0x3000, 0x37ff, { &SA1IRAM::read, &memory::sa1iram }, { &SA1IRAM::write, &memory::sa1iram }, 0, 2048);

  map(MapMode::Linear, 0x40, 0x4f, 0x0000, 0xffff, { &SA1BWRAM::read, &memory::sa1bwram }, { &SA1BWRAM::write, &memory::sa1bwram }, 0, memory::sa1bwram.size());
  map(MapMode::Linear, 0x60, 0x6f, 0x0000, 0xffff, { &BitmapRAM::read, &memory::bitmapram }, { &BitmapRAM::write, &memory::bitmapram }, 0, memory::bitmapram.size());
}

//=======
//SA1IRAM
//=======

unsigned SA1IRAM::size() const {
  return memory::iram.size();
}

uint8 SA1IRAM::read(unsigned addr) {
  sa1.synchronize_cpu();
  return memory::iram.read(addr);
}

void SA1IRAM::write(unsigned addr, uint8 data) {
  sa1.synchronize_cpu();
  memory::iram.write(addr, data);
}

//=======
//CPUIRAM
//=======

unsigned CPUIRAM::size() const {
  return memory::iram.size();
}

uint8 CPUIRAM::read(unsigned addr) {
  cpu.synchronize_coprocessor();
  return memory::iram.read(addr);
}

void CPUIRAM::write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessor();
  memory::iram.write(addr, data);
}

//========
//SA1BWRAM
//========

unsigned SA1BWRAM::size() const {
  return memory::cartram.size();
}

uint8 SA1BWRAM::read(unsigned addr) {
  sa1.synchronize_cpu();
  return memory::cartram.read(addr);
}

void SA1BWRAM::write(unsigned addr, uint8 data) {
  sa1.synchronize_cpu();
  memory::cartram.write(addr, data);
}

//========
//CC1BWRAM
//========

unsigned CC1BWRAM::size() const {
  return memory::cartram.size();
}

uint8 CC1BWRAM::read(unsigned addr) {
  cpu.synchronize_coprocessor();
  if(dma) return sa1.dma_cc1_read(addr);
  return memory::cartram.read(addr);
}

void CC1BWRAM::write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessor();
  memory::cartram.write(addr, data);
}

//=========
//BitmapRAM
//=========

unsigned BitmapRAM::size() const {
  return 0x100000;
}

uint8 BitmapRAM::read(unsigned addr) {
  sa1.synchronize_cpu();

  if(sa1.mmio.bbf == 0) {
    //4bpp
    unsigned shift = addr & 1;
    addr = (addr >> 1) & (memory::cartram.size() - 1);
    switch(shift) { default:
      case 0: return (memory::cartram.read(addr) >> 0) & 15;
      case 1: return (memory::cartram.read(addr) >> 4) & 15;
    }
  } else {
    //2bpp
    unsigned shift = addr & 3;
    addr = (addr >> 2) & (memory::cartram.size() - 1);
    switch(shift) { default:
      case 0: return (memory::cartram.read(addr) >> 0) & 3;
      case 1: return (memory::cartram.read(addr) >> 2) & 3;
      case 2: return (memory::cartram.read(addr) >> 4) & 3;
      case 3: return (memory::cartram.read(addr) >> 6) & 3;
    }
  }
}

void BitmapRAM::write(unsigned addr, uint8 data) {
  sa1.synchronize_cpu();

  if(sa1.mmio.bbf == 0) {
    //4bpp
    unsigned shift = addr & 1;
    addr = (addr >> 1) & (memory::cartram.size() - 1);
    switch(shift) { default:
      case 0: data = (memory::cartram.read(addr) & 0xf0) | ((data & 15) << 0); break;
      case 1: data = (memory::cartram.read(addr) & 0x0f) | ((data & 15) << 4); break;
    }
  } else {
    //2bpp
    unsigned shift = addr & 3;
    addr = (addr >> 2) & (memory::cartram.size() - 1);
    switch(shift) { default:
      case 0: data = (memory::cartram.read(addr) & 0xfc) | ((data &  3) << 0); break;
      case 1: data = (memory::cartram.read(addr) & 0xf3) | ((data &  3) << 2); break;
      case 2: data = (memory::cartram.read(addr) & 0xcf) | ((data &  3) << 4); break;
      case 3: data = (memory::cartram.read(addr) & 0x3f) | ((data &  3) << 6); break;
    }
  }

  memory::cartram.write(addr, data);
}

#endif
