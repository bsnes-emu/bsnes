uint8 CPU::read(uint32 addr) {
  uint8 result = 0;

  switch(addr) {

  //DMA0CNT_H
  //DMA1CNT_H
  //DMA2CNT_H
  //DMA3CNT_H
  case 0x040000ba: case 0x040000bb:
  case 0x040000c6: case 0x040000c7:
  case 0x040000d2: case 0x040000d3:
  case 0x040000de: case 0x040000df: {
    auto &dma = regs.dma[(addr - 0x040000ba) / 12];
    unsigned shift = (addr & 1) * 8;
    return dma.control >> shift;
  }

  //TM0CNT_L
  //TM1CNT_L
  //TM2CNT_L
  //TM3CNT_L
  case 0x04000100: case 0x04000101:
  case 0x04000104: case 0x04000105:
  case 0x04000108: case 0x04000109:
  case 0x0400010c: case 0x0400010d: {
    auto &timer = regs.timer[(addr >> 2) & 3];
    unsigned shift = (addr & 1) * 8;
    return timer.counter >> shift;
  }

  //TIM0CNT_H
  case 0x04000102: case 0x04000103:
  case 0x04000106: case 0x04000107:
  case 0x0400010a: case 0x0400010b:
  case 0x0400010e: case 0x0400010f: {
    auto &timer = regs.timer[(addr >> 2) & 3];
    unsigned shift = (addr & 1) * 8;
    return timer.control >> shift;
  }

  //KEYINPUT
  case 0x04000130:
    for(unsigned n = 0; n < 8; n++) result |= interface->inputPoll(n) << n;
    if((result & 0xc0) == 0xc0) result &= ~0xc0;  //up+down cannot be pressed simultaneously
    if((result & 0x30) == 0x30) result &= ~0x30;  //left+right cannot be pressed simultaneously
    return result ^ 0xff;
  case 0x04000131:
    result |= interface->inputPoll(8) << 0;
    result |= interface->inputPoll(9) << 1;
    return result ^ 0x03;

  //KEYCNT
  case 0x04000132: return regs.keypad.control >> 0;
  case 0x04000133: return regs.keypad.control >> 8;

  //IE
  case 0x04000200: return regs.irq.enable >> 0;
  case 0x04000201: return regs.irq.enable >> 8;

  //IF
  case 0x04000202: return regs.irq.flag >> 0;
  case 0x04000203: return regs.irq.flag >> 8;

  //WAITCNT
  case 0x04000204: return regs.wait.control >> 0;
  case 0x04000205: return regs.wait.control >> 8;

  //IME
  case 0x04000208: return regs.ime;
  case 0x04000209: return 0u;

  //POSTFLG + HALTCNT
  case 0x04000300: return regs.postboot;
  case 0x04000301: return 0u;

  //MEMCNT_L
  case 0x04000800: return regs.memory.control >>  0;
  case 0x04000801: return regs.memory.control >>  8;

  //MEMCNT_H
  case 0x04000802: return regs.memory.control >> 16;
  case 0x04000803: return regs.memory.control >> 24;

  }

  return 0u;
}

