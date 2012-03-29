#include <gba/gba.hpp>

namespace GBA {

#include "registers.cpp"
CPU cpu;

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
    if(crash) {
      print(cpsr().t ? disassemble_thumb_instruction(pipeline.execute.address)
                       : disassemble_arm_instruction(pipeline.execute.address), "\n");
      print(disassemble_registers(), "\n");
      print("Executed: ", instructions, "\n");
      while(true) step(frequency);
    }

    processor.irqline = regs.ime && regs.irq_flag;
    exec();
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

  regs.ime = 0;
  regs.irq_enable = 0;
  regs.irq_flag = 0;

  bus.mmio[0x130] = this;
  bus.mmio[0x200] = this;
  bus.mmio[0x202] = this;
  bus.mmio[0x208] = this;
}

uint32 CPU::read(uint32 addr, uint32 size) {
  uint32 result = 0;

  switch(addr & 0x0ffffffc) {

  case 0x04000130:  //KEYINPUT
    for(unsigned n = 0; n < 10; n++) result |= (interface->inputPoll(n) == false) << n;
    if((result & 0xc0) == 0xc0) result &= ~0xc0;  //up+down cannot be pressed simultaneously
    if((result & 0x30) == 0x30) result &= ~0x30;  //left+right cannot be pressed simultaneously
    return result;

  case 0x04000200:  //IE
    return regs.irq_enable;

  case 0x04000202:  //IF
    return regs.irq_flag;

  case 0x04000208:  //IME
    return regs.ime;

  }

  return 0u;
}

void CPU::write(uint32 addr, uint32 size, uint32 word) {
  switch(addr & 0x0ffffffc) {

  case 0x04000200:  //IE
    regs.irq_enable = word;
    return;

  case 0x04000202:  //IF
    regs.irq_flag = regs.irq_flag & ~word;
    return;

  case 0x04000208:  //IME
    regs.ime = word & 1;
    return;

  }
}

CPU::CPU() {
  iwram.data = new uint8[iwram.size =  32 * 1024];
  ewram.data = new uint8[ewram.size = 256 * 1024];
}

}
