#include <gba/gba.hpp>

#define CPU_CPP
namespace GBA {

#include "core/core.cpp"
CPU cpu;

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
  //scheduler.synchronize_cpu();

    step(4);
  }
}

void CPU::step(unsigned clocks) {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

//apu.clock -= clocks;
//if(apu.clock < 0) co_switch(apu.thread);
}

void CPU::power() {
  create(CPU::Enter, 16777216);
  ARM7TDMI::power();
}

}
