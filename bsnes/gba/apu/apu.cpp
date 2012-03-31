#include <gba/gba.hpp>

namespace GBA {

APU apu;

void APU::Enter() { apu.enter(); }

void APU::enter() {
  while(true) {
    interface->audioSample(0, 0);
    step(512);
  }
}

void APU::step(unsigned clocks) {
  clock += clocks;
  if(clock >= 0) co_switch(cpu.thread);
}

uint32 APU::read(uint32 addr, uint32 size) {
  if(size == Word) {
    addr &= ~3;
    uint32 word = 0;
    word |= read(addr + 0, Byte) <<  0;
    word |= read(addr + 1, Byte) <<  8;
    word |= read(addr + 2, Byte) << 16;
    word |= read(addr + 3, Byte) << 24;
    return word;
  }

  if(size == Half) {
    addr &= ~1;
    uint32 half = 0;
    half |= read(addr + 0, Byte) <<  0;
    half |= read(addr + 0, Byte) <<  8;
    return half;
  }

  switch(addr & 0x0f000000) {

  //SOUNDBIAS
  case 0x04000088: return 0x00;
  case 0x04000089: return 0x02;

  }

  return 0u;
}

void APU::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Word) {
    addr &= ~3;
    write(addr + 0, Byte, word >>  0);
    write(addr + 1, Byte, word >>  8);
    write(addr + 2, Byte, word >> 16);
    write(addr + 3, Byte, word >> 24);
    return;
  }

  if(size == Half) {
    addr &= ~1;
    write(addr + 0, Byte, word >>  0);
    write(addr + 1, Byte, word >>  8);
    return;
  }

  uint8 byte = word;
}

void APU::power() {
  create(APU::Enter, 16777216);

  bus.mmio[0x0088] = this;
}

}
