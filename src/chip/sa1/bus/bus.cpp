#ifdef SA1_CPP

SA1Bus sa1bus;

namespace memory {
  VectorSelectionPage vectorsp;
  StaticRAM iram(2048);
  MappedRAM &bwram = memory::cartram;
  CC1BWRAM cc1bwram;
  BitmapRAM bitmapram;
}

void SA1Bus::init() {
  for(uint32_t i = 0x0000; i <= 0xffff; i++) {
    map(i << 8, memory::memory_unmapped, 0);
  }

  for(uint16_t i = 0x2200; i <= 0x23ff; i++) {
    memory::mmio.map(i, sa1);
  }

  map(MapLinear, 0x00, 0x3f, 0x0000, 0x07ff, memory::iram);
  map(MapDirect, 0x00, 0x3f, 0x2200, 0x23ff, memory::mmio);
  map(MapLinear, 0x00, 0x3f, 0x3000, 0x37ff, memory::iram);
  map(MapLinear, 0x00, 0x3f, 0x6000, 0x7fff, memory::bwram);
  map(MapLinear, 0x00, 0x3f, 0x8000, 0xffff, memory::cartrom);
  map(MapLinear, 0x40, 0x4f, 0x0000, 0xffff, memory::bwram);
  map(MapLinear, 0x60, 0x6f, 0x0000, 0xffff, memory::bitmapram);
  map(MapLinear, 0x80, 0xbf, 0x0000, 0x07ff, memory::iram);
  map(MapDirect, 0x80, 0xbf, 0x2200, 0x23ff, memory::mmio);
  map(MapLinear, 0x80, 0xbf, 0x3000, 0x37ff, memory::iram);
  map(MapLinear, 0x80, 0xbf, 0x6000, 0x7fff, memory::bwram);
  map(MapLinear, 0x80, 0xbf, 0x8000, 0xffff, memory::cartrom);
  map(MapLinear, 0xc0, 0xff, 0x0000, 0xffff, memory::cartrom);

  bus.map(MapLinear, 0x00, 0x3f, 0x3000, 0x37ff, memory::iram);
  bus.map(MapLinear, 0x00, 0x3f, 0x6000, 0x7fff, memory::cc1bwram);
  bus.map(MapLinear, 0x00, 0x3f, 0x8000, 0xffff, memory::cartrom);
  bus.map(MapLinear, 0x40, 0x4f, 0x0000, 0xffff, memory::cc1bwram);
  bus.map(MapLinear, 0x80, 0xbf, 0x3000, 0x37ff, memory::iram);
  bus.map(MapLinear, 0x80, 0xbf, 0x6000, 0x7fff, memory::cc1bwram);
  bus.map(MapLinear, 0x80, 0xbf, 0x8000, 0xffff, memory::cartrom);
  bus.map(MapLinear, 0xc0, 0xff, 0x0000, 0xffff, memory::cartrom);

  memory::vectorsp.sync();
}

//===================
//VectorSelectionPage
//===================

//this class maps $00:[ff00-ffff] for the purpose of supporting:
//$2209.d6 IVSW (S-CPU IRQ vector selection) (0 = cart, 1 = SA-1)
//$2209.d4 NVSW (S-CPU NMI vector selection) (0 = cart, 1 = SA-1)
//when set, vector addresses are over-ridden with SA-1 register settings:
//SIV = S-CPU IRQ vector address override
//SNV = S-CPU NMI vector address override
//
//$00:[ffea-ffeb|ffee-ffef] are special cased on read;
//all other addresses return original mapped data.

uint8_t VectorSelectionPage::read(unsigned addr) {
  switch(0xff00 | (addr & 0xff)) {
    case 0xffea: case 0xffeb: {
      if(sa1.mmio.cpu_nvsw == true) return (sa1.mmio.snv >> ((addr & 1) << 3));
    } break;

    case 0xffee: case 0xffef: {
      if(sa1.mmio.cpu_ivsw == true) return (sa1.mmio.siv >> ((addr & 1) << 3));
    } break;
  }

  return access->read(addr);
}

void VectorSelectionPage::write(unsigned addr, uint8_t data) {
  return access->write(addr, data);
}

//call this whenever bus is remapped.
//note: S-CPU and SA-1 bus always share $00:[ff00-ffff] as cartridge ROM data;
//the SA-1 MMC does not allow mapping these independently between processors.
//this allows this class to be shared for both, caching only ones' access class.
void VectorSelectionPage::sync() {
  if(bus.page[0x00ff00 >> 8].access != this) {
    //bus was re-mapped, hook access routine
    access = bus.page[0x00ff00 >> 8].access;
       bus.page[0x00ff00 >> 8].access = this;
    sa1bus.page[0x00ff00 >> 8].access = this;
  }
}

//========
//CC1BWRAM
//========

unsigned CC1BWRAM::size() const {
  return memory::cartram.size();
}

uint8_t CC1BWRAM::read(unsigned addr) {
  if(dma) return sa1.dma_cc1_read(addr);
  return memory::cartram.read(addr);
}

void CC1BWRAM::write(unsigned addr, uint8_t data) {
  memory::cartram.write(addr, data);
}

//=========
//BitmapRAM
//=========

unsigned BitmapRAM::size() const {
  return 0x100000;
}

uint8_t BitmapRAM::read(unsigned addr) {
  if(sa1.mmio.bbf == 0) {
    //4bpp
    unsigned shift = addr & 1;
    addr = (addr >> 1) & (memory::cartram.size() - 1);
    switch(shift) {
      case 0: return (memory::cartram.read(addr) >> 0) & 15;
      case 1: return (memory::cartram.read(addr) >> 4) & 15;
    }
  } else {
    //2bpp
    unsigned shift = addr & 3;
    addr = (addr >> 2) & (memory::cartram.size() - 1);
    switch(shift) {
      case 0: return (memory::cartram.read(addr) >> 0) & 3;
      case 1: return (memory::cartram.read(addr) >> 2) & 3;
      case 2: return (memory::cartram.read(addr) >> 4) & 3;
      case 3: return (memory::cartram.read(addr) >> 6) & 3;
    }
  }
}

void BitmapRAM::write(unsigned addr, uint8_t data) {
  if(sa1.mmio.bbf == 0) {
    //4bpp
    uint8_t shift = addr & 1;
    addr = (addr >> 1) & (memory::cartram.size() - 1);
    switch(shift) {
      case 0: data = (memory::cartram.read(addr) & 0xf0) | ((data & 15) << 0); break;
      case 1: data = (memory::cartram.read(addr) & 0x0f) | ((data & 15) << 4); break;
    }
  } else {
    //2bpp
    uint8_t shift = addr & 3;
    addr = (addr >> 2) & (memory::cartram.size() - 1);
    switch(shift) {
      case 0: data = (memory::cartram.read(addr) & 0xfc) | ((data &  3) << 0); break;
      case 1: data = (memory::cartram.read(addr) & 0xf3) | ((data &  3) << 2); break;
      case 2: data = (memory::cartram.read(addr) & 0xcf) | ((data &  3) << 4); break;
      case 3: data = (memory::cartram.read(addr) & 0x3f) | ((data &  3) << 6); break;
    }
  }

  memory::cartram.write(addr, data);
}

#endif
