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

    processor.irqline = regs.ime && (regs.irq_enable & regs.irq_flag);

    if(regs.mode == Registers::Mode::Halt) {
      if((regs.irq_enable & regs.irq_flag) == 0) {
        step(1);
        continue;
      }
      regs.mode = Registers::Mode::Normal;
    }

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

  regs.dma[0].source  = regs.dma[1].source  = regs.dma[2].source  = regs.dma[3].source  = 0;
  regs.dma[0].target  = regs.dma[1].target  = regs.dma[2].target  = regs.dma[3].target  = 0;
  regs.dma[0].length  = regs.dma[1].length  = regs.dma[2].length  = regs.dma[3].length  = 0;
  regs.dma[0].control = regs.dma[1].control = regs.dma[2].control = regs.dma[3].control = 0;

  regs.ime = 0;
  regs.irq_enable = 0;
  regs.irq_flag = 0;
  regs.postboot = 0;
  regs.mode = Registers::Mode::Normal;

  for(unsigned n = 0x0b0; n <= 0x0df; n++) bus.mmio[n] = this;

  bus.mmio[0x130] = bus.mmio[0x131] = this;
  bus.mmio[0x200] = bus.mmio[0x201] = this;
  bus.mmio[0x202] = bus.mmio[0x203] = this;
  bus.mmio[0x208] = bus.mmio[0x209] = this;
  bus.mmio[0x300] = bus.mmio[0x301] = this;
}

uint32 CPU::read(uint32 addr, uint32 size) {
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

  uint8 result = 0;

  switch(addr & 0x0fffffff) {

  //KEYINPUT
  case 0x04000130:
    for(unsigned n = 0; n < 8; n++) result |= interface->inputPoll(n) << n;
    if((result & 0xc0) == 0xc0) result &= ~0xc0;  //up+down cannot be pressed simultaneously
    if((result & 0x30) == 0x30) result &= ~0x30;  //left+right cannot be pressed simultaneously
    return result ^ 0xff;
  case 0x40000131:
    result |= interface->inputPoll(8) << 0;
    result |= interface->inputPoll(9) << 1;
    return result ^ 0x03;

  //IE
  case 0x04000200: return regs.irq_enable >> 0;
  case 0x04000201: return regs.irq_enable >> 8;

  //IF
  case 0x04000202: return regs.irq_flag >> 0;
  case 0x04000203: return regs.irq_flag >> 8;

  //IME
  case 0x04000208: return regs.ime;
  case 0x04000209: return 0u;

  //POSTFLG + HALTCNT
  case 0x04000300: return regs.postboot;
  case 0x04000301: return 0u;

  }

  return 0u;
}

