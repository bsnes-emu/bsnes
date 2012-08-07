
ARM7TDMI::ARM7TDMI() {
  bios.data = new uint32[(bios.size = 0x004000)/4]();
  memset(bios.data, 0, bios.size);
}

void ARM7TDMI::Thread() { arm7.main(); }

void ARM7TDMI::power() {
  // 33513982 MHz
  if(thread) co_delete(thread);
  thread = co_create(262144 * sizeof(void*), ARM7TDMI::Thread);
  
  vectorBase     = 0x00000000;
  bxWithLoadedPC = false;
  
  booted = 0;
  flag300 = 0;
  
  spi.enable  = 0;
  spi.hold    = 0;
  spi.irq     = 0;
  spi.size    = 0;
  spi.divider = 0;
  spi.device  = SPI::none;
  spi.data    = 0;
  
  rtc.in[0]  = rtc.in[1]  = 0xf;
  rtc.out[0] = rtc.out[1] = 0x0;
  rtc.dir[0] = rtc.dir[1] = 0x0;
  rtc.buffer = 0;
  rtc.index  = 0;
  
  sio.in   = 0;
  sio.out  = 0;
  sio.dir  = 0;
  sio.irq  = false;
  sio.mode = 0;
  
  CPUCore::power();
  trace = false;
}

uint16 crc16(uint8* data, unsigned size, uint16 initial) {
  uint16 table[] = { 0xa001,0xf001,0xd801,0xcc01,0xc601,0xc301,0xc181,0xc0c1 };
  uint32 crc = initial;
  
  for(unsigned i = 0; i < size; i++) {
    crc ^= data[i];
    for(int j = 7; j >= 0; j--)
      crc = crc>>1 ^ (crc&1) * (table[j] << j);
  }
  return crc;
}

