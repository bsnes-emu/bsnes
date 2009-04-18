#ifdef SA1_CPP

namespace memory {
  namespace cpu {
    CPUIRAM iram;
    CPUBWRAM bwram;
  }

  namespace sa1 {
    SA1IRAM iram;
    SA1BWRAM bwram;
    SA1BitmapRAM bitmapram;
  }
}

void SA1Bus::init() {
  for(uint32_t i = 0x0000; i <= 0xffff; i++) {
    map(i << 8, memory::memory_unmapped, 0);
  }

  for(uint16_t i = 0x2200; i <= 0x23ff; i++) {
    memory::mmio.map(i, sa1);
  }

  map(MapLinear, 0x00, 0x3f, 0x0000, 0x07ff, memory::sa1::iram);
  map(MapDirect, 0x00, 0x3f, 0x2200, 0x23ff, memory::mmio);
  map(MapLinear, 0x00, 0x3f, 0x3000, 0x37ff, memory::sa1::iram);
  map(MapLinear, 0x00, 0x3f, 0x6000, 0x7fff, memory::sa1::bwram);
  map(MapLinear, 0x00, 0x3f, 0x8000, 0xffff, memory::cartrom);
  map(MapLinear, 0x40, 0x4f, 0x0000, 0xffff, memory::sa1::bwram, 0, 0x040000);
  map(MapLinear, 0x60, 0x6f, 0x0000, 0xffff, memory::sa1::bitmapram);
  map(MapLinear, 0x80, 0xbf, 0x0000, 0x07ff, memory::sa1::iram);
  map(MapDirect, 0x80, 0xbf, 0x2200, 0x23ff, memory::mmio);
  map(MapLinear, 0x80, 0xbf, 0x3000, 0x37ff, memory::sa1::iram);
  map(MapLinear, 0x80, 0xbf, 0x6000, 0x7fff, memory::sa1::bwram);
  map(MapLinear, 0x80, 0xbf, 0x8000, 0xffff, memory::cartrom);
  map(MapLinear, 0xc0, 0xff, 0x0000, 0xffff, memory::cartrom);

  bus.map(MapLinear, 0x00, 0x3f, 0x3000, 0x37ff, memory::cpu::iram);
  bus.map(MapLinear, 0x00, 0x3f, 0x6000, 0x7fff, memory::cpu::bwram);
  bus.map(MapLinear, 0x00, 0x3f, 0x8000, 0xffff, memory::cartrom);
  bus.map(MapLinear, 0x40, 0x4f, 0x0000, 0xffff, memory::cpu::bwram, 0, 0x040000);
  bus.map(MapLinear, 0x80, 0xbf, 0x3000, 0x37ff, memory::cpu::iram);
  bus.map(MapLinear, 0x80, 0xbf, 0x6000, 0x7fff, memory::cpu::bwram);
  bus.map(MapLinear, 0x80, 0xbf, 0x8000, 0xffff, memory::cartrom);
  bus.map(MapLinear, 0xc0, 0xff, 0x0000, 0xffff, memory::cartrom);
}

//=======
//CPUIRAM
//=======

unsigned CPUIRAM::size() const {
  return sizeof(sa1.iram);
}

uint8_t CPUIRAM::read(unsigned addr) {
  return sa1.iram[addr];
}

void CPUIRAM::write(unsigned addr, uint8_t data) {
  uint8_t wpbit = (addr >> 8) & 7;
  if(1 || sa1.mmio.siwp & wpbit) {
    //allow only when write-protection is disabled
    sa1.iram[addr] = data;
  }
}

//========
//CPUBWRAM
//========

unsigned CPUBWRAM::size() const {
  return memory::cartram.size();
}

uint8_t CPUBWRAM::read(unsigned addr) {
  if(cc1dma) return sa1.dma_cc1_read(addr);
  return memory::cartram.read(addr);
}

void CPUBWRAM::write(unsigned addr, uint8_t data) {
  if(sa1.mmio.swen == false) {
    //write-protection enabled
    unsigned limit = 0x100 << sa1.mmio.bwp;
    //if(addr < limit) return;
  }

  memory::cartram.write(addr, data);
}

//=======
//SA1IRAM
//=======

unsigned SA1IRAM::size() const {
  return sizeof(sa1.iram);
}

uint8_t SA1IRAM::read(unsigned addr) {
  return sa1.iram[addr];
}

void SA1IRAM::write(unsigned addr, uint8_t data) {
  uint8_t wpbit = (addr >> 8) & 7;
  if(1 || sa1.mmio.ciwp & wpbit) {
    //allow only when write-protection is disabled
    sa1.iram[addr] = data;
  }
}

//========
//SA1BWRAM
//========

unsigned SA1BWRAM::size() const {
  return memory::cartram.size();
}

uint8_t SA1BWRAM::read(unsigned addr) {
  return memory::cartram.read(addr);
}

void SA1BWRAM::write(unsigned addr, uint8_t data) {
  if(sa1.mmio.cwen == false) {
    //write-protection enabled
    unsigned limit = 0x100 << sa1.mmio.bwp;
    //if(addr < limit) return;
  }

  memory::cartram.write(addr, data);
}

//============
//SA1BitmapRAM
//============

unsigned SA1BitmapRAM::size() const {
  return 0x100000;
}

uint8_t SA1BitmapRAM::read(unsigned addr) {
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

void SA1BitmapRAM::write(unsigned addr, uint8_t data) {
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
