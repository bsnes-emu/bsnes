#include "../../base.h"
#define SDD1_CPP

#include "sdd1emu.cpp"

void SDD1::init() {}

void SDD1::enable() {
  for(int i = 0x4800; i <= 0x4807; i++) memory::mmio.map(i, *this);
}

void SDD1::power() {
  reset();
}

void SDD1::reset() {
  sdd1.dma_active = false;

  regs.r4800 = 0x00;
  regs.r4801 = 0x00;

  regs.r4804 = 0x00;
  regs.r4805 = 0x01;
  regs.r4806 = 0x02;
  regs.r4807 = 0x03;

  bus.map(Bus::MapLinear, 0xc0, 0xcf, 0x0000, 0xffff, memory::cartrom, (regs.r4804 & 7) << 20);
  bus.map(Bus::MapLinear, 0xd0, 0xdf, 0x0000, 0xffff, memory::cartrom, (regs.r4805 & 7) << 20);
  bus.map(Bus::MapLinear, 0xe0, 0xef, 0x0000, 0xffff, memory::cartrom, (regs.r4806 & 7) << 20);
  bus.map(Bus::MapLinear, 0xf0, 0xff, 0x0000, 0xffff, memory::cartrom, (regs.r4807 & 7) << 20);
}

uint8 SDD1::mmio_read(uint addr) {
  switch(addr & 0xffff) {
    case 0x4804: return regs.r4804;
    case 0x4805: return regs.r4805;
    case 0x4806: return regs.r4806;
    case 0x4807: return regs.r4807;
  }

  return cpu.regs.mdr;
}

void SDD1::mmio_write(uint addr, uint8 data) {
  switch(addr & 0xffff) {
    case 0x4800: {
      regs.r4800 = data;
    } break;

    case 0x4801: {
      regs.r4801 = data;
    } break;

    case 0x4804: {
      if(regs.r4804 != data) {
        regs.r4804 = data;
        bus.map(Bus::MapLinear, 0xc0, 0xcf, 0x0000, 0xffff,
          memory::cartrom, (regs.r4804 & 7) << 20);
      }
    } break;

    case 0x4805: {
      if(regs.r4805 != data) {
        regs.r4805 = data;
        bus.map(Bus::MapLinear, 0xd0, 0xdf, 0x0000, 0xffff,
          memory::cartrom, (regs.r4805 & 7) << 20);
      }
    } break;

    case 0x4806: {
      if(regs.r4806 != data) {
        regs.r4806 = data;
        bus.map(Bus::MapLinear, 0xe0, 0xef, 0x0000, 0xffff,
          memory::cartrom, (regs.r4806 & 7) << 20);
      }
    } break;

    case 0x4807: {
      if(regs.r4807 != data) {
        regs.r4807 = data;
        bus.map(Bus::MapLinear, 0xf0, 0xff, 0x0000, 0xffff,
          memory::cartrom, (regs.r4807 & 7) << 20);
      }
    } break;
  }
}

void SDD1::dma_begin(uint8 channel, uint32 addr, uint16 length) {
  if(regs.r4800 & (1 << channel) && regs.r4801 & (1 << channel)) {
    regs.r4801 &= ~(1 << channel);
    sdd1.dma_active   = true;
    sdd1.buffer_index = 0;
    sdd1.buffer_size  = length;
    sdd1emu.decompress(addr, (length) ? length : 65536, sdd1.buffer);
  }
}

bool SDD1::dma_active() {
  return sdd1.dma_active;
}

uint8 SDD1::dma_read() {
  if(--sdd1.buffer_size == 0) sdd1.dma_active = false;

//sdd1.buffer[] is 65536 bytes, and sdd1.buffer_index
//is of type uint16, so no buffer overflow is possible
  return sdd1.buffer[sdd1.buffer_index++];
}

SDD1::SDD1() {}
