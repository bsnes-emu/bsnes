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
  if(addr == 0x04000088) {
    //SOUNDBIAS
    return 0x0200;
  }

  return 0u;
}

void APU::write(uint32 addr, uint32 size, uint32 word) {
}

void APU::power() {
  create(APU::Enter, 16777216);

  bus.mmio[0x0088] = this;
}

}
