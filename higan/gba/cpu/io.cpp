auto CPU::readIO(uint32 addr) -> uint8 {
  auto dma = [&]() -> DMA& { return this->dma[addr / 12 & 3]; };
  auto timer = [&]() -> Timer& { return this->timer[addr.bits(2,3)]; };

  switch(addr) {

  //DMA0CNT_L, DMA1CNT_L, DMA2CNT_L, DMA3CNT_L
  case 0x0400'00b8: case 0x0400'00c4: case 0x0400'00d0: case 0x0400'00dc: return 0x00;
  case 0x0400'00b9: case 0x0400'00c5: case 0x0400'00d1: case 0x0400'00dd: return 0x00;

  //DMA0CNT_H, DMA1CNT_H, DMA2CNT_H, DMA3CNT_H
  case 0x0400'00ba: case 0x0400'00c6: case 0x0400'00d2: case 0x0400'00de: return (
    dma().targetMode        << 5
  | dma().sourceMode.bit(0) << 7
  );
  case 0x0400'00bb: case 0x0400'00c7: case 0x0400'00d3: case 0x0400'00df: return (
    dma().sourceMode.bit(1) << 0
  | dma().repeat            << 1
  | dma().size              << 2
  | dma().drq               << 3
  | dma().timingMode        << 4
  | dma().irq               << 6
  | dma().enable            << 7
  );

  //TM0CNT_L, TM1CNT_L, TM2CNT_L, TM3CNT_L
  case 0x0400'0100: case 0x0400'0104: case 0x0400'0108: case 0x0400'010c: return timer().period.byte(0);
  case 0x0400'0101: case 0x0400'0105: case 0x0400'0109: case 0x0400'010d: return timer().period.byte(1);

  //TM0CNT_H, TM1CNT_H, TM2CNT_H, TM3CNT_H
  case 0x0400'0102: case 0x0400'0106: case 0x0400'010a: case 0x0400'010e: return (
    timer().frequency << 0
  | timer().cascade   << 2
  | timer().irq       << 6
  | timer().enable    << 7
  );
  case 0x0400'0103: case 0x0400'0107: case 0x0400'010b: case 0x0400'010f: return 0;

  //SIOMULTI0 (SIODATA32_L), SIOMULTI1 (SIODATA32_H), SIOMULTI2, SIOMULTI3
  case 0x0400'0120: case 0x0400'0122: case 0x0400'0124: case 0x0400'0126: {
    if(auto data = player.read()) return data().byte(addr.bits(0,1));
    return serial.data[addr.bits(1,2)].byte(0);
  }
  case 0x0400'0121: case 0x0400'0123: case 0x0400'0125: case 0x0400'0127: {
    if(auto data = player.read()) return data().byte(addr.bits(0,1));
    return serial.data[addr.bits(1,2)].byte(1);
  }

  //SIOCNT
  case 0x0400'0128: return (
    serial.shiftClockSelect      << 0
  | serial.shiftClockFrequency   << 1
  | serial.transferEnableReceive << 2
  | serial.transferEnableSend    << 3
  | serial.startBit              << 7
  );
  case 0x0400'0129: return (
    serial.transferLength << 4
  | serial.irqEnable      << 6
  );

  //SIOMLT_SEND (SIODATA8)
  case 0x0400'012a: return serial.data8;
  case 0x0400'012b: return 0;

  //KEYINPUT
  case 0x04000130: {
    static const uint landscape[] = {5, 4, 8, 9, 3, 2, 0, 1};
    static const uint portrait[]  = {5, 4, 8, 9, 0, 1, 2, 3};
    auto lookup = !settings.rotateLeft ? landscape : portrait;
    if(auto result = player.keyinput()) return result() >> 0;
    uint8 result = 0;
    for(uint n = 0; n < 8; n++) result |= platform->inputPoll(0, 0, lookup[n]) << n;
    if((result & 0xc0) == 0xc0) result &= (uint8)~0xc0;  //up+down cannot be pressed simultaneously
    if((result & 0x30) == 0x30) result &= (uint8)~0x30;  //left+right cannot be pressed simultaneously
    return result ^ 0xff;
  }
  case 0x04000131: {
    if(auto result = player.keyinput()) return result() >> 8;
    uint8 result = 0;
    result |= platform->inputPoll(0, 0, 7) << 0;
    result |= platform->inputPoll(0, 0, 6) << 1;
    return result ^ 0x03;
  }

  //KEYCNT
  case 0x0400'0132: return (
    keypad.flag[0] << 0
  | keypad.flag[1] << 1
  | keypad.flag[2] << 2
  | keypad.flag[3] << 3
  | keypad.flag[4] << 4
  | keypad.flag[5] << 5
  | keypad.flag[6] << 6
  | keypad.flag[7] << 7
  );
  case 0x0400'0133: return (
    keypad.flag[8]   << 0
  | keypad.flag[9]   << 1
  | keypad.enable    << 6
  | keypad.condition << 7
  );

  //RCNT
  case 0x0400'0134: return (
    joybus.sc     << 0
  | joybus.sd     << 1
  | joybus.si     << 2
  | joybus.so     << 3
  | joybus.scMode << 4
  | joybus.sdMode << 5
  | joybus.siMode << 6
  | joybus.soMode << 7
  );
  case 0x0400'0135: return (
    joybus.siIRQEnable << 0
  | joybus.mode        << 6
  );

  //JOYCNT
  case 0x0400'0140: return (
    joybus.resetSignal     << 0
  | joybus.receiveComplete << 1
  | joybus.sendComplete    << 2
  | joybus.resetIRQEnable  << 6
  );
  case 0x0400'0141: return 0;
  case 0x0400'0142: return 0;
  case 0x0400'0143: return 0;

  //JOY_RECV_L, JOY_RECV_H
  case 0x0400'0150: return joybus.receive.byte(0);
  case 0x0400'0151: return joybus.receive.byte(1);
  case 0x0400'0152: return joybus.receive.byte(2);
  case 0x0400'0153: return joybus.receive.byte(3);

  //JOY_TRANS_L, JOY_TRANS_H
  case 0x0400'0154: return joybus.transmit.byte(0);
  case 0x0400'0155: return joybus.transmit.byte(1);
  case 0x0400'0156: return joybus.transmit.byte(2);
  case 0x0400'0157: return joybus.transmit.byte(3);

  //JOYSTAT
  case 0x0400'0158: return (
    joybus.receiveFlag << 1
  | joybus.sendFlag    << 3
  | joybus.generalFlag << 4
  );
  case 0x0400'0159: return 0;
  case 0x0400'015a: return 0;
  case 0x0400'015b: return 0;

  //IE
  case 0x0400'0200: return irq.enable.byte(0);
  case 0x0400'0201: return irq.enable.byte(1);

  //IF
  case 0x0400'0202: return irq.flag.byte(0);
  case 0x0400'0203: return irq.flag.byte(1);

  //WAITCNT
  case 0x0400'0204: return (
    wait.nwait[3] << 0
  | wait.nwait[0] << 2
  | wait.swait[0] << 4
  | wait.nwait[1] << 5
  | wait.swait[1] << 7
  );
  case 0x0400'0205: return (
    wait.nwait[2] << 0
  | wait.swait[2] << 2
  | wait.phi      << 3
  | wait.prefetch << 6
  | wait.gameType << 7
  );

  //IME
  case 0x0400'0208: return irq.ime;
  case 0x0400'0209: return 0;

  //POSTFLG + HALTCNT
  case 0x0400'0300: return context.booted;
  case 0x0400'0301: return 0;

  //MEMCNT_L
  case 0x0400'0800: return (
    memory.disable  << 0
  | memory.unknown1 << 1
  | memory.ewram    << 5
  );
  case 0x0400'0801: return 0;

  //MEMCNT_H
  case 0x0400'0802: return 0;
  case 0x0400'0803: return (
    memory.ewramWait << 0
  | memory.unknown2  << 4
  );

  }

  return cpu.pipeline.fetch.instruction.byte(addr & 1);
}

