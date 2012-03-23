#include <gba/gba.hpp>

namespace GBA {

CPU cpu;

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
    if(exception) {
      print(disassemble_registers(), "\n");
      while(true) step(frequency);
    }
    cpsr().t ? thumb_step() : arm_step();
  }
}

void CPU::step(unsigned clocks) {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

uint32 CPU::bus_read(uint32 addr, uint32 size) {
  step(1);
  return bus.read(addr, size);
}

void CPU::bus_write(uint32 addr, uint32 size, uint32 word) {
  step(1);
  return bus.write(addr, size, word);
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
