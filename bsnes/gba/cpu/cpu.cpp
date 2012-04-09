#include <gba/gba.hpp>

namespace GBA {

#include "registers.cpp"
#include "mmio.cpp"
#include "dma.cpp"
#include "timer.cpp"
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

    processor.irqline = regs.ime && (regs.irq.enable & regs.irq.flag);
    dma_run();

    if(regs.mode == Registers::Mode::Halt) {
      if((regs.irq.enable & regs.irq.flag) == 0) {
        step(1);
        continue;
      }
      regs.mode = Registers::Mode::Normal;
    }

    exec();
  }
}

void CPU::step(unsigned clocks) {
  for(unsigned n = 0; n < clocks; n++) timer_tick();

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

  for(auto &dma : regs.dma) {
    dma.source = 0;
    dma.target = 0;
    dma.length = 0;
    dma.control = 0;
    dma.active = 0;
  }
  for(auto &timer : regs.timer) {
    timer.counter = 0;
    timer.reload = 0;
    timer.control = 0;
  }
  regs.keypad.control = 0;
  regs.ime = 0;
  regs.irq.enable = 0;
  regs.irq.flag = 0;
  regs.wait.control = 0;
  regs.postboot = 0;
  regs.mode = Registers::Mode::Normal;
  regs.clock = 0;
  regs.memory.control = 0x0d000020;

  pending.dma.vblank = 0;
  pending.dma.hblank = 0;
  pending.dma.hdma = 0;

  for(unsigned n = 0x0b0; n <= 0x0df; n++) bus.mmio[n] = this;  //DMA
  for(unsigned n = 0x100; n <= 0x10f; n++) bus.mmio[n] = this;  //Timers
  for(unsigned n = 0x130; n <= 0x133; n++) bus.mmio[n] = this;  //Keypad
  for(unsigned n = 0x200; n <= 0x209; n++) bus.mmio[n] = this;  //System
  for(unsigned n = 0x300; n <= 0x301; n++) bus.mmio[n] = this;  //System
  //0x080-0x083 mirrored via gba/memory/memory.cpp              //System
}

CPU::CPU() {
  iwram.data = new uint8[iwram.size =  32 * 1024];
  ewram.data = new uint8[ewram.size = 256 * 1024];
}

}
