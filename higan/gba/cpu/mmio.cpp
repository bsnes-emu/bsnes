auto CPU::read(uint32 addr) -> uint8 {
  auto dma = [&]() -> Registers::DMA& { return regs.dma[addr / 12 & 3]; };
  auto timer = [&]() -> Registers::Timer& { return regs.timer[addr.bits(2,3)]; };

  switch(addr) {

  //DMA0CNT_H, DMA1CNT_H, DMA2CNT_H, DMA3CNT_H
  case 0x0400'00ba: case 0x0400'00c6: case 0x0400'00d2: case 0x0400'00de: return (
    dma().control.targetmode        << 5
  | dma().control.sourcemode.bit(0) << 7
  );
  case 0x0400'00bb: case 0x0400'00c7: case 0x0400'00d3: case 0x0400'00df: return (
    dma().control.sourcemode.bit(1) << 0
  | dma().control.repeat            << 1
  | dma().control.size              << 2
  | dma().control.drq               << 3
  | dma().control.timingmode        << 4
  | dma().control.irq               << 6
  | dma().control.enable            << 7
  );

  //TM0CNT_L, TM1CNT_L, TM2CNT_L, TM3CNT_L
  case 0x0400'0100: case 0x0400'0104: case 0x0400'0108: case 0x0400'010c: return timer().period.byte(0);
  case 0x0400'0101: case 0x0400'0105: case 0x0400'0109: case 0x0400'010d: return timer().period.byte(1);

  //TM0CNT_H, TM1CNT_H, TM2CNT_H, TM3CNT_H
  case 0x0400'0102: case 0x0400'0106: case 0x0400'010a: case 0x0400'010e: return (
    timer().control.frequency << 0
  | timer().control.cascade   << 2
  | timer().control.irq       << 6
  | timer().control.enable    << 7
  );
  case 0x0400'0103: case 0x0400'0107: case 0x0400'010b: case 0x0400'010f: return 0;

  //SIOMULTI0 (SIODATA32_L), SIOMULTI1 (SIODATA32_H), SIOMULTI2, SIOMULTI3
  case 0x0400'0120: case 0x0400'0122: case 0x0400'0124: case 0x0400'0126: {
    if(auto data = player.read()) return data().byte(addr.bits(0,1));
    return regs.serial.data[addr.bits(1,2)].byte(0);
  }
  case 0x0400'0121: case 0x0400'0123: case 0x0400'0125: case 0x0400'0127: {
    if(auto data = player.read()) return data().byte(addr.bits(0,1));
    return regs.serial.data[addr.bits(1,2)].byte(1);
  }

  //SIOCNT
  case 0x0400'0128: return (
    regs.serial.control.shiftclockselect      << 0
  | regs.serial.control.shiftclockfrequency   << 1
  | regs.serial.control.transferenablereceive << 2
  | regs.serial.control.transferenablesend    << 3
  | regs.serial.control.startbit              << 7
  );
  case 0x0400'0129: return (
    regs.serial.control.transferlength << 4
  | regs.serial.control.irqenable      << 6
  );

  //SIOMLT_SEND (SIODATA8)
  case 0x0400'012a: return regs.serial.data8;
  case 0x0400'012b: return 0;

  //KEYINPUT
  case 0x04000130: {
    static const uint lookup[] = {5, 4, 8, 9, 3, 2, 0, 1};
    if(auto result = player.keyinput()) return result() >> 0;
    uint8 result = 0;
    for(uint n = 0; n < 8; n++) result |= interface->inputPoll(0, 0, lookup[n]) << n;
    if((result & 0xc0) == 0xc0) result &= (uint8)~0xc0;  //up+down cannot be pressed simultaneously
    if((result & 0x30) == 0x30) result &= (uint8)~0x30;  //left+right cannot be pressed simultaneously
    return result ^ 0xff;
  }
  case 0x04000131: {
    if(auto result = player.keyinput()) return result() >> 8;
    uint8 result = 0;
    result |= interface->inputPoll(0, 0, 7) << 0;
    result |= interface->inputPoll(0, 0, 6) << 1;
    return result ^ 0x03;
  }

  //KEYCNT
  case 0x0400'0132: return (
    regs.keypad.control.flag[0] << 0
  | regs.keypad.control.flag[1] << 1
  | regs.keypad.control.flag[2] << 2
  | regs.keypad.control.flag[3] << 3
  | regs.keypad.control.flag[4] << 4
  | regs.keypad.control.flag[5] << 5
  | regs.keypad.control.flag[6] << 6
  | regs.keypad.control.flag[7] << 7
  );
  case 0x0400'0133: return (
    regs.keypad.control.flag[8]   << 0
  | regs.keypad.control.flag[9]   << 1
  | regs.keypad.control.enable    << 6
  | regs.keypad.control.condition << 7
  );

  //RCNT
  case 0x0400'0134: return (
    regs.joybus.settings.sc     << 0
  | regs.joybus.settings.sd     << 1
  | regs.joybus.settings.si     << 2
  | regs.joybus.settings.so     << 3
  | regs.joybus.settings.scmode << 4
  | regs.joybus.settings.sdmode << 5
  | regs.joybus.settings.simode << 6
  | regs.joybus.settings.somode << 7
  );
  case 0x0400'0135: return (
    regs.joybus.settings.irqenable << 0
  | regs.joybus.settings.mode      << 6
  );

  //JOYCNT
  case 0x0400'0140: return (
    regs.joybus.control.resetsignal     << 0
  | regs.joybus.control.receivecomplete << 1
  | regs.joybus.control.sendcomplete    << 2
  | regs.joybus.control.irqenable       << 6
  );
  case 0x0400'0141: return 0;
  case 0x0400'0142: return 0;
  case 0x0400'0143: return 0;

  //JOY_RECV_L, JOY_RECV_H
  case 0x0400'0150: return regs.joybus.receive.byte(0);
  case 0x0400'0151: return regs.joybus.receive.byte(1);
  case 0x0400'0152: return regs.joybus.receive.byte(2);
  case 0x0400'0153: return regs.joybus.receive.byte(3);

  //JOY_TRANS_L, JOY_TRANS_H
  case 0x0400'0154: return regs.joybus.transmit.byte(0);
  case 0x0400'0155: return regs.joybus.transmit.byte(1);
  case 0x0400'0156: return regs.joybus.transmit.byte(2);
  case 0x0400'0157: return regs.joybus.transmit.byte(3);

  //JOYSTAT
  case 0x0400'0158: return (
    regs.joybus.status.receiveflag << 1
  | regs.joybus.status.sendflag    << 3
  | regs.joybus.status.generalflag << 4
  );
  case 0x0400'0159: return 0;
  case 0x0400'015a: return 0;
  case 0x0400'015b: return 0;

  //IE
  case 0x0400'0200: return regs.irq.enable.byte(0);
  case 0x0400'0201: return regs.irq.enable.byte(1);

  //IF
  case 0x0400'0202: return regs.irq.flag.byte(0);
  case 0x0400'0203: return regs.irq.flag.byte(1);

  //WAITCNT
  case 0x0400'0204: return (
    regs.wait.control.nwait[3] << 0
  | regs.wait.control.nwait[0] << 2
  | regs.wait.control.swait[0] << 4
  | regs.wait.control.nwait[1] << 5
  | regs.wait.control.swait[1] << 7
  );
  case 0x0400'0205: return (
    regs.wait.control.nwait[2] << 0
  | regs.wait.control.swait[2] << 2
  | regs.wait.control.phi      << 3
  | regs.wait.control.prefetch << 6
  | regs.wait.control.gametype << 7
  );

  //IME
  case 0x0400'0208: return regs.ime;
  case 0x0400'0209: return 0;

  //POSTFLG + HALTCNT
  case 0x0400'0300: return regs.postboot;
  case 0x0400'0301: return 0;

  //MEMCNT_L
  case 0x0400'0800: return (
    regs.memory.control.disable  << 0
  | regs.memory.control.unknown1 << 1
  | regs.memory.control.ewram    << 5
  );
  case 0x0400'0801: return 0;

  //MEMCNT_H
  case 0x0400'0802: return 0;
  case 0x0400'0803: return (
    regs.memory.control.ewramwait << 0
  | regs.memory.control.unknown2  << 4
  );

  }

  return 0;
}

