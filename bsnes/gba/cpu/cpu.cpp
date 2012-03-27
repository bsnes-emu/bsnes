#include <gba/gba.hpp>

namespace GBA {

CPU cpu;

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
    if(exception) {
      print(cpsr().t ? disassemble_thumb_instruction(pipeline.execute.address)
                       : disassemble_arm_instruction(pipeline.execute.address), "\n");
      print(disassemble_registers(), "\n");
      print("Executed: ", instructions, "\n");
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
  for(unsigned n = 0; n < iwram.size; n++) iwram.data[n] = 0;
  for(unsigned n = 0; n < ewram.size; n++) ewram.data[n] = 0;

  bus.mmio[0x0300] = this;
  bus.mmio[0x0301] = this;
  bus.mmio[0x0302] = this;
  bus.mmio[0x0303] = this;
}

uint32 CPU::read(uint32 addr, uint32 size) {
  return 0u;
}

void CPU::write(uint32 addr, uint32 size, uint32 word) {
}

CPU::CPU() {
  iwram.data = new uint8[iwram.size =  32 * 1024];
  ewram.data = new uint8[ewram.size = 256 * 1024];
}

}