void CPU::write(uint32 addr, uint8 byte) {
  switch(addr) {

  //DMA0SAD
  //DMA1SAD
  //DMA2SAD
  //DMA3SAD
  case 0x040000b0: case 0x040000b1: case 0x040000b2: case 0x040000b3:
  case 0x040000bc: case 0x040000bd: case 0x040000be: case 0x040000bf:
  case 0x040000c8: case 0x040000c9: case 0x040000ca: case 0x040000cb:
  case 0x040000d4: case 0x040000d5: case 0x040000d6: case 0x040000d7: {
    auto &dma = regs.dma[(addr - 0x040000b0) / 12];
    unsigned shift = (addr & 3) * 8;
    dma.source = (dma.source & ~(255 << shift)) | (byte << shift);
    return;
  }

  //DMA0DAD
  //DMA1DAD
  //DMA2DAD
  //DMA3DAD
  case 0x040000b4: case 0x040000b5: case 0x040000b6: case 0x040000b7:
  case 0x040000c0: case 0x040000c1: case 0x040000c2: case 0x040000c3:
  case 0x040000cc: case 0x040000cd: case 0x040000ce: case 0x040000cf:
  case 0x040000d8: case 0x040000d9: case 0x040000da: case 0x040000db: {
    auto &dma = regs.dma[(addr - 0x040000b4) / 12];
    unsigned shift = (addr & 3) * 8;
    dma.target = (dma.target & ~(255 << shift)) | (byte << shift);
    return;
  }

  //DMA0CNT_L
  //DMA1CNT_L
  //DMA2CNT_L
  //DMA3CNT_L
  case 0x040000b8: case 0x040000b9:
  case 0x040000c4: case 0x040000c5:
  case 0x040000d0: case 0x040000d1:
  case 0x040000dc: case 0x040000dd: {
    auto &dma = regs.dma[(addr - 0x040000b8) / 12];
    unsigned shift = (addr & 1) * 8;
    dma.length = (dma.length & ~(255 << shift)) | (byte << shift);
    return;
  }

  //DMA0CNT_H
  //DMA1CNT_H
  //DMA2CNT_H
  //DMA3CNT_H
  case 0x040000ba: case 0x040000bb:
  case 0x040000c6: case 0x040000c7:
  case 0x040000d2: case 0x040000d3:
  case 0x040000de: case 0x040000df: {
    auto &dma = regs.dma[(addr - 0x040000ba) / 12];
    unsigned shift = (addr & 1) * 8;
    bool enable = dma.control.enable;
    dma.control = (dma.control & ~(255 << shift)) | (byte << shift);
    if(enable == 0 && dma.control.enable) {
      dma.run.target = dma.target;
      dma.run.source = dma.source;
      dma.run.length = dma.length;
    }
    return;
  }

  //TM0CNT_L
  //TM1CNT_L
  //TM2CNT_L
  //TM3CNT_L
  case 0x04000100: case 0x04000101:
  case 0x04000104: case 0x04000105:
  case 0x04000108: case 0x04000109:
  case 0x0400010c: case 0x0400010d: {
    auto &timer = regs.timer[(addr >> 2) & 3];
    unsigned shift = (addr & 1) * 8;
    timer.reload = (timer.reload & ~(255 << shift)) | (byte << shift);
    return;
  }

  //TM0CNT_H
  //TM1CNT_H
  //TM2CNT_H
  //TM3CNT_H
  case 0x04000102:
  case 0x04000106:
  case 0x0400010a:
  case 0x0400010e: {
    auto &timer = regs.timer[(addr >> 2) & 3];
    bool enable = timer.control.enable;
    timer.control = byte;
    if(enable == 0 && timer.control.enable == 1) {
      timer.counter = timer.reload;
    }
    return;
  }

  //KEYCNT
  case 0x04000132: regs.keypad.control = (regs.keypad.control & 0xff00) | (byte << 0); return;
  case 0x04000133: regs.keypad.control = (regs.keypad.control & 0x00ff) | (byte << 8); return;

  //IE
  case 0x04000200: regs.irq.enable = (regs.irq.enable & 0xff00) | (byte << 0); return;
  case 0x04000201: regs.irq.enable = (regs.irq.enable & 0x00ff) | (byte << 8); return;

  //IF
  case 0x04000202: regs.irq.flag = regs.irq.flag & ~(byte << 0); return;
  case 0x04000203: regs.irq.flag = regs.irq.flag & ~(byte << 8); return;

  //WAITCNT
  case 0x04000204: regs.wait.control = (regs.wait.control & 0xff00) | ((byte & 0xff) << 0); return;
  case 0x04000205: regs.wait.control = (regs.wait.control & 0x00ff) | ((byte & 0x7f) << 8); return;

  //IME
  case 0x04000208: regs.ime = byte & 1; return;
  case 0x04000209: return;

  //POSTFLG + HALTCNT
  case 0x04000300: regs.postboot = byte & 1; return;
  case 0x04000301: regs.mode = byte & 0x80 ? Registers::Mode::Stop : Registers::Mode::Halt; return;

  //MEMCNT_L
  case 0x04000800: regs.memory.control = (regs.memory.control & 0xffffff00) | (byte <<  0); return;
  case 0x04000801: regs.memory.control = (regs.memory.control & 0xffff00ff) | (byte <<  8); return;

  //MEMCNT_H
  case 0x04000802: regs.memory.control = (regs.memory.control & 0xff00ffff) | (byte << 16); return;
  case 0x04000803: regs.memory.control = (regs.memory.control & 0x00ffffff) | (byte << 24); return;

  }
}