void ARM7TDMI::main() {
  if(auto card = slot1.card) {
    // ARM7 BIOS and firmware should be doing this, but it requires
    // clock, card emulation and fancy things like that.
    uint32 arm9src   = card->rom.read(0x20, Word), arm7src   = card->rom.read(0x30, Word);
    uint32 arm9entry = card->rom.read(0x24, Word), arm7entry = card->rom.read(0x34, Word);
    uint32 arm9dest  = card->rom.read(0x28, Word), arm7dest  = card->rom.read(0x38, Word);
    uint32 arm9size  = card->rom.read(0x2c, Word), arm7size  = card->rom.read(0x3c, Word);
    
    // Copy user settings to RAM
    // - Homebrew and games (?) both require this. libnds actually attempts
    //   to read the settings from flash, but the struct contains a bitfield
    //   that GCC expands by 2 bytes, throwing the size and checksum off.
    for(unsigned n = 0; n < 0x70; n += 1) {
      store(0x02fffc80+n, Byte, system.firmware.read(0x3fe00+n, Byte));
    }
    
    // Check CRCs on firmware data and warn if incorrect
    uint8 *wifiData = &system.firmware.data[0x0002a];
    
    uint8 *wfcData[3] = {
      &system.firmware.data[0x3fa00],
      &system.firmware.data[0x3fb00],
      &system.firmware.data[0x3fc00],
    };
    uint8 *userData[2] = {
      &system.firmware.data[0x3fe00],
      &system.firmware.data[0x3ff00],
    };
    
    // MAC address + Wifi chipset programming data
    uint32 wifiDataLen      = wifiData[2] | wifiData[3]<<8;
    uint32 wifiDataExpected = wifiData[0] | wifiData[1]<<8;
    uint32 wifiDataActual   = crc16(wifiData+2, wifiDataLen, 0);
    
    if(wifiDataExpected != wifiDataActual)
      print("Warning: Wifi chipset data:   crc is ",
        hex<4>(wifiDataActual),"; expected ",hex<4>(wifiDataExpected),"\n");
    
    // Nintendo Wifi Connection - access point IDs + WEP passwords
    for(unsigned i = 0; i < 3; i++) {
      uint32 expected = wfcData[i][0xfe] | wfcData[i][0xff]<<8;
      uint32 actual   = crc16(wfcData[i], 0xfe, 0);
      
      if(expected != actual)
        print("Warning: WFC access point #",i,": crc is ",
          hex<4>(actual),"; expected ",hex<4>(expected),"\n");
    }
    
    // User settings area - nickname, birthday, favorite color etc.
    for(unsigned i = 0; i < 2; i++) {
      uint32 expected = userData[i][0x72] | userData[i][0x73]<<8;
      uint32 actual   = crc16(userData[i], 0x70, 0xffff);
      
      if(expected != actual)
        print("Warning: User settings #",i,":    crc is ",
          hex<4>(actual),"; expected ",hex<4>(expected),"\n");
    }
    
    // Copy header into RAM
    for(unsigned n = 0; n < 0x200; n += 4) {
      write(0x02fffe00+n, Word, 0, card->rom.read(n, Word));
    }
    
    // Copy executables
    if(0x200 <= arm9src && arm9src + arm9size <= card->rom.size) {
      if(0x02000000 <= arm9dest && arm9dest + arm9size <= 0x023bfe00) {
        for(unsigned n = 0; n < arm9size; n += 4)
          write(arm9dest + n, Word, 0, card->rom.read(arm9src + n, Word));
      }
    }
    if(0x200 <= arm7src && arm7src + arm7size <= card->rom.size) {
      if(0x02000000 <= arm7dest && arm7dest + arm7size <= 0x023bfe00
      || 0x037f8000 <= arm7dest && arm7dest + arm7size <= 0x03807e00) {
        for(unsigned n = 0; n < arm7size; n += 4)
          write(arm7dest + n, Word, 0, card->rom.read(arm7src + n, Word));
      }
    }
    // Should write the card's ROM ID to RAM too... where is it again?
    arm7.booted = 0;
    arm9.booted = 0;
    
    arm7.writeCpsr(0xdf, 0xf);
    arm7.branch(arm7entry & 1, arm7entry);
    arm7.r[15]    = arm7entry;  // pc
    arm7.r[14]    = arm7entry;  // lr
    arm7.r[13]    = 0x03007f00; // sp
    arm7.r_irq[0] = 0x03007fa0; // sp_irq
    arm7.r_svc[0] = 0x03007fe0; // sp_svc
    
    arm9.writeCpsr(0xdf, 0xf);
    arm9.branch(arm9entry & 1, arm9entry);
    arm9.r[15]    = arm9entry;  // pc
    arm9.r[14]    = arm9entry;  // lr
    arm9.r[13]    = 0x00803ec0; // sp
    arm9.r_irq[0] = 0x00803fa0; // sp_irq
    arm9.r_svc[0] = 0x00803fc0; // sp_svc 
  }
  
  arm7.event.queue.time -= arm7.clock;
  for(unsigned n=1; n <= arm7.event.queue.size; n++)
    arm7.event.queue.items[n]->time -= arm7.clock;
  
  arm7.clock = 0;
  
  for(;;) {
    if(arm7.clock >= 256)
      co_switch(arm9.thread);
    
    event.irq = interrupt.gate && (interrupt.enable & interrupt.flags);
    //if(event.irq && !If) {
    //  print("arm7: irq (if=",hex<8>(interrupt.flags),")\n");
    //}
    
    if(Tf) execTHUMB();
    else   execARM();
  }
}

void ARM7TDMI::istep(unsigned clocks) {
  arm7.clock += 2*clocks;
  event.queue.step(2*clocks);
}

void ARM7TDMI::step(unsigned clocks) {
  return istep(clocks);
}



