#ifdef SA1_CPP

namespace memory {
  StaticRAM iram(2048);
                        //accessed by:
  CPUIRAM cpuiram;      //S-CPU
  SA1IRAM sa1iram;      //SA-1
  SA1BWRAM sa1bwram;    //SA-1
  CC1BWRAM cc1bwram;    //S-CPU
  BitmapRAM bitmapram;  //SA-1
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