auto CPU::writeIO(uint32 addr, uint8 data) -> void {
  auto dma = [&]() -> DMA& { return this->dma[addr / 12 & 3]; };
  auto timer = [&]() -> Timer& { return this->timer[addr.bits(2,3)]; };

  switch(addr) {

  //DMA0SAD, DMA1SAD, DMA2SAD, DMA3SAD
  case 0x0400'00b0: case 0x0400'00bc: case 0x0400'00c8: case 0x0400'00d4: dma().source.byte(0) = data; return;
  case 0x0400'00b1: case 0x0400'00bd: case 0x0400'00c9: case 0x0400'00d5: dma().source.byte(1) = data; return;
  case 0x0400'00b2: case 0x0400'00be: case 0x0400'00ca: case 0x0400'00d6: dma().source.byte(2) = data; return;
  case 0x0400'00b3: case 0x0400'00bf: case 0x0400'00cb: case 0x0400'00d7: dma().source.byte(3) = data; return;

  //DMA0DAD, DMA1DAD, DMA2DAD, DMA3DAD
  case 0x0400'00b4: case 0x0400'00c0: case 0x0400'00cc: case 0x0400'00d8: dma().target.byte(0) = data; return;
  case 0x0400'00b5: case 0x0400'00c1: case 0x0400'00cd: case 0x0400'00d9: dma().target.byte(1) = data; return;
  case 0x0400'00b6: case 0x0400'00c2: case 0x0400'00ce: case 0x0400'00da: dma().target.byte(2) = data; return;
  case 0x0400'00b7: case 0x0400'00c3: case 0x0400'00cf: case 0x0400'00db: dma().target.byte(3) = data; return;

  //DMA0CNT_L, DMA1CNT_L, DMA2CNT_L, DMA3CNT_L
  case 0x0400'00b8: case 0x0400'00c4: case 0x0400'00d0: case 0x0400'00dc: dma().length.byte(0) = data; return;
  case 0x0400'00b9: case 0x0400'00c5: case 0x0400'00d1: case 0x0400'00dd: dma().length.byte(1) = data; return;

  //DMA0CNT_H, DMA1CNT_H, DMA2CNT_H, DMA3CNT_H
  case 0x0400'00ba: case 0x0400'00c6: case 0x0400'00d2: case 0x0400'00de:
    dma().targetMode        = data.bits(5,6);
    dma().sourceMode.bit(0) = data.bit (7);
    return;
  case 0x0400'00bb: case 0x0400'00c7: case 0x0400'00d3: case 0x0400'00df: {
    bool enable = dma().enable;
    if(addr != 0x0400'00df) data.bit(3) = 0;  //gamepad DRQ valid for DMA3 only

    dma().sourceMode.bit(1) = data.bit (0);
    dma().repeat            = data.bit (1);
    dma().size              = data.bit (2);
    dma().drq               = data.bit (3);
    dma().timingMode        = data.bits(4,5);
    dma().irq               = data.bit (6);
    dma().enable            = data.bit (7);

    if(!enable && dma().enable) {  //0->1 transition
      if(dma().timingMode == 0) {
        dma().active = true;  //immediate transfer mode
        dma().waiting = 2;
      }
      dma().latch.target = dma().target;
      dma().latch.source = dma().source;
      dma().latch.length = dma().length;
    } else if(!dma().enable) {
      dma().active = false;
    }
    return;
  }

  //TM0CNT_L, TM1CNT_L, TM2CNT_L, TM3CNT_L
  case 0x0400'0100: case 0x0400'0104: case 0x0400'0108: case 0x0400'010c: timer().reload.byte(0) = data; return;
  case 0x0400'0101: case 0x0400'0105: case 0x0400'0109: case 0x0400'010d: timer().reload.byte(1) = data; return;

  //TM0CNT_H, TM1CNT_H, TM2CNT_H, TM3CNT_H
  case 0x0400'0102: case 0x0400'0106: case 0x0400'010a: case 0x0400'010e: {
    bool enable = timer().enable;

    timer().frequency = data.bits(0,1);
    timer().cascade   = data.bit (2);
    timer().irq       = data.bit (6);
    timer().enable    = data.bit (7);

    if(!enable && timer().enable) {  //0->1 transition
      timer().pending = true;
    }
    return;
  }
  case 0x0400'0103: case 0x0400'0107: case 0x0400'010b: case 0x0400'010f:
    return;

  //SIOMULTI0 (SIODATA32_L), SIOMULTI1 (SIODATA32_H), SIOMULTI2, SIOMULTI3
  case 0x0400'0120: case 0x0400'0122: case 0x0400'0124: case 0x0400'0126:
    player.write(addr.bits(0,1), data);
    serial.data[addr.bits(1,2)].byte(0) = data;
    return;
  case 0x0400'0121: case 0x0400'0123: case 0x0400'0125: case 0x0400'0127:
    player.write(addr.bits(0,1), data);
    serial.data[addr.bits(1,2)].byte(1) = data;
    return;

  //SIOCNT
  case 0x0400'0128:
    serial.shiftClockSelect      = data.bit(0);
    serial.shiftClockFrequency   = data.bit(1);
    serial.transferEnableReceive = data.bit(2);
    serial.transferEnableSend    = data.bit(3);
    serial.startBit              = data.bit(7);
    return;
  case 0x0400'0129:
    serial.transferLength = data.bit(4);
    serial.irqEnable      = data.bit(6);
    return;

  //SIOMLT_SEND (SIODATA8)
  case 0x0400'012a: serial.data8 = data; return;
  case 0x0400'012b: return;

  //KEYCNT
  case 0x0400'0132:
    keypad.flag[0] = data.bit(0);
    keypad.flag[1] = data.bit(1);
    keypad.flag[2] = data.bit(2);
    keypad.flag[3] = data.bit(3);
    keypad.flag[4] = data.bit(4);
    keypad.flag[5] = data.bit(5);
    keypad.flag[6] = data.bit(6);
    keypad.flag[7] = data.bit(7);
    return;
  case 0x0400'0133:
    keypad.flag[8]   = data.bit(0);
    keypad.flag[9]   = data.bit(1);
    keypad.enable    = data.bit(6);
    keypad.condition = data.bit(7);
    return;

  //RCNT
  case 0x0400'0134:
    joybus.sc     = data.bit(0);
    joybus.sd     = data.bit(1);
    joybus.si     = data.bit(2);
    joybus.so     = data.bit(3);
    joybus.scMode = data.bit(4);
    joybus.sdMode = data.bit(5);
    joybus.siMode = data.bit(6);
    joybus.soMode = data.bit(7);
    return;
  case 0x0400'0135:
    joybus.siIRQEnable = data.bit (0);
    joybus.mode        = data.bits(6,7);
    return;

  //JOYCNT
  case 0x0400'0140:
    joybus.resetSignal     = data.bit(0);
    joybus.receiveComplete = data.bit(1);
    joybus.sendComplete    = data.bit(2);
    joybus.resetIRQEnable  = data.bit(6);
    return;
  case 0x0400'0141: return;
  case 0x0400'0142: return;
  case 0x0400'0143: return;

  //JOY_RECV_L
  //JOY_RECV_H
  case 0x0400'0150: joybus.receive.byte(0) = data; return;
  case 0x0400'0151: joybus.receive.byte(1) = data; return;
  case 0x0400'0152: joybus.receive.byte(2) = data; return;
  case 0x0400'0153: joybus.receive.byte(3) = data; return;

  //JOY_TRANS_L
  //JOY_TRANS_H
  case 0x0400'0154: joybus.transmit.byte(0) = data; return;
  case 0x0400'0155: joybus.transmit.byte(1) = data; return;
  case 0x0400'0156: joybus.transmit.byte(2) = data; return;
  case 0x0400'0157: joybus.transmit.byte(3) = data; return;

  //JOYSTAT
  case 0x0400'0158:
    joybus.receiveFlag = data.bit (1);
    joybus.sendFlag    = data.bit (3);
    joybus.generalFlag = data.bits(4,5);
    return;
  case 0x0400'0159: return;

  //IE
  case 0x0400'0200: irq.enable.byte(0) = data; return;
  case 0x0400'0201: irq.enable.byte(1) = data; return;

  //IF
  case 0x0400'0202: irq.flag.byte(0) = irq.flag.byte(0) & ~data; return;
  case 0x0400'0203: irq.flag.byte(1) = irq.flag.byte(1) & ~data; return;

  //WAITCNT
  case 0x0400'0204:
    wait.swait[3] = data.bit (0);  //todo: is this correct?
    wait.nwait[3] = data.bits(0,1);
    wait.nwait[0] = data.bits(2,3);
    wait.swait[0] = data.bit (4);
    wait.nwait[1] = data.bits(5,6);
    wait.swait[1] = data.bit (7);
    return;
  case 0x0400'0205:
    wait.nwait[2] = data.bits(0,1);
    wait.swait[2] = data.bit (2);
    wait.phi      = data.bit (3);
    wait.prefetch = data.bit (6);
  //wait.gameType is read-only
    return;

  //IME
  case 0x0400'0208: irq.ime = data.bit(0); return;
  case 0x0400'0209: return;

  //POSTFLG, HALTCNT
  case 0x0400'0300:
    if(data.bit(0)) context.booted = 1;
    return;
  case 0x0400'0301:
    context.halted  = data.bit(7) == 0;
    context.stopped = data.bit(7) == 1;
    return;

  //MEMCNT_L
  //MEMCNT_H
  case 0x0400'0800:
    memory.disable  = data.bit (0);
    memory.unknown1 = data.bits(1,3);
    memory.ewram    = data.bit (5);
    return;
  case 0x0400'0801: return;
  case 0x0400'0802: return;
  case 0x0400'0803:
    memory.ewramWait = data.bits(0,3);
    memory.unknown2  = data.bits(4,7);
    return;

  }
}
