#include "../../base.h"

SDD1 *sdd1;

#include "sdd1emu.cpp"

void SDD1::init() {}

void SDD1::enable() {
  for(int i = 0x4800; i <= 0x4807; i++) {
    r_mem->set_mmio_mapper(i, this);
  }
}

void SDD1::power() {
  reset();
}

void SDD1::reset() {
  sdd1.index[0] = 0x000000;
  sdd1.index[1] = 0x100000;
  sdd1.index[2] = 0x200000;
  sdd1.index[3] = 0x300000;

  for(int i=0;i<8;i++) {
    sdd1.active[i] = false;
  }

  sdd1.dma_active = false;
}

uint32 SDD1::offset(uint32 addr) {
uint8 b = (addr >> 16) & 0xff;
  if(b <= 0xbf)return 0;

  b  -= 0xc0; //b = 0x00-0x3f
  b >>= 4;    //b = 0-3
  b  &= 3;    //bitmask

  return sdd1.index[b] + (addr & 0x0fffff);
}

uint8 SDD1::mmio_read(uint16 addr) {
  switch(addr) {
  //>>20 == 0x100000 == 1mb
  case 0x4804: return (sdd1.index[0] >> 20) & 7;
  case 0x4805: return (sdd1.index[1] >> 20) & 7;
  case 0x4806: return (sdd1.index[2] >> 20) & 7;
  case 0x4807: return (sdd1.index[3] >> 20) & 7;
  }

  return r_cpu->regs.mdr;
}

void SDD1::mmio_write(uint16 addr, uint8 data) {
  switch(addr) {
  case 0x4801:
    for(int i = 0; i < 8; i++) {
      sdd1.active[i] = !!(data & (1 << i));
    }
    break;
  //<<20 == 0x100000 == 1mb
  case 0x4804: sdd1.index[0] = (data & 7) << 20; break;
  case 0x4805: sdd1.index[1] = (data & 7) << 20; break;
  case 0x4806: sdd1.index[2] = (data & 7) << 20; break;
  case 0x4807: sdd1.index[3] = (data & 7) << 20; break;
  }
}

void SDD1::dma_begin(uint8 channel, uint32 addr, uint16 length) {
  if(sdd1.active[channel] == true) {
    sdd1.active[channel] = false;
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
  if(--sdd1.buffer_size == 0) {
    sdd1.dma_active = false;
  }

//sdd1.buffer[] is 65536 bytes, and sdd1.buffer_index
//is of type uint16, so no buffer overflow is possible
  return sdd1.buffer[sdd1.buffer_index++];
}

SDD1::SDD1() {}