void CPU::write(uint32 addr, uint32 size, uint32 word) {
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

  switch(addr & 0x0fffffff) {

  //DMA0SAD
  case 0x040000b0: regs.dma[0].source = (regs.dma[0].source & 0xffffff00) | (byte <<  0); return;
  case 0x040000b1: regs.dma[0].source = (regs.dma[0].source & 0xffff00ff) | (byte <<  8); return;
  case 0x040000b2: regs.dma[0].source = (regs.dma[0].source & 0xff00ffff) | (byte << 16); return;
  case 0x040000b3: regs.dma[0].source = (regs.dma[0].source & 0x00ffffff) | (byte << 24); return;

  //DMA0DAD
  case 0x040000b4: regs.dma[0].target = (regs.dma[0].target & 0xffffff00) | (byte <<  0); return;
  case 0x040000b5: regs.dma[0].target = (regs.dma[0].target & 0xffff00ff) | (byte <<  8); return;
  case 0x040000b6: regs.dma[0].target = (regs.dma[0].target & 0xff00ffff) | (byte << 16); return;
  case 0x040000b7: regs.dma[0].target = (regs.dma[0].target & 0x00ffffff) | (byte << 24); return;

  //DMA0CNT_L
  case 0x040000b8: regs.dma[0].length = (regs.dma[0].length & 0xff00) | (byte << 0); return;
  case 0x040000b9: regs.dma[0].length = (regs.dma[0].length & 0x00ff) | (byte << 8); return;

  //DMA0CNT_H
  case 0x040000ba: regs.dma[0].control = (regs.dma[0].control & 0xff00) | (byte << 0); return;
  case 0x040000bb: regs.dma[0].control = (regs.dma[0].control & 0x00ff) | (byte << 8); return;

  //DMA1SAD
  case 0x040000bc: regs.dma[1].source = (regs.dma[1].source & 0xffffff00) | (byte <<  0); return;
  case 0x040000bd: regs.dma[1].source = (regs.dma[1].source & 0xffff00ff) | (byte <<  8); return;
  case 0x040000be: regs.dma[1].source = (regs.dma[1].source & 0xff00ffff) | (byte << 16); return;
  case 0x040000bf: regs.dma[1].source = (regs.dma[1].source & 0x00ffffff) | (byte << 24); return;

  //DMA1DAD
  case 0x040000c0: regs.dma[1].target = (regs.dma[1].target & 0xffffff00) | (byte <<  0); return;
  case 0x040000c1: regs.dma[1].target = (regs.dma[1].target & 0xffff00ff) | (byte <<  8); return;
  case 0x040000c2: regs.dma[1].target = (regs.dma[1].target & 0xff00ffff) | (byte << 16); return;
  case 0x040000c3: regs.dma[1].target = (regs.dma[1].target & 0x00ffffff) | (byte << 24); return;

  //DMA1CNT_L
  case 0x040000c4: regs.dma[1].length = (regs.dma[1].length & 0xff00) | (byte << 0); return;
  case 0x040000c5: regs.dma[1].length = (regs.dma[1].length & 0x00ff) | (byte << 8); return;

  //DMA1CNT_H
  case 0x040000c6: regs.dma[1].control = (regs.dma[1].control & 0xff00) | (byte << 0); return;
  case 0x040000c7: regs.dma[1].control = (regs.dma[1].control & 0x00ff) | (byte << 8); return;

  //DMA2SAD
  case 0x040000c8: regs.dma[2].source = (regs.dma[2].source & 0xffffff00) | (byte <<  0); return;
  case 0x040000c9: regs.dma[2].source = (regs.dma[2].source & 0xffff00ff) | (byte <<  8); return;
  case 0x040000ca: regs.dma[2].source = (regs.dma[2].source & 0xff00ffff) | (byte << 16); return;
  case 0x040000cb: regs.dma[2].source = (regs.dma[2].source & 0x00ffffff) | (byte << 24); return;

  //DMA2DAD
  case 0x040000cc: regs.dma[2].target = (regs.dma[2].target & 0xffffff00) | (byte <<  0); return;
  case 0x040000cd: regs.dma[2].target = (regs.dma[2].target & 0xffff00ff) | (byte <<  8); return;
  case 0x040000ce: regs.dma[2].target = (regs.dma[2].target & 0xff00ffff) | (byte << 16); return;
  case 0x040000cf: regs.dma[2].target = (regs.dma[2].target & 0x00ffffff) | (byte << 24); return;

  //DMA2CNT_L
  case 0x040000d0: regs.dma[2].length = (regs.dma[2].length & 0xff00) | (byte << 0); return;
  case 0x040000d1: regs.dma[2].length = (regs.dma[2].length & 0x00ff) | (byte << 8); return;

  //DMA2CNT_H
  case 0x040000d2: regs.dma[2].control = (regs.dma[2].control & 0xff00) | (byte << 0); return;
  case 0x040000d3: regs.dma[2].control = (regs.dma[2].control & 0x00ff) | (byte << 8); return;

  //DMA3SAD
  case 0x040000d4: regs.dma[3].source = (regs.dma[3].source & 0xffffff00) | (byte <<  0); return;
  case 0x040000d5: regs.dma[3].source = (regs.dma[3].source & 0xffff00ff) | (byte <<  8); return;
  case 0x040000d6: regs.dma[3].source = (regs.dma[3].source & 0xff00ffff) | (byte << 16); return;
  case 0x040000d7: regs.dma[3].source = (regs.dma[3].source & 0x00ffffff) | (byte << 24); return;

  //DMA3DAD
  case 0x040000d8: regs.dma[3].target = (regs.dma[3].target & 0xffffff00) | (byte <<  0); return;
  case 0x040000d9: regs.dma[3].target = (regs.dma[3].target & 0xffff00ff) | (byte <<  8); return;
  case 0x040000da: regs.dma[3].target = (regs.dma[3].target & 0xff00ffff) | (byte << 16); return;
  case 0x040000db: regs.dma[3].target = (regs.dma[3].target & 0x00ffffff) | (byte << 24); return;

  //DMA3CNT_L
  case 0x040000dc: regs.dma[3].length = (regs.dma[3].length & 0xff00) | (byte << 0); return;
  case 0x040000dd: regs.dma[3].length = (regs.dma[3].length & 0x00ff) | (byte << 8); return;

  //DMA3CNT_H
  case 0x040000de: regs.dma[3].control = (regs.dma[3].control & 0xff00) | (byte << 0); return;
  case 0x040000df: regs.dma[3].control = (regs.dma[3].control & 0x00ff) | (byte << 8); return;

  //IE
  case 0x04000200: regs.irq_enable = (regs.irq_enable & 0xff00) | (byte << 0); return;
  case 0x04000201: regs.irq_enable = (regs.irq_enable & 0x00ff) | (byte << 8); return;

  //IF
  case 0x04000202: regs.irq_flag = regs.irq_flag & ~(byte << 0); return;
  case 0x04000203: regs.irq_flag = regs.irq_flag & ~(byte << 8); return;

  //IME
  case 0x04000208: regs.ime = byte & 1; return;
  case 0x04000209: return;

  //POSTFLG + HALTCNT
  case 0x04000300: regs.postboot = byte & 1; return;
  case 0x04000301: regs.mode = byte & 0x80 ? Registers::Mode::Stop : Registers::Mode::Halt; return;

  }
}

CPU::CPU() {
  iwram.data = new uint8[iwram.size =  32 * 1024];
  ewram.data = new uint8[ewram.size = 256 * 1024];
}

}