void ARM7TDMI::execARM() {
  if(branched) {
    //if(r[15] == 0) print(hex<8>(r[14]), ": jump to nullptr!\n");
    branched = false;                 r[15] &= ~3;
    iexecute = fetch(r[15], Word, 0); r[15] += 4;
    idecode  = fetch(r[15], Word, 1); r[15] += 4;
  } else {
    iexecute = idecode, idecode = ifetch;
  }
  ifetch = fetch(r[15], Word, 1);
  uint32 i = iexecute;
  
  if(event.irq && !If) return irq();
  //if(trace && r[15] >= 0x2000000) traceInsn();
  
  if(!evalCond(i>>28))  { r[15] += 4; return; }
  
  //                                                    opcode, sh,      Rd,  Rn,  Rm,  Rs
  if(imatch("00x10xx0/////")) {
    if(imatch("00110r10/// ..../")) return armWritePsr(i>>22,              i>>16, armImmed(i, i>>8));
    if(imatch("00010r10/// 0000/")) return armWritePsr(i>>22,              i>>16, armRm(i));
    if(imatch("00010r00/// 0000/")) return armReadPsr (i>>22,       i>>12);
    if(imatch("00010010/// 00l1/")) return armBranchEx(       i>>5,               i);
    if(imatch("00010b00/// 1001/")) return armMemSwap (i>>22,       i>>12, i>>16, i);
  }
  if(imatch("000...../// 1..1/")) {
    if(imatch("0000luas/// 1001/")) return armMultiply(i>>20,       i>>16, i>>12, i, i>>8);
    if(imatch("000pu0wl/// 1sh1/")) return armMem_v4  (i>>20, i>>5, i>>12, i>>16, armRm(i));
    if(imatch("000pu1wl/// 1sh1/")) return armMem_v4  (i>>20, i>>5, i>>12, i>>16, armOffset8(i, i>>8));
  }
  if(imatch("001aaaas/// ..../")) return armData    (i>>20,       i>>12, i>>16, armImmed(i, i>>8));
  if(imatch("000aaaas/// .sh0/")) return armData    (i>>20,       i>>12, i>>16, shiftImm(i, i>>5, i>>7));
  if(imatch("010pubwl/// ..../")) return armMem     (i>>20,       i>>12, i>>16, armOffset12(i));
  if(imatch("011pubwl/// ...0/")) return armMem     (i>>20,       i>>12, i>>16, shiftImm(i, i>>5, i>>7));
  if(imatch("100puswl/// ..../")) return armBlock   (i>>20,              i>>16, i);
  if(imatch("101l..../// ..../")) return armBranch  (i>>24,                  0, i<<2);
  if(imatch("000aaaas/// 0sh1/")) return armDataRs  (i>>20,       i>>12, i>>16, i, i>>5, i>>8);
  
  if(imatch("1111..../// ..../")) return swi        ();
  return undefined();
}

