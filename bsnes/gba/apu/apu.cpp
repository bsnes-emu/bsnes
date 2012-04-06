#include <gba/gba.hpp>

namespace GBA {

#include "registers.cpp"
#include "mmio.cpp"
#include "square.cpp"
#include "square1.cpp"
#include "square2.cpp"
#include "wave.cpp"
#include "noise.cpp"
#include "sequencer.cpp"
APU apu;

void APU::Enter() { apu.enter(); }

void APU::enter() {
  while(true) {
    runsequencer();
    interface->audioSample(sequencer.lsample, sequencer.rsample);
    step(4);
  }
}

void APU::step(unsigned clocks) {
  clock += clocks;
  if(clock >= 0) co_switch(cpu.thread);
}

void APU::power() {
  create(APU::Enter, 16777216);

  square1.power();
  square2.power();
  wave.power();
  noise.power();
  sequencer.power();

  regs.bias = 0x0200;

  for(unsigned n = 0x060; n <= 0x0a7; n++) bus.mmio[n] = this;
}

}
