#include <nds/nds.hpp>

namespace NintendoDS {

WIFI wifi;


void WIFI::power() {
  // This isn't really cleared but we have to be deterministic.
  memset(system.wxram.data, 0, system.wxram.size);
  
  powered = true;
  
  reg004 = 0;
  reg034 = 0;
  swMode = 0;
  wepMode = 0;
  memset(macAddr, 0, sizeof macAddr);
  memset(bssId, 0, sizeof bssId);
  assocIdl = 0;
  assocIdf = 0;
  
  rxControl = 0;
  wepControl = 0;
  reg034 = 0;
  baseBandPower = 0;
  
  interrupt.flags = 0;
  interrupt.enable = 0;
  interrupt.counterFlags = 0;
  interrupt.counterEnable = 0;
  interrupt.oflowFlags = 0;
  interrupt.oflowEnable = 0;
  
  pm.txIdle = 0;
  pm.wakeRequest = 0;
  pm.wakePending = 0;
  pm.sleeping = 0;
  
  bb.event.action = [&]() { bbTransferBit(); };
  bb.time = 0;
  bb.busy = 0;
  bb.read = 0;
  bb.write = 0;
  bb.header = 0;
  bb.powerl = 0;
  bb.powerh = 0;
  bb.mode8 = 0;
  bb.modeE = 0;
  bb.clock = 0;
  
  memset(bb.regs, 0, sizeof bb.regs);
  
  bb.regs[0x00] = 0x6d;
  bb.regs[0x4d] = 0xff;
  bb.regs[0x5d] = 0x01;
  bb.regs[0x64] = 0xff;
  
  rf.event.action = [&]() { rfTransferBit(); };
  rf.time = 0;
  rf.busy = 0;
  rf.data = 0;
  rf.length = 24;
  rf.type = 0;
  rf.reserved = 0;
  
  memset(rf.regs, 0, sizeof rf.regs);
  
  rf.regs[0x00] = 0x00007;
  rf.regs[0x01] = 0x09003;
  rf.regs[0x02] = 0x00022;
  rf.regs[0x03] = 0x1ff78;
  rf.regs[0x04] = 0x09003;
  rf.regs[0x05] = 0x01780;
  rf.regs[0x06] = 0x00000;
  rf.regs[0x07] = 0x14578;
  rf.regs[0x08] = 0x1e742;
  rf.regs[0x09] = 0x00120;
  rf.regs[0x0a] = 0x00000;
  rf.regs[0x0b] = 0x00000;
  rf.regs[0x0c] = 0x00000;
  rf.regs[0x1b] = 0x0000f;
  rf.regs[0x0e] = 0x00022;
  rf.regs[0x1f] = 0x00001;

  txWritePos        = 0;    rxBufBegin      = 0;
  txWriteCounter    = 0;    rxBufEnd        = 0;
  txGapBegin        = 0;    rxWritePos      = 0;
  txGapSize         = 0;    rxWritePosLatch = 0;
  txTimOffset       = 0;    rxReadPos       = 0;
  txBeacon          = 0;    rxSoftReadPos   = 0;
  txMultiCmd        = 0;    rxReadCounter   = 0;
  txSlots[0]        = 0;    rxGapBegin      = 0;
  txSlots[1]        = 0;    rxGapSize       = 0;
  txSlots[2]        = 0;    reg00a          = 0;
  txStatControl     = 0;
  txRetryLimit      = 0;
  
  reg1a0l = 0;
  reg1a0m = 0;
  reg1a0n = 0;
  reg1a0h = 0;
  reg1a2  = 0;
  reg1a4  = 0;
  
  listenCount    = 0;
  listenInterval = 0;
  beaconInterval = 0;
  preamble       = 0;
  random         = 1;
  
  timer.enable     = false;
  timer.enableIrq  = false;
  timer.enableTxMP = false;
  timer.count      = 0;
  timer.compare    = 0;

  config120l        = 0;    config140  = 0;    config154l = 0;
  config120h        = 0;    config142  = 0;    config154m = 0;
  config122         = 0;    config144  = 0;    config154h = 0;
  config124         = 0;    config146  = 0;
  config128         = 0;    config148  = 0;    config0d4  = 0;
  config130         = 0;    config14a  = 0;    config0d8  = 0;
  config132l        = 0;    config14c  = 0;    config0da  = 0;
  config132h        = 0;    config150l = 0;    config0ecl = 0;
  configBeaconCount = 0;    config150h = 0;    config0ech = 0;
  
  stats.reg1b0      = 0;
  stats.reg1b2      = 0;
  stats.reg1b4      = 0;
  stats.reg1b6      = 0;
  stats.reg1b8      = 0;
  stats.reg1ba      = 0;
  stats.reg1bc      = 0;
  stats.reg1be      = 0;
  stats.txErrors    = 0;
  stats.rxPackets   = 0;
  memset(stats.mpErrors, 0, sizeof stats.mpErrors);
}

void WIFI::bbTransfer() {
  // Assuming 22MHz (66MHz/3) clock here. Probably wrong.
  bb.busy = 0;
  bb.read = 0;
  bb.time = 16 + 8;
  arm7.event.queue.add(3, bb.event);
}

void WIFI::bbTransferBit() {
  if(--bb.time) {
    // Small delay before setting busy bit
    if(bb.time == 20) bb.busy = 1;
    
    arm7.event.queue.add(3, bb.event);
    return;
  }
  // Finished, commit the read or write.
  bb.busy = 0;
  
  uint1 start = bb.header>>14;
  uint1 rd    = bb.header>>13;
  uint1 wr    = bb.header>>12;
  uint8 addr  = bb.header>>0;
  
  if(start) {
    if(rd) {
      bb.read = addr < 0x69? bb.regs[addr] : 0;
      //print("bb read ",hex<2>(addr)," = ",hex<2>(bb.read),"\n");
    }
    if(wr) {
      if(0x0d <= addr && addr <= 0x12) return;
      if(0x16 <= addr && addr <= 0x1a) return;
      if(0x5d <= addr && addr <= 0x61) return;
      if(0x00 == addr || 0x27 == addr) return;
      if(0x4d == addr || 0x64 == addr) return;
      if(0x66 == addr || 0x69 <= addr) return;
      
      if(addr < 0x69) bb.regs[addr] = bb.write;
      //print("bb write ",hex<2>(addr)," : ",hex<2>(bb.write),"\n");
    }
  }
}

void WIFI::rfTransfer() {
  rf.busy = 1;
  rf.time = 18 + 6;
  arm7.event.queue.add(3, rf.event);
}

void WIFI::rfTransferBit() {
  if(--rf.time) {
    arm7.event.queue.add(3, rf.event);
    return;
  }
  //Finished, commit the read or write.
  rf.busy = 0;
  
  uint1  rd   = rf.data>>23;
  uint5  addr = rf.data>>18;
  uint18 data = rf.data>>0;
  
  if(rd) {
    rf.data = rf.regs[addr];
    //print("rf read ",hex<2>(addr)," = ",hex<5>(rf.data),"\n");
  }
  else {
    rf.regs[addr] = data;
    //print("rf write ",hex<2>(addr)," : ",hex<5>(data),"\n");
  }
}

uint32 WIFI::read(uint32 addr, uint32 size) {
  if(addr & 0x4000)
    return system.wxram.read(addr & 0x1fff, size);
  
  if(size==Word) { return read(addr&~2, Half)<<0
                        | read(addr| 2, Half)<<16; }
  //if(addr != 0x480015e && addr != 0x4800180)
  //  print("wifi r ",hex<8>(addr),"\n");
  
  switch(addr & 0xffe) {
  case 0x000: return 0xc340;  // chip ID
  case 0x004: return reg004;
  case 0x006: return swMode<<0 | wifi.wepMode<<3;
  case 0x008: return txStatControl;
  case 0x00a: return reg00a;
  case 0x010: return interrupt.flags;
  case 0x012: return interrupt.enable;
  
  case 0x018: return macAddr[0] | macAddr[1]<<8;
  case 0x01a: return macAddr[2] | macAddr[3]<<8;
  case 0x01c: return macAddr[4] | macAddr[5]<<8;
  case 0x020: return bssId[0] | bssId[1]<<8;
  case 0x022: return bssId[2] | bssId[3]<<8;
  case 0x024: return bssId[4] | bssId[5]<<8;
  case 0x028: return assocIdl;
  case 0x02a: return assocIdf;
  case 0x02c: return txRetryLimit;
  
  case 0x030: return rxControl;
  case 0x032: return wepControl;
  case 0x034: return reg034;
  case 0x036: return bb.clock;
  case 0x038: return pm.txIdle;
  case 0x03c: return pm.wakeRequest | pm.wakePending<<8 | pm.sleeping<<9;
  case 0x040: return 0;//wifi.pm.;
  
  case 0x044: {
    uint11 data = random;
    random = random<<1 | random>>10;
    random ^= data & 1;
    return data;
  }
  
  case 0x050: return rxBufBegin;
  case 0x052: return rxBufEnd;
  case 0x054: return rxWritePos;
  case 0x056: return rxWritePosLatch;
  case 0x058: return rxReadPos<<1;
  case 0x05a: return rxSoftReadPos;
  case 0x05c: return rxReadCounter;
  case 0x060: break;//return rxBufRead();
  case 0x062: return rxGapBegin<<1;
  case 0x064: return rxGapSize;
  
  case 0x068: return txWritePos<<1;
  case 0x06c: return txWriteCounter;
  case 0x074: return txGapBegin<<1;
  case 0x076: return txGapSize;
  case 0x080: return txBeacon;
  case 0x084: return txTimOffset;
  case 0x088: return listenCount;
  case 0x08c: return beaconInterval;
  case 0x08e: return listenInterval;
  
  case 0x0bc: return preamble;
  
  case 0x0d4: return config0d4;
  case 0x0d8: return config0d8;
  case 0x0da: return config0da;
  case 0x0e8: return timer.enable;
  case 0x0ea: return timer.enableIrq;
  case 0x0ec: return config0ecl | config0ech<<8;
  case 0x0ee: return timer.enableTxMP;
  
  case 0x110: return preBeacon;
  
  case 0x120: return config120l | config120h<<15;
  case 0x122: return config122;
  case 0x124: return config124;
  case 0x128: return config128;
  case 0x130: return config130;
  case 0x132: return config132l | config132h<<15;
  case 0x134: return configBeaconCount;
  case 0x140: return config140;
  case 0x142: return config142;
  case 0x144: return config144;
  case 0x146: return config146;
  case 0x148: return config148;
  case 0x14a: return config14a;
  case 0x14c: return config14c;
  case 0x150: return config150l | config150h<<8;
  case 0x154: return config154l | config154m<<9 | config154h<<11;
  
  case 0x15c: return bb.read;
  case 0x15e: return bb.busy;
  case 0x160: return bb.mode8<<8 | bb.modeE<<14;
  case 0x168: return bb.powerl<<0 | bb.powerh<<15;
  
  case 0x17c: return rf.data>>16;
  case 0x17e: return rf.data>>0;
  case 0x180: return rf.busy;
  
  case 0x1a0: return reg1a0l | reg1a0m<<4 | reg1a0n<<8 | reg1a0h<<11;
  case 0x1a2: return reg1a2;
  case 0x1a4: return reg1a4;
  
  case 0x244: return 0x0000;
  case 0x254: return 0xeeee;
  case 0x290: return 0xffff;
  }
  //print("r ",hex<8>(addr),": unimplemented\n");
  return 0;
}

void WIFI::write(uint32 addr, uint32 size, uint32 data) {
  if(size==Byte) return;
  if(size==Word) { write(addr&~2, Half, data & 0xffff);
                   write(addr| 2, Half, data >> 16); return; }
  if(addr & 0x4000)
    return system.wxram.write(addr & 0x1fff, Half, size);
  
  //print("wifi w ",hex<8>(addr)," : ",hex<4>(data),"\n");
  
  switch(addr & 0xffe) {
  case 0x004:
    if((reg004 ^ data) & 1<<0) {
      if(data & 1) {
        reg034 = 0x2;
        //rf.pins = 0x46;
        //rf.status = 9;
        //reg27c = 5;
        //reg2a2 = ..;
      }
      else {
        reg034 = 0xa;
      }
    }
    if(data & 1<<13) {
      rxWritePosLatch = 0;
      //reg0c0 = 0;
      //reg0c4 = 0;
      //reg1a4 = 0;
      //reg278 = 0xf;
    }
    if(data & 1<<14) {
      swMode = 0;
      wepMode = 0;
      //txStatCnt = 0;
      //reg00a = 0;
      memset(macAddr, 0, sizeof macAddr);
      memset(bssId, 0, sizeof bssId);
      assocIdl = 0;
      assocIdf = 0;
      //txRetryLimit = 0;
      //reg02e = 0;
      rxBufBegin = 0x4000;
      rxBufEnd   = 0x4800;
      txTimOffset = 0;
      //reg0bc = 1;
      //reg0d0 = 0x401;
      config0d4 = 1;
      //reg0e0 = 8;
      config0ecl = 3;
      config0ech = 0x3f;
      //reg194 = 0;
      //reg198 = 0;
      //reg1a2 = 1;
      //reg224 = 3;
      //reg230 = 0x47;
    }
    reg004 = data;
    return;
    
  case 0x006: swMode = data>>0; wepMode = data>>3; return;
  case 0x008: txStatControl = data; return;
  case 0x00a: reg00a = data; return;
  case 0x21c: interrupt.flags |= data & ~0x400; return;
  case 0x010: interrupt.flags &= ~data; return;
  case 0x012: interrupt.enable = data; return;
  
  case 0x018: macAddr[0] = data; macAddr[1] = data>>8; return;
  case 0x01a: macAddr[2] = data; macAddr[3] = data>>8; return;
  case 0x01c: macAddr[4] = data; macAddr[5] = data>>8; return;
  case 0x020: bssId[0] = data;   bssId[1] = data>>8; return;
  case 0x022: bssId[2] = data;   bssId[3] = data>>8; return;
  case 0x024: bssId[4] = data;   bssId[5] = data>>8; return;
  case 0x028: assocIdl = data; return;
  case 0x02a: assocIdf = data; return;
  case 0x02c: txRetryLimit = data; return;
  
  case 0x030: rxControl = data; return;
  case 0x032: wepControl = data & 0x8000; return;
  case 0x034: reg034 = data; return;
  case 0x036: bb.clock = data; return;
  case 0x038: pm.txIdle = data; return;
  case 0x03c: pm.wakeRequest = data>>1; return;
  case 0x040: if(data & 1<<15) {
                reg034 = 2;
                pm.sleeping = data>>0;
              }
              return;
  
  case 0x050: rxBufBegin      = data; return;
  case 0x052: rxBufEnd        = data; return;
  case 0x056: rxWritePosLatch = data; return;
  case 0x058: rxReadPos       = data>>1; return;
  case 0x05a: rxSoftReadPos   = data; return;
  case 0x05c: rxReadCounter   = data; return;
  case 0x062: rxGapBegin      = data>>1; return;
  case 0x064: rxGapSize       = data; return;
  
  case 0x068: txWritePos      = data>>1; return;
  case 0x06c: txWriteCounter  = data; return;
  case 0x070: break;//return txBufWrite(data);
  case 0x074: txGapBegin      = data>>1; return;
  case 0x076: txGapSize       = data; return;
  
  case 0x080: txBeacon = data; return;
  case 0x084: txTimOffset = data; return;
  case 0x088: listenCount = data; return;
  case 0x08c: beaconInterval = data; return;
  case 0x08e: listenInterval = data; return;
  
  case 0x0b4: return; //txBufReset
  case 0x0bc: preamble = data; return;
  
  case 0x0d4: config0d4 = data; return;
  case 0x0d8: config0d8 = data; return;
  case 0x0da: config0da = data; return;
  case 0x0e8: timer.enable = data; return;
  case 0x0ec: config0ecl = data; config0ech = data>>8; return;
  case 0x0ea: timer.enableIrq = data; return;//if bit 1 trigger irq14
  case 0x0ee: timer.enableTxMP = data; return;
  
  case 0x110: preBeacon = data; return;
  
  case 0x120: config120l = data; config120h = data>>15; return;
  case 0x122: config122 = data; return;
  case 0x124: config124 = data; return;
  case 0x128: config128 = data; return;
  case 0x130: config130 = data; return;
  case 0x132: config132l = data; config132h = data>>15; return;
  case 0x134: configBeaconCount = data; return;
  case 0x140: config140 = data; return;
  case 0x142: config142 = data; return;
  case 0x144: config144 = data; return;
  case 0x146: config146 = data; return;
  case 0x148: config148 = data; return;
  case 0x14a: config14a = data; return;
  case 0x14c: config14c = data; return;
  case 0x150: config150l = data; config150h = data>>8; return;
  case 0x154: config154l = data; config154m = data>>9; config154h = data>>11; return;
  
  case 0x158: bb.header = data; return wifi.bbTransfer();
  case 0x15a: bb.write  = data; return;
  case 0x160: bb.mode8  = data>>8; bb.modeE = data>>14; return;
  case 0x168: bb.powerl = data>>0; bb.powerh = data>>15; return;
  
  case 0x17c: rf.data &= 0xffff; rf.data |= data<<16; return wifi.rfTransfer();
  case 0x17e: rf.data &= ~0xffff; rf.data |= data<<0; return;
  case 0x184: rf.length   = data>>0;
              rf.type     = data>>8;
              rf.reserved = data>>14; return;
  
  case 0x1a0: reg1a0l = data; reg1a0m = data>>4; reg1a0n = data>>8; reg1a0h = data>>11; return;
  case 0x1a2: reg1a2 = data; return;
  case 0x1a4: reg1a4 = data; return;
  
  case 0x244: return;
  case 0x254: return;
  case 0x290: return;
  }
  //print("w ",hex<8>(addr),": unimplemented\n");
}


}
