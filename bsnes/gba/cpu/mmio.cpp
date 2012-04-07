uint8 CPU::read(uint32 addr) {
  uint8 result = 0;

  switch(addr) {

  //DMA0CNT_H
  case 0x040000ba: return regs.dma[0].control >> 0;
  case 0x040000bb: return regs.dma[0].control >> 8;

  //DMA1CNT_H
  case 0x040000c6: return regs.dma[1].control >> 0;
  case 0x040000c7: return regs.dma[1].control >> 8;

  //DMA2CNT_H
  case 0x040000d2: return regs.dma[2].control >> 0;
  case 0x040000d3: return regs.dma[2].control >> 8;

  //DMA3CNT_H
  case 0x040000de: return regs.dma[3].control >> 0;
  case 0x040000df: return regs.dma[3].control >> 8;

  //TM0CNT_L
  case 0x04000100: return regs.timer[0].counter >> 0;
  case 0x04000101: return regs.timer[0].counter >> 8;

  //TIM0CNT_H
  case 0x04000102: return regs.timer[0].control >> 0;
  case 0x04000103: return regs.timer[0].control >> 8;

  //TM1CNT_L
  case 0x04000104: return regs.timer[1].reload >> 0;
  case 0x04000105: return regs.timer[1].reload >> 8;

  //TM1CNT_H
  case 0x04000106: return regs.timer[1].control >> 0;
  case 0x04000107: return regs.timer[1].control >> 8;

  //TM2CNT_L
  case 0x04000108: return regs.timer[2].reload >> 0;
  case 0x04000109: return regs.timer[2].reload >> 8;

  //TM2CNT_H
  case 0x0400010a: return regs.timer[2].control >> 0;
  case 0x0400010b: return regs.timer[2].control >> 8;

  //TM3CNT_L
  case 0x0400010c: return regs.timer[3].reload >> 0;
  case 0x0400010d: return regs.timer[3].reload >> 8;

  //TM3CNT_H
  case 0x0400010e: return regs.timer[3].control >> 0;
  case 0x0400010f: return regs.timer[3].control >> 8;

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

  //TM0CNT_L
  case 0x04000100: regs.timer[0].reload = (regs.timer[0].reload & 0xff00) | (byte << 0); return;
  case 0x04000101: regs.timer[0].reload = (regs.timer[0].reload & 0x00ff) | (byte << 8); return;

  //TM1CNT_L
  case 0x04000104: regs.timer[1].reload = (regs.timer[1].reload & 0xff00) | (byte << 0); return;
  case 0x04000105: regs.timer[1].reload = (regs.timer[1].reload & 0x00ff) | (byte << 8); return;

  //TM2CNT_L
  case 0x04000108: regs.timer[2].reload = (regs.timer[2].reload & 0xff00) | (byte << 0); return;
  case 0x04000109: regs.timer[2].reload = (regs.timer[2].reload & 0x00ff) | (byte << 8); return;

  //TM3CNT_L
  case 0x0400010c: regs.timer[3].reload = (regs.timer[3].reload & 0xff00) | (byte << 0); return;
  case 0x0400010d: regs.timer[3].reload = (regs.timer[3].reload & 0x00ff) | (byte << 8); return;

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
    if(timer.control.enable == 0 && enable == 1) {
      timer.counter = timer.reload;
    }
    timer.control = byte;
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