auto CPU::write(uint32 addr, uint8 data) -> void {
  auto dma = [&]() -> Registers::DMA& { return regs.dma[addr / 12 & 3]; };
  auto timer = [&]() -> Registers::Timer& { return regs.timer[addr.bits(2,3)]; };

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
    dma().control.targetmode        = data.bits(5,6);
    dma().control.sourcemode.bit(0) = data.bit (7);
    return;
  case 0x0400'00bb: case 0x0400'00c7: case 0x0400'00d3: case 0x0400'00df: {
    bool enable = dma().control.enable;
    if(addr != 0x0400'00df) data.bit(3) = 0;  //gamepad DRQ valid for DMA3 only

    dma().control.sourcemode.bit(1) = data.bit (0);
    dma().control.repeat            = data.bit (1);
    dma().control.size              = data.bit (2);
    dma().control.drq               = data.bit (3);
    dma().control.timingmode        = data.bits(4,5);
    dma().control.irq               = data.bit (6);
    dma().control.enable            = data.bit (7);

    if(!enable && dma().control.enable) {  //0->1 transition
      if(dma().control.timingmode == 0) dma().pending = true;  //immediate transfer mode
      dma().run.target = dma().target;
      dma().run.source = dma().source;
      dma().run.length = dma().length;
    } else if(!dma().control.enable) {
      dma().pending = false;
    }
    return;
  }

  //TM0CNT_L, TM1CNT_L, TM2CNT_L, TM3CNT_L
  case 0x0400'0100: case 0x0400'0104: case 0x0400'0108: case 0x0400'010c: timer().reload.byte(0) = data; return;
  case 0x0400'0101: case 0x0400'0105: case 0x0400'0109: case 0x0400'010d: timer().reload.byte(1) = data; return;

  //TM0CNT_H, TM1CNT_H, TM2CNT_H, TM3CNT_H
  case 0x0400'0102: case 0x0400'0106: case 0x0400'010a: case 0x0400'010e: {
    bool enable = timer().control.enable;

    timer().control.frequency = data.bits(0,1);
    timer().control.cascade   = data.bit (2);
    timer().control.irq       = data.bit (6);
    timer().control.enable    = data.bit (7);

    if(!enable && timer().control.enable) {  //0->1 transition
      timer().pending = true;
    }
    return;
  }
  case 0x0400'0103: case 0x0400'0107: case 0x0400'010b: case 0x0400'010f:
    return;

  //SIOMULTI0 (SIODATA32_L), SIOMULTI1 (SIODATA32_H), SIOMULTI2, SIOMULTI3
  case 0x0400'0120: case 0x0400'0122: case 0x0400'0124: case 0x0400'0126:
    player.write(addr.bits(0,1), data);
    regs.serial.data[addr.bits(1,2)].byte(0) = data;
    return;
  case 0x0400'0121: case 0x0400'0123: case 0x0400'0125: case 0x0400'0127:
    player.write(addr.bits(0,1), data);
    regs.serial.data[addr.bits(1,2)].byte(1) = data;
    return;

  //SIOCNT
  case 0x0400'0128:
    regs.serial.control.shiftclockselect      = data.bit(0);
    regs.serial.control.shiftclockfrequency   = data.bit(1);
    regs.serial.control.transferenablereceive = data.bit(2);
    regs.serial.control.transferenablesend    = data.bit(3);
    regs.serial.control.startbit              = data.bit(7);
    return;
  case 0x0400'0129:
    regs.serial.control.transferlength = data.bit(4);
    regs.serial.control.irqenable      = data.bit(6);
    return;

  //SIOMLT_SEND (SIODATA8)
  case 0x0400'012a: regs.serial.data8 = data; return;
  case 0x0400'012b: return;

  //KEYCNT
  case 0x0400'0132:
    regs.keypad.control.flag[0] = data.bit(0);
    regs.keypad.control.flag[1] = data.bit(1);
    regs.keypad.control.flag[2] = data.bit(2);
    regs.keypad.control.flag[3] = data.bit(3);
    regs.keypad.control.flag[4] = data.bit(4);
    regs.keypad.control.flag[5] = data.bit(5);
    regs.keypad.control.flag[6] = data.bit(6);
    regs.keypad.control.flag[7] = data.bit(7);
    return;
  case 0x0400'0133:
    regs.keypad.control.flag[8]   = data.bit(0);
    regs.keypad.control.flag[9]   = data.bit(1);
    regs.keypad.control.enable    = data.bit(6);
    regs.keypad.control.condition = data.bit(7);
    return;

  //RCNT
  case 0x0400'0134:
    regs.joybus.settings.sc     = data.bit(0);
    regs.joybus.settings.sd     = data.bit(1);
    regs.joybus.settings.si     = data.bit(2);
    regs.joybus.settings.so     = data.bit(3);
    regs.joybus.settings.scmode = data.bit(4);
    regs.joybus.settings.sdmode = data.bit(5);
    regs.joybus.settings.simode = data.bit(6);
    regs.joybus.settings.somode = data.bit(7);
    return;
  case 0x0400'0135:
    regs.joybus.settings.irqenable = data.bit (0);
    regs.joybus.settings.mode      = data.bits(6,7);
    return;

  //JOYCNT
  case 0x0400'0140:
    regs.joybus.control.resetsignal     = data.bit(0);
    regs.joybus.control.receivecomplete = data.bit(1);
    regs.joybus.control.sendcomplete    = data.bit(2);
    regs.joybus.control.irqenable       = data.bit(6);
    return;
  case 0x0400'0141: return;
  case 0x0400'0142: return;
  case 0x0400'0143: return;

  //JOY_RECV_L
  //JOY_RECV_H
  case 0x0400'0150: regs.joybus.receive.byte(0) = data; return;
  case 0x0400'0151: regs.joybus.receive.byte(1) = data; return;
  case 0x0400'0152: regs.joybus.receive.byte(2) = data; return;
  case 0x0400'0153: regs.joybus.receive.byte(3) = data; return;

  //JOY_TRANS_L
  //JOY_TRANS_H
  case 0x0400'0154: regs.joybus.transmit.byte(0) = data; return;
  case 0x0400'0155: regs.joybus.transmit.byte(1) = data; return;
  case 0x0400'0156: regs.joybus.transmit.byte(2) = data; return;
  case 0x0400'0157: regs.joybus.transmit.byte(3) = data; return;

  //JOYSTAT
  case 0x0400'0158:
    regs.joybus.status.receiveflag = data.bit (1);
    regs.joybus.status.sendflag    = data.bit (3);
    regs.joybus.status.generalflag = data.bits(4,5);
    return;
  case 0x0400'0159: return;

  //IE
  case 0x0400'0200: regs.irq.enable.byte(0) = data; return;
  case 0x0400'0201: regs.irq.enable.byte(1) = data; return;

  //IF
  case 0x0400'0202: regs.irq.flag.byte(0) = regs.irq.flag.byte(0) & ~data; return;
  case 0x0400'0203: regs.irq.flag.byte(1) = regs.irq.flag.byte(1) & ~data; return;

  //WAITCNT
  case 0x0400'0204:
    regs.wait.control.swait[3] = data.bit (0);  //todo: is this correct?
    regs.wait.control.nwait[3] = data.bits(0,1);
    regs.wait.control.nwait[0] = data.bits(2,3);
    regs.wait.control.swait[0] = data.bit (4);
    regs.wait.control.nwait[1] = data.bits(5,6);
    regs.wait.control.swait[1] = data.bit (7);
    return;
  case 0x0400'0205:
    regs.wait.control.nwait[2] = data.bits(0,1);
    regs.wait.control.swait[2] = data.bit (2);
    regs.wait.control.phi      = data.bit (3);
    regs.wait.control.prefetch = data.bit (6);
    regs.wait.control.gametype = data.bit (7);
    return;

  //IME
  case 0x0400'0208: regs.ime = data.bit(0); return;
  case 0x0400'0209: return;

  //POSTFLG, HALTCNT
  case 0x0400'0300:
    if(data.bit(0)) regs.postboot = 1;
    return;
  case 0x0400'0301:
    regs.mode = data.bit(7) ? Registers::Mode::Stop : Registers::Mode::Halt;
    return;

  //MEMCNT_L
  //MEMCNT_H
  case 0x0400'0800:
    regs.memory.control.disable  = data.bit (0);
    regs.memory.control.unknown1 = data.bits(1,3);
    regs.memory.control.ewram    = data.bit (5);
    return;
  case 0x0400'0801: return;
  case 0x0400'0802: return;
  case 0x0400'0803:
    regs.memory.control.ewramwait = data.bits(0,3);
    regs.memory.control.unknown2  = data.bits(4,7);
    return;

  }
}
