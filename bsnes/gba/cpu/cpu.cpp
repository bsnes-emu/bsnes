#include <gba/gba.hpp>

namespace GBA {

#include "core/core.cpp"
CPU cpu;

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
    step(2);
  }
}

void CPU::step(unsigned clocks) {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

void CPU::power() {
  create(CPU::Enter, 16777216);

  ARM::power();
  for(unsigned n = 0; n < iram.size; n++) iram.data[n] = 0;
  for(unsigned n = 0; n < eram.size; n++) eram.data[n] = 0;
}

CPU::CPU() {
  iram.data = new uint8[iram.size =  32 * 1024];
  eram.data = new uint8[eram.size = 256 * 1024];
}

}