void ARM7TDMI::execTHUMB() {
  if(branched) {
    //if(r[15] == 0) print(hex<8>(r[14]), ": jump to nullptr!\n");
    branched = false;                                           r[15] &= ~1;
    iexecute = fetch(r[15], Half, 0) >> 8*(r[15] & 2) & 0xffff; r[15] += 2;
    idecode  = fetch(r[15], Half, 1) >> 8*(r[15] & 2) & 0xffff; r[15] += 2;
  } else {
    iexecute = idecode, idecode = ifetch;
  }
  ifetch = fetch(r[15], Half, 1) >> 8*(r[15] & 2) & 0xffff;
  uint16 i = iexecute;
  
  if(event.irq && !If) return irq();
  //if(trace && r[15] >= 0x2000000) traceInsn();
  
  if(imatch("00011ismmmnnnddd")) return thumbAddSub  (i>>9,  i, i>>3, i>>6);
  if(imatch("000ssiiiiimmmddd")) return thumbShiftImm(i>>11, i, i>>3, i>>6);
  if(imatch("001oodddiiiiiiii")) return thumbDataImm (i>>11, i>>8, i);
  if(imatch("010000oooommmddd")) return thumbDataLo  (i>>6,  i, i>>3);
  if(imatch("010001oodmmmmddd")) return thumbDataHi  (i>>8, (i&7)+(i>>4&8), i>>3);
  
  if(imatch("0101ooommmnnnddd")) return thumbMemReg  (i>>9,  i, i>>3, i>>6);
  if(imatch("011bliiiiinnnddd")) return thumbMemImm  (i>>11, i, i>>3, i>>6);
  if(imatch("1000liiiiinnnddd")) return thumbMemImm  (i>>11, i, i>>3, i>>6);
  
  if(imatch("10110000siiiiiii")) return thumbAddSP   (       i>>7, i);
  if(imatch("01001dddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  if(imatch("1001odddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  if(imatch("1010odddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  
  if(imatch("1101ccccrrrrrrrr")) return thumbCond    (       i>>8, i);
  if(imatch("11100rrrrrrrrrrr")) return thumbBranch  (i);
  if(imatch("11110rrrrrrrrrrr")) return thumbBh      (i);
  if(imatch("11111rrrrrrrrrrr")) return thumbBlx     (1, i);
  
  if(imatch("1o..lnnnrrrrrrrr")) return thumbBlock   (i>>11, i>>8, i);
  
  return undefined();
}



uint32 ARM7TDMI::fetch(uint32 addr, uint32 size, bool s) {
  return read(addr, size, s);
}

uint32 ARM7TDMI::read(uint32 addr, uint32 size, bool s) {
  const int h = size==Word? 2 : 1;  // 16-bit bus timing
  const int e = 8*!s + h;           // EWRAM timing
  
  //if((addr & 0xf3ff000) == 0x23ff000) {
  //  istep(e);
  //  uint32 data = system.ewram.read(addr % 0x400000, size);
  //  print(hex<8>(arm7.event.queue.time), " ",hex<8>(r[15])," arm7: r ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
  //  return data;
  //}
  
  switch(addr >> 23) {
  case 0x00>>3:
  case 0x08>>3: istep(1); addr &= bios.size-1; return bios.read(addr, size);
  case 0x20>>3:
  case 0x28>>3: istep(e); addr %= 0x400000; return system.ewram.read(addr, size);
  case 0x30>>3: istep(1); addr %= 0x008000; return system.swram[addr>>14].read(addr % 0x4000, size);
  case 0x38>>3: istep(1); addr %= 0x010000; return system.iwram.read(addr, size);
  case 0x40>>3: istep(1); { //                  return readReg(addr, size);
      uint32 data = readReg(addr, size);
      //print(hex<8>(arm7.event.queue.time), " ", hex<8>(r[15]),"  arm7: r ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
      return data;
    }
  case 0x48>>3: istep(1);                   return wifi.read(addr, size);
  case 0x60>>3:
  case 0x68>>3: istep(h); addr %= 0x040000; return system.vmap.arm7[addr>>14].read(addr, size);
  }
  istep(1);
  return 0;
}

void ARM7TDMI::write(uint32 addr, uint32 size, bool s, uint32 data) {
  const int h = size==Word? 2 : 1;  // 16-bit bus timing
  const int e = 8*!s + h;           // EWRAM timing
  
  //if((addr & 0xf3ff000) == 0x23ff000)
  //  print(hex<8>(arm7.event.queue.time), " ",hex<8>(r[15]), "  arm7: w ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
  
  switch(addr >> 23) {
  case 0x20>>3:
  case 0x28>>3: istep(e); addr %= 0x400000; return system.ewram.write(addr, size, data);
  case 0x30>>3: istep(1); addr %= 0x008000; return system.swram[addr>>14].write(addr % 0x4000, size, data);
  case 0x38>>3: istep(1); addr %= 0x010000; return system.iwram.write(addr, size, data);
  case 0x40>>3: istep(1); { //                  return writeReg(addr, size, data);
      //if(addr != 0x4000301)
      //  print(hex<8>(arm7.event.queue.time), " ", hex<8>(r[15]),"  arm7: w ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
      writeReg(addr, size, data);
      return;
    }
  case 0x48>>3: istep(1);                   return wifi.write(addr, size, data);
  case 0x60>>3:
  case 0x68>>3: istep(h); addr %= 0x040000; if(size > Byte) return system.vmap.arm7[addr>>14].write(addr, size, data);
    // STRB - special case, only works on ARM7
    auto &page = system.vmap.arm7[addr>>14];
    page[addr] &= addr & 1?        0x00ff :        0xff00;
    page[addr] |= addr & 1? data & 0xff00 : data & 0x00ff;
    return;
  }
  istep(1);
}

void ARM7TDMI::dataCop(uint4 cpno, uint4 op1, uint4 ird, uint4 irn, uint4 irm, uint4 op2) {
  return undefined();
}



uint32 ARM7TDMI::readReg(uint32 addr, uint32 size) {
  switch(addr-0x4000000 & ~3) {
  
  case 0x134: {
    // Aux GPIO
    // - missing link port (SI pin = clock /IRQ)
    // - misc inputs
    uint8 keys = 0;
    for(unsigned n = 10; n < 16; n++) {
      if(interface->inputPoll(ID::Port::Buttons, 0, n))
        keys += 1 << n-10;
    }
    if(system.touchscreen.penDown()) keys += 1<<6;
    if(interface->inputPoll(ID::Port::Sensors,  0, ID::Sensors::Lid)==0)    keys += 1<<7;
    return (keys ^ 0xff)<<16 | regSio()<<0;
    }
    
  case 0x138: return regRtc();  // GPIO - system clock
  case 0x1c0: return regSpi();  // SPI  - power, touch, firmware flash
    
  // Memory status, slot 2
  case 0x204: return arm9.ramPriority<<15 | arm9.slot1access<<11
                   | arm9.slot2access<< 7 | regSlot2Control();
  // Wifi waitstates
  case 0x206: break;
  
  case 0x240: break; // VRAM status
  case 0x241: break; // SWRAM status
  
  case 0x300: return flag300<<1 | booted<<0;
  case 0x304: return apu.powered<<0 | wifi.powered<<1;
    
  case 0x400: case 0x410: case 0x420: case 0x430:
  case 0x440: case 0x450: case 0x460: case 0x470:
  case 0x480: case 0x490: case 0x4a0: case 0x4b0:
  case 0x4c0: case 0x4d0: case 0x4e0: case 0x4f0:
    return apu.regVoiceControl(addr>>4 & 15);
    
  case 0x500: return apu.regControl();
  case 0x504: return apu.regBias();
  case 0x508: return apu.regCaptureControl();
  case 0x510: return apu.regCaptureDest(0);
  case 0x518: return apu.regCaptureDest(1);
  
  }
  return CPUCore::readReg(addr, size);
}

void ARM7TDMI::writeReg(uint32 addr, uint32 size, uint32 data) {
  uint32 mask = 0xffffffff;
  if(size == Half) mask = 0xffff << 8*(addr & 2);
  if(size == Byte) mask = 0xff << 8*(addr & 3);
  
  data &= mask;
  
  switch(addr-0x4000000 & ~3) {
  
  case 0x134: return regSio(data, mask);  // GPIO - missing link port
  case 0x138: return regRtc(data, mask);  // GPIO - system clock
  case 0x1c0: return regSpi(data, mask);  // SPI  - power, touch, firmware flash
    
  case 0x204: return regSlot2Control(data, mask);
  case 0x206: break; // Wifi waitstates
  
  case 0x300: // System mode
    booted |= data & 1;  // not possible to clear it
    if(mask & 0xff)
      flag300 = data >> 1;
    
    // 0x4000: halt arm9 and enter GBA mode
    // 0x8000: halt arm7 and wait for IRQ
    // 0xc000: halt system and wait for IRQ (low-power sleep mode)
    //
    // Nearly everything is frozen in sleep mode, including timers. Only
    // a few things can generate interrupts to bring it back out:
    //   - lid sensor, buttons
    //   - alarm from real-time clock
    //   - wireless chipset (?)
    //   - cart in slot 1 or 2 (?)
    if(data & 0xc000) {
      //print("arm7: wait (ime=",interrupt.gate,", ie=",hex<8>(interrupt.enable),")\n");
      powerState = waiting;
      for(;;) {
        if((interrupt.enable & interrupt.flags)) {
          powerState = running;
          break;
        }
        istep(256);
        if(arm7.clock >= 256)
          co_switch(arm9.thread);
      }
    }
    return;
    
  case 0x304: // Power
    if(mask & 0xff) {
      apu.powered  = data>>0;
      wifi.powered = data>>1;
    }
    return;
  
  case 0x308: break; // BIOS protection
  
  case 0x400: case 0x410: case 0x420: case 0x430:
  case 0x440: case 0x450: case 0x460: case 0x470:
  case 0x480: case 0x490: case 0x4a0: case 0x4b0:
  case 0x4c0: case 0x4d0: case 0x4e0: case 0x4f0:
    return apu.regVoiceControl(addr>>4 & 15, data, mask);
    
  case 0x404: case 0x414: case 0x424: case 0x434:
  case 0x444: case 0x454: case 0x464: case 0x474:
  case 0x484: case 0x494: case 0x4a4: case 0x4b4:
  case 0x4c4: case 0x4d4: case 0x4e4: case 0x4f4:
    return apu.regVoiceSource(addr>>4 & 15, data, mask);
    
  case 0x408: case 0x418: case 0x428: case 0x438:
  case 0x448: case 0x458: case 0x468: case 0x478:
  case 0x488: case 0x498: case 0x4a8: case 0x4b8:
  case 0x4c8: case 0x4d8: case 0x4e8: case 0x4f8:
    return apu.regVoicePeriod(addr>>4 & 15, data, mask);
    
  case 0x40c: case 0x41c: case 0x42c: case 0x43c:
  case 0x44c: case 0x45c: case 0x46c: case 0x47c:
  case 0x48c: case 0x49c: case 0x4ac: case 0x4bc:
  case 0x4cc: case 0x4dc: case 0x4ec: case 0x4fc:
    return apu.regVoiceLength(addr>>4 & 15, data, mask);
    
  case 0x500: return apu.regControl(data, mask);
  case 0x504: return apu.regBias(data, mask);
  case 0x508: return apu.regCaptureControl(data, mask);
  case 0x510: return apu.regCaptureDest(0, data, mask);
  case 0x514: return apu.regCaptureLength(0, data, mask);
  case 0x518: return apu.regCaptureDest(1, data, mask);
  case 0x51c: return apu.regCaptureLength(1, data, mask);
  
  }
  return CPUCore::writeReg(addr, size, data);
}


uint32 ARM7TDMI::regSpi() {
  return spi.divider<<0 | spi.device<<8 | spi.size<<10
       | spi.hold<<11 | spi.irq<<14 | spi.enable<<15
       | spi.data<<16;
}

void ARM7TDMI::regSpi(uint32 data, uint32 mask) {
  if(mask & 0x000000ff) {
    spi.divider = data>>0;
  }
  if(mask & 0x0000ff00) {
    spi.device = data>>8;
    spi.size   = data>>10;
    spi.hold   = data>>11;
    spi.irq    = data>>14;
    spi.enable = data>>15;
    
    if(spi.enable == false) {
      // Hrm, must be implicit as libnds does not release the hold
      // before last transfer - or is spi.hold not delayed after all?
      system.firmware.select(false);
      system.powerMgr.select(false);
      system.touchscreen.select(false);
    }
  }
  if(mask & 0xffff0000) {
    if(!spi.enable) return;
    
    SPIDevice* device = nullptr;
    if(spi.device == SPI::flash) device = &system.firmware;
    if(spi.device == SPI::power) device = &system.powerMgr;
    if(spi.device == SPI::touch) device = &system.touchscreen;
    
    if(device) {
      // SPI transfers are bidirectional so there's always a read+write.
      device->select(true);
      spi.data = device->transfer(data>>16);
      device->select(spi.hold);
    }
  }
}



uint32 ARM7TDMI::regRtc() {
  // I've blithely assumed: pin = (out | ~dir) & in
  //
  // Something like that is necessary because Nintendo read-modify-writes
  // the output pins.
  return uint4( (rtc.out[0] | ~rtc.dir[0]) & rtc.in[0] )<<0 | rtc.dir[0]<<4
       | uint4( (rtc.out[1] | ~rtc.dir[1]) & rtc.in[1] )<<8 | rtc.dir[1]<<12;
}

void ARM7TDMI::regRtc(uint32 data, uint32 mask) {
  // It would've been nice if the clock was on the SPI bus, like the touchpad,
  // firmware and power chips.. it seems that wasn't possible because of the
  // bi-directional data pin. Software has to bit-bang the GPIO pins instead.
  
  if(mask & 0x00ff) {
    rtc.out[0] = data>>0;  // if dir==1, use output from the DS side.
    rtc.dir[0] = data>>4;  // if dir==0, use as input with pullup (?)
  }
  if(mask & 0xff00) {
    rtc.out[1] = data>>0;  // these may exist, but don't appear to be used.
    rtc.dir[1] = data>>4;
  }
  // All pins can be either inputs or outputs.
  //   "in" is the value being driven externally (or 1, if not).
  uint4 pins = (rtc.out[0] | ~rtc.dir[0]) & rtc.in[0];
  rtc.in[0] = system.clock.io(pins);
}



uint32 ARM7TDMI::regSio() {
  return sio.in<<0 | sio.dir<<4 | sio.irq<<8 | sio.mode<<14;
}

void ARM7TDMI::regSio(uint32 data, uint32 mask) {
  sio.out = data>>0;
  sio.dir = data>>4;
  sio.irq = data>>8;
  sio.mode = data>>14;
}

