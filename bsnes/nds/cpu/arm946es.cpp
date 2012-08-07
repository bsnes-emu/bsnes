
ARM946ES::ARM946ES() {
  itcm.data = new uint32_t[(itcm.size = 0x8000)/4];
  dtcm.data = new uint32_t[(dtcm.size = 0x4000)/4];
  bios.data = new uint32[(bios.size = 0x001000)/4]();
  memset(bios.data, 0, bios.size);
}

void ARM946ES::Thread() { arm9.main(); }

void ARM946ES::power() {
  // 2 * 33513982 MHz
  if(thread) co_delete(thread);
  thread = co_create(262144 * sizeof(void*), ARM946ES::Thread);
  
  vectorBase     = 0xffff0000;
  bxWithLoadedPC = true;
  insnLatch      = 0;
  
  memset(itcm.data, 0, itcm.size);
  memset(dtcm.data, 0, dtcm.size);
  
  divMode     = 0;  rootMode   = 0;
  divByZero   = 0;  rootBusy   = 0;
  divBusy     = 0;  square     = 0;
  numerator   = 0;  squareRoot = 0;
  denominator = 0;
  quotient    = 0;
  remainder   = 0;
  
  booted      = 0;
  flag300     = 0;
  
  slot1access = 1;
  slot2access = 1;
  ramPriority = 1;
  
  CPUCore::power();
  trace = false;
  
  control.mmu      = false;
  control.dcache   = false;
  control.icache   = false;
  
  control.dtcm     = false;
  control.itcm     = false;
  control.dtcmLoad = false;
  control.itcmLoad = false;
  
  control.endian      = CR::little;
  control.cachePolicy = CR::random;
}

void ARM946ES::istep(unsigned clocks) {
  arm7.clock -= clocks;
  event.queue.step(clocks);
}

void ARM946ES::step(unsigned clocks) {
  return istep(clocks);
}



void ARM946ES::main() {
  itcmRegion = 0x00000000 | 14<<1; control.itcm = true;
  dtcmRegion = 0x00800000 | 14<<1; control.dtcm = true;
  updateTcm();
  
  for(;;) {
    if(arm7.clock < -256)
      co_switch(arm7.thread);
    
    event.irq = interrupt.gate && (interrupt.enable & interrupt.flags);
    //if(event.irq && !If) {
    //  print("arm9: irq (if=",hex<8>(interrupt.flags),")\n");
    //}
    
    if(Tf) execTHUMB();
    else   execARM();
  }
}



void ARM946ES::execARM() {
  if(branched) {
    //if(r[15] == 0) print(hex<8>(r[14]), ": jump to nullptr!\n");
    branched = false;                 r[15] &= ~3;
    iexecute = fetch(r[15], Word, 0); r[15] += 4;
    idecode  = fetch(r[15], Word, 0); r[15] += 4;
  } else {
    iexecute = idecode, idecode = ifetch;
  }
  ifetch = fetch(r[15], Word, 1);
  uint32 i = iexecute;
  if(event.irq && !If) return irq();
  //if(trace) traceInsn();
  
  if(i < 0xe0000000 && !evalCond(i>>28)) { r[15] += 4; return; }
  
  //                                                    opcode, sh,      Rd,  Rn,  Rm,  Rs
  if(imatch("1111......../////")) {
    if(imatch("111101x1u101/////")) { r[15] += 4; return; } // pld - reportedly NOP on DS?
    if(imatch("1111101l..../////")) return armBranch  (1, 1, i<<2 | (i>>23 & 2));
    return undefined();
  }
  if(imatch("00x10xx0/////")) {
    if(imatch("00110r10/// ..../")) return armWritePsr(i>>22,              i>>16, armImmed(i, i>>8));
    if(imatch("00010r10/// 0000/")) return armWritePsr(i>>22,              i>>16, armRm(i));
    if(imatch("00010r00/// 0000/")) return armReadPsr (i>>22,       i>>12);
    if(imatch("00010010/// 00l1/")) return armBranchEx(       i>>5,               i);
    if(imatch("00010010/// 0111/")) return pfabort    (); // bkpt
    if(imatch("00010110/// 0001/")) return armClz     (             i>>12,        i);
    if(imatch("00010ds0/// 0101/")) return armDspAdd  (i>>21,       i>>12, i>>16, i);
    if(imatch("00010oo0/// 1yx0/")) return armDspMul  (i>>21, i>>5, i>>16, i>>12, i, i>>8);
    if(imatch("00010b00/// 1001/")) return armMemSwap (i>>22,       i>>12, i>>16, i);
  }
  if(imatch("000...../// 1..1/")) {
    if(imatch("0000luas/// 1001/")) return armMultiply(i>>20,       i>>16, i>>12, i, i>>8);
    if(imatch("000pu0w0/// 11s1/")) return armMem_v5  (i>>20, i>>5, i>>12, i>>16, armRm(i));
    if(imatch("000pu0wl/// 1sh1/")) return armMem_v4  (i>>20, i>>5, i>>12, i>>16, armRm(i));
    if(imatch("000pu1w0/// 11s1/")) return armMem_v5  (i>>20, i>>5, i>>12, i>>16, armOffset8(i, i>>8));
    if(imatch("000pu1wl/// 1sh1/")) return armMem_v4  (i>>20, i>>5, i>>12, i>>16, armOffset8(i, i>>8));
  }
  if(imatch("001aaaas/// ..../")) return armData    (i>>20,       i>>12, i>>16, armImmed(i, i>>8));
  if(imatch("000aaaas/// .sh0/")) return armData    (i>>20,       i>>12, i>>16, shiftImm(i, i>>5, i>>7));
  if(imatch("010pubwl/// ..../")) return armMem     (i>>20,       i>>12, i>>16, armOffset12(i));
  if(imatch("011pubwl/// ...0/")) return armMem     (i>>20,       i>>12, i>>16, shiftImm(i, i>>5, i>>7));
  if(imatch("100puswl/// ..../")) return armBlock   (i>>20,              i>>16, i);
  if(imatch("101l..../// ..../")) return armBranch  (i>>24,                  0, i<<2);
  if(imatch("000aaaas/// 0sh1/")) return armDataRs  (i>>20,       i>>12, i>>16, i, i>>5, i>>8);
  
  if(imatch("1110..../// ..../")) return dataCop    (i>>8, i>>20, i>>12, i>>16, i, i>>4);
  if(imatch("1111..../// ..../")) return swi();
  return undefined();
}

void ARM946ES::execTHUMB() {
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
  //if(trace) traceInsn();
  
  if(imatch("00011ismmmnnnddd")) return thumbAddSub  (i>>9,  i, i>>3, i>>6);
  if(imatch("000ssiiiiimmmddd")) return thumbShiftImm(i>>11, i, i>>3, i>>6);
  if(imatch("001oodddiiiiiiii")) return thumbDataImm (i>>11, i>>8, i);
  if(imatch("010000oooommmddd")) return thumbDataLo  (i>>6,  i, i>>3);
  if(imatch("010001oodmmmmddd")) return thumbDataHi  (i>>8, (i&7)+(i>>4&8), i>>3);
  
  if(imatch("0101ooommmnnnddd")) return thumbMemReg  (i>>9,  i, i>>3, i>>6);
  if(imatch("011bliiiiinnnddd")) return thumbMemImm  (i>>11, i, i>>3, i>>6);
  if(imatch("1000liiiiinnnddd")) return thumbMemImm  (i>>11, i, i>>3, i>>6);
  
  if(imatch("10111110........")) return pfabort(); // bkpt
  if(imatch("10110000siiiiiii")) return thumbAddSP   (       i>>7, i);
  if(imatch("01001dddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  if(imatch("1001odddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  if(imatch("1010odddrrrrrrrr")) return thumbRelative(i>>11, i>>8, i);
  
  if(imatch("1101ccccrrrrrrrr")) return thumbCond    (       i>>8, i);
  if(imatch("11100rrrrrrrrrrr")) return thumbBranch  (i);
  if(imatch("11110rrrrrrrrrrr")) return thumbBh      (i);
  if(imatch("111t1rrrrrrrrrrr")) return thumbBlx     (i>>12, i);

  if(imatch("1o..lnnnrrrrrrrr")) return thumbBlock   (i>>11, i>>8, i);
  
  return undefined();
}



uint32 ARM946ES::fetch(uint32 addr, uint32 size, bool s) {
  istep(1);  // Assume cached for now
  
  if(itcmRCompare == (itcmRMask & addr)) { return itcm.read(addr % 0x8000, size); }
  /* DTCM is not executable */
  
  switch(addr >> 24) {
  case 0x2: return system.ewram.read(addr % 0x400000, size);
  case 0x3: return system.swram[addr>>14 & 1].read(addr % 0x4000, size);
  case 0x6: return system.vmap.arm9[addr>>21 & 7][addr>>14 & 63].read(addr, size);
  }
  if(addr >= 0xffff0000) return bios.read(addr & bios.size-1, size);
  return 0;
}

uint32 ARM946ES::read(uint32 addr, uint32 size, bool s) {
  if(itcmRCompare == (itcmRMask & addr)) { istep(1); return itcm.read(addr % 0x8000, size); }
  if(dtcmRCompare == (dtcmRMask & addr)) {           return dtcm.read(addr % 0x4000, size); }
  
  const int w = 6*!s + 2;                    // 32-bit bus timing
  const int h = 6*!s + (size==Word? 4 : 2);  // 16-bit bus timing
  //const int e = 2*!s + h;                    // EWRAM timing
  
  //if((addr & 0xf3ff000) == 0x23ff000) {
  //  istep(e);
  //  uint32 data = system.ewram.read(addr % 0x400000, size);
  //  print(hex<8>(arm9.event.queue.time), " ",hex<8>(r[15])," arm9: r ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
  //  return data;
  //}
  
  switch(addr >> 24) {
  case 0x2: istep(3); return system.ewram.read(addr % 0x400000, size);
  case 0x3: istep(w); return system.swram[addr>>14 & 1].read(addr % 0x4000, size);
  case 0x4: istep(w); { //return readReg(addr, size); {
      uint32 data = readReg(addr, size);
      //if(addr != 0x40001a0 && addr != 0x40001a4 && addr != 0x4100010)
      //  print(hex<8>(arm9.event.queue.time), " ",hex<8>(r[15])," arm9: r ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
      return data;
    }
  case 0x5: istep(h); return ppu[addr>>10 & 1].readPalette(addr % 0x400);
  case 0x6: istep(h); return system.vmap.arm9[addr>>21 & 7][addr>>14 & 63].read(addr, size);
  case 0x7: istep(w); return ppu[addr>>10 & 1].readOam(addr % 0x400);
  }
  istep(w); if(addr >= 0xffff0000) return bios.read(addr & bios.size-1, size);
  return 0;
}

void ARM946ES::write(uint32 addr, uint32 size, bool s, uint32 data) {
  if(itcmWCompare == (itcmWMask & addr)) { istep(1); return itcm.write(addr % 0x8000, size, data); }
  if(dtcmWCompare == (dtcmWMask & addr)) {           return dtcm.write(addr % 0x4000, size, data); }
  
  //if((addr & 0xf3ff000) == 0x23ff000)
  //  print(hex<8>(arm9.event.queue.time), " ",hex<8>(r[15]), " arm9: w ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
  
  const int w = 6*!s + 2;                    // 32-bit bus timing
  const int h = 6*!s + (size==Word? 4 : 2);  // 16-bit bus timing
  //const int e = 2*!s + h;                    // EWRAM timing
  
  switch(addr >> 24) {
  case 0x2: istep(3); return system.ewram.write(addr % 0x400000, size, data);
  case 0x3: istep(w); return system.swram[addr>>16 & 1].write(addr % 0x4000, size, data);
  case 0x4: istep(w); { //return writeReg(addr, size, data);
      //if(addr != 0x04000400)
      //  print(hex<8>(arm9.event.queue.time), " ", hex<8>(r[15])," arm9: w ",hex<8>(addr),":",size," = ",hex<8>(data),"\n");
      writeReg(addr, size, data);
      return;
    }
  case 0x5: istep(h); return ppu[addr>>10 & 1].writePalette(addr % 0x400, size, data);
  case 0x6: istep(h); return system.vmap.arm9[addr>>21 & 7][addr>>14 & 63].write(addr, size, data);
  case 0x7: istep(w); return ppu[addr>>10 & 1].writeOam(addr % 0x400, size, data);
  }
  istep(w);
}

void ARM946ES::dataCop(uint4 cpno, uint4 op1, uint4 ird, uint4 irn, uint4 irm, uint4 op2) {
  if(cpno == 15) {
    auto  &rd = r[ird];
    uint32 rm = r[irm];
    r[15] += 4;
    
    if(op2 & 1) { // MRC, MCR
      switch(op1<<12 | irn<<8 | irm<<4 | op2>>1) {
        
      case 0x1000: // c0,c0,0  Processor ID
        rd  =   'A'<<24;  // Implementor  ARM
        rd |=     0<<20;  // Variant      0
        rd |=     5<<16;  // Architecture v5
        rd |= 0x946<< 4;  // Model        946
        rd |=     1<< 0;  // Revision     1
        return;
        
      case 0x1001: // c0,c0,1  Cache information
        rd  =  0x0f<<24;  // type B Harvard
        rd |= 0x0d2<<12;  // 4K data, 4-way, 32-byte lines
        rd |= 0x112<< 0;  // 8K insn, 4-way, 32-byte lines
        return;
        
      case 0x1002: // c0,c0,2  TCM information
        rd  = 5<<18;  // 16K dtcm
        rd |= 6<< 6;  // 32K itcm
        return;
        
      case 0x1100: // c1,c0,0  Control register
        rd = control.mmu<<0 | control.dcache<<2 | 1<<3 /*write buffer*/
           | 1<<4 | 1<<5 | 1<<6       /*32 bit only, late abort model*/
           | control.endian<<7 | control.icache<<12 | !!vectorBase<<13
           | control.cachePolicy<<14 | !bxWithLoadedPC<<15
           | control.dtcm<<16 | control.dtcmLoad<<17
           | control.itcm<<18 | control.itcmLoad<<19;
        return;
        
      case 0x0100:
        control.mmu         = rd >> 0;
        control.dcache      = rd >> 1;
        control.endian      = rd >> 7;
        control.icache      = rd >> 12;
        vectorBase          = rd & 1<<13? 0xffff0000 : 0;
        control.cachePolicy = rd >> 14;
        bxWithLoadedPC      = rd & 1<<15? false : true;
        control.dtcm        = rd >> 16;
        control.dtcmLoad    = rd >> 17;
        control.itcm        = rd >> 18;
        control.itcmLoad    = rd >> 19;
        
        updateTcm();
        return;
        
      case 0x0900: case 0x1900: // c9,c0,0  Lock dcache
      case 0x0901: case 0x1901: // c9,c0,1  Lock icache
        return;
        
      case 0x0910: // c9,c1,0  DTCM mapping
        dtcmRegion = rd;
        updateTcm();
        return;
      case 0x1910:
        rd = dtcmRegion;
        return;
        
      case 0x0911: // c9,c1,1  ITCM mapping
        itcmRegion = rd;
        updateTcm();
        return;
      case 0x1911:
        rd = itcmRegion;
        return;
        
      case 0x0200: case 0x1200: // c2,c0,0  Region data cache bits
      case 0x0201: case 0x1201: // c2,c0,1  Region insn cache bits
      case 0x0300: case 0x1300: // c3,c0,0  Region data buffer bits
      case 0x0500: case 0x1500: // c5,c0,0  Region data permissions
      case 0x0501: case 0x1501: // c5,c0,1  Region insn permissions
      case 0x0502: case 0x1502: // c5,c0,2  Region data permissions - extended
      case 0x0503: case 0x1503: // c5,c0,3  Region insn permissions - extended
        return;
        
      case 0x0600: case 0x1600: case 0x0601: case 0x1601: // c6,cN,0  Region mappings
      case 0x0610: case 0x1610: case 0x0611: case 0x1611: 
      case 0x0620: case 0x1620: case 0x0621: case 0x1621: 
      case 0x0630: case 0x1630: case 0x0631: case 0x1631: 
      case 0x0640: case 0x1640: case 0x0641: case 0x1641: 
      case 0x0650: case 0x1650: case 0x0651: case 0x1651: 
      case 0x0660: case 0x1660: case 0x0661: case 0x1661: 
      case 0x0670: case 0x1670: case 0x0671: case 0x1671: 
        return;
        
      case 0x0704: // c7,c0,4  Wait for Interrupt
      case 0x0782: // c7,c8,2    (alternate version)
        //print("arm9: wait (ime=",interrupt.gate,", ie=",hex<8>(interrupt.enable),")\n");
        powerState = waiting;
        for(;;) {
          // Because this one is integrated with the ARM9,
          // it's "behind" the IRQ line and knows nothing about IME/IE/IF.
          if(interrupt.gate && (interrupt.enable & interrupt.flags)) {
            powerState = running;
            break;
          }
          istep(256);
          if(arm7.clock < -256)
            co_switch(arm7.thread);
        }
        return;
        
      case 0x0750: // c7, c5,0  Invalidate icache entirely
      case 0x0751: // c7, c5,1  Invalidate icache by address
      case 0x0752: // c7, c5,2  Invalidate icache by line (?)
      case 0x07d1: // c7,c13,1  Preload icache by address
        return;
        
      case 0x0760: // c7, c6,0  Invalidate dcache entirely
      case 0x0761: // c7, c6,1  Invalidate dcache by address
      case 0x0762: // c7, c6,2  Invalidate dcache by line (? zelda gallery uses this)
      
      //case 0x07a0://c7,c10,0  Clean dcache entirely (?)
      case 0x07a1: // c7,c10,1  Clean dcache by address
      case 0x07a2: // c7,c10,2  Clean dcache by line
      case 0x07a4: // c7,c10,4  Drain write buffer
      
      //case 0x07e0://c7,c14,0  Flush dcache entirely (?)
      case 0x07e1: // c7,c14,1  Flush dcache by address
      case 0x07e2: // c7,c14,2  Flush dcache by line
        return;
        
      case 0x0f00: case 0x1f00: // 0,c15,c0,0  Cache control
      case 0x6f00: case 0x7f00: // 3,c15,c0,0  Cache tag/data index
      case 0x6f01: case 0x7f01: // 3,c15,c0,1  R/W icache tag
      case 0x6f02: case 0x7f02: // 3,c15,c0,2  R/W dcache tag
      case 0x6f03: case 0x7f03: // 3,c15,c0,3  R/W icache data
      case 0x6f04: case 0x7f04: // 3,c15,c0,4  R/W dcache data
        return;
        
      }
    }
  }
  bool iscdp = op2&1, isld = op1&1;
  print("arm9: ",hex<8>(r[15])," undefined ", iscdp? "cdp" : isld? "mrc" : "mcr", " p",cpno,", ",
    iscdp? (unsigned)op1 : op1>>1, ", ", iscdp? "cr":"r", ird, ", cr", irn, ", cr", irm, ", ", op2>>1, "\n");
  return undefined();
}



uint32 ARM946ES::readReg(uint32 addr, uint32 size) {
  switch(addr-0x4000000 & ~3) {
  
  // These registers shouldn't be here - but they contain bits from logically
  // separate hardware units, so it's not clear where else to put them.
  
  case 0x0000: // Display 0 / PPU0 BG, OBJ control
    return (video.frameBuffer<<18 & 0xc0000)
         | video.source[0]<<16 | ppu[0].regControl();
    
  case 0x1000: // Display 1 / PPU1 BG, OBJ control
    return video.source[1]<<16 | ppu[1].regControl();
    
  case 0x0008: case 0x1008: // BG0,BG1
  case 0x000c: case 0x100c: // BG2,BG3
    return ppu[addr>>12 & 1].regBg((addr>>1 & 2) + 0)<<0
         | ppu[addr>>12 & 1].regBg((addr>>1 & 2) + 1)<<16;
    
  case 0x0010: case 0x1010: // BG0H,V
  case 0x0014: case 0x1014: // BG1H,V
  case 0x0018: case 0x1018: // BG2H,V
  case 0x001c: case 0x101c: // BG3H,V
    return ppu[addr>>12 & 1].regBgOffs(addr>>2 & 3);
  
  case 0x0048: case 0x1048: // Window area 0,1,out,obj
    return ppu[addr>>12 & 1].regWinArea();
    
  case 0x0050: case 0x1050: // Blend control
    return ppu[addr>>12 & 1].regBlend();
  
  // Miscellaneous graphics registers
  case 0x0060: return gpu.regRenderOptions();
  case 0x0064: return video.regCapture();
  case 0x006c: return video.regBrightness(0);
    
  case 0x1060: return 0;
  case 0x1064: return 0;
  case 0x1068: return 0;  
  case 0x106c: return video.regBrightness(1);
  
  // Memory control
  case 0x0204: return ramPriority<<15 | slot1access<<11
                    | slot2access<<7 | regSlot2Control();
  
  case 0x0240: return system.regVmap(0);
  case 0x0244: return system.regVmap(1);
  case 0x0248: return system.regVmap(2);
  
  // Math
  case 0x0280: return regDivideControl();
  case 0x0290: return regNumerator(0);
  case 0x0294: return regNumerator(1);
  case 0x0298: return regDenominator(0);
  case 0x029c: return regDenominator(1);
  case 0x02a0: return regQuotient(0);
  case 0x02a4: return regQuotient(1);
  case 0x02a8: return regRemainder(0);
  case 0x02ac: return regRemainder(1);
  
  case 0x02b0: return regSquareRootControl();
  case 0x02b4: return regSquareRoot();
  case 0x02b8: return regSquare(0);
  case 0x02bc: return regSquare(1);
  
  case 0x0300: return booted<<0 | flag300<<1;
  
  // Power
  case 0x0304:
    return video.screensPowered<< 0 | ppu[0].powered<<1
         | video.ppu0Screen    <<15 | ppu[1].powered<<9;
    
  // Render status
  case 0x0320: return gpu.regRenderLoad();
  
  // This shouldn't exist, but various games read it nonetheless..
  case 0x04a4: return 0;
  
  // Geometry status
  case 0x0600: return gpu.regGeomStatus();
  case 0x0604: return gpu.regGeomLoad();
  
  case 0x0620: case 0x0624: case 0x0628: case 0x062c:
    return gpu.regGeomPosition((addr - 0x4000620)/4);
    
  case 0x0630: case 0x0634:
    return gpu.regGeomNormal((addr - 0x4000630)/4);
    
  case 0x0640: case 0x0644: case 0x0648: case 0x064c:
  case 0x0650: case 0x0654: case 0x0658: case 0x065c:
  case 0x0660: case 0x0664: case 0x0668: case 0x066c:
  case 0x0670: case 0x0674: case 0x0678: case 0x067c:
    return gpu.regClipMatrix((addr - 0x4000640)/4);
    
  case 0x0680: case 0x0684: case 0x0688:
  case 0x068c: case 0x0690: case 0x0694:
  case 0x0698: case 0x069c: case 0x06a0:
    return gpu.regLightMatrix((addr - 0x4000680)/4);
    
  }
  return CPUCore::readReg(addr, size);
}

void ARM946ES::writeReg(uint32 addr, uint32 size, uint32 data) {
  uint32 mask = 0xffffffff;
  if(size == Half) mask = 0xffff << 8*(addr & 2);
  if(size == Byte) mask = 0xff << 8*(addr & 3);
  
  data &= mask;
  
  switch(addr-0x4000000 & ~3) {
  
  case 0x0000: // Display 0 / PPU0 control
    if(mask & 0x000f0000) {
      // These bits go to the display controller - not PPU0.
      video.frameBuffer &= ~0xc;
      video.frameBuffer |=  0xc & data>>16;
      video.source[0]    =  0x3 & data>>16;
    }
    ppu[0].regControl(data, mask);
    return;
    
  case 0x1000: // Display 1 / PPU1 control
    if(mask & 0x000f0000) {
      // Framebuffer/FIFO sources aren't supported here.
      video.source[1]    =  0x1 & data>>16;
    }
    ppu[1].regControl(data, mask);
    return;
    
  case 0x0008: case 0x1008: // BGn control
  case 0x000c: case 0x100c: //
    if(mask & 0x0000ffff) ppu[addr>>12 & 1].regBg((addr>>1 & 2) + 0, data>>0,  mask>>0);
    if(mask & 0xffff0000) ppu[addr>>12 & 1].regBg((addr>>1 & 2) + 1, data>>16, mask>>16);
    return;
    
  case 0x0010: case 0x1010: // BGn H,V scroll
  case 0x0014: case 0x1014: //
  case 0x0018: case 0x1018: //
  case 0x001c: case 0x101c: return ppu[addr>>12 & 1].regBgOffs(addr>>2 & 3, data, mask);
    
  case 0x0020: case 0x1020: // BG2 affine A, B, C, D
  case 0x0024: case 0x1024: // 
  case 0x0028: case 0x1028: // BG2 origin X, Y
  case 0x002c: case 0x102c: return ppu[addr>>12 & 1].regBgAffine(2, addr>>2 & 3, data, mask);
    
  case 0x0030: case 0x1030: // BG3 affine A, B, C, D
  case 0x0034: case 0x1034: //
  case 0x0038: case 0x1038: // BG3 origin X, Y
  case 0x003c: case 0x103c: return ppu[addr>>12 & 1].regBgAffine(3, addr>>2 & 3, data, mask);
    
  case 0x0040: case 0x1040: return ppu[addr>>12 & 1].regWinDims(0, data, mask); // Window 0,1 X range
  case 0x0044: case 0x1044: return ppu[addr>>12 & 1].regWinDims(1, data, mask); //        0,1 Y range
  case 0x0048: case 0x1048: return ppu[addr>>12 & 1].regWinArea(data, mask);    //   area 0,1,out,obj
  case 0x004c: case 0x104c: return; // Mosaic BG,OBJ X,Y
  
  case 0x0050: case 0x1050: // Blend control
  case 0x0054: case 0x1054: return ppu[addr>>12 & 1].regBlend(addr>>2 & 1, data, mask);
    
  case 0x0058: case 0x1058: return; // not present, but frequently zeroed anyway
  case 0x005c: case 0x105c: return; //
  
  // Some miscellaneous registers, none of which belong here... hunh.
  case 0x0060: return gpu.regRenderOptions(data, mask);
  case 0x0064: return video.regCapture(data, mask);
  case 0x0068: return video.regFifo(data);
  case 0x006c: return video.regBrightness(0, data, mask);
    
  case 0x1060: return;
  case 0x1064: return;
  case 0x1068: return;  
  case 0x106c: return video.regBrightness(1, data, mask);
    
  // Memory control
  case 0x0204:
    if(mask & 0x00ff) {
      slot2access = !(data & 1<<7);
    }
    if(mask & 0xff00) {
      slot1access = !(data & 1<<11);
      ramPriority = !(data & 1<<15);
    }
    return regSlot2Control(data, mask);
    
  case 0x0240: return system.regVmap(0, data, mask);
  case 0x0244: return system.regVmap(1, data, mask);
  case 0x0248: return system.regVmap(2, data, mask);
  
  // Math
  case 0x0280: return regDivideControl(data, mask);
  case 0x0290: return regNumerator(0, data, mask);
  case 0x0294: return regNumerator(1, data, mask);
  case 0x0298: return regDenominator(0, data, mask);
  case 0x029c: return regDenominator(1, data, mask);
  
  case 0x02b0: return regSquareRootControl(data, mask);
  case 0x02b8: return regSquare(0, data, mask);
  case 0x02bc: return regSquare(1, data, mask);
  
  case 0x0300:
    if(mask & 0xff) {
      booted |= data>>0;
      flag300 = data>>1;
    }
    return;
  
  case 0x0304: // Power control
    if(mask & 0x00ff) {
      video.screensPowered = data>>0;
      ppu[0].powered       = data>>1;
      //gpu.renderPowered  = data>>2;
      //gpu.geomPowered    = data>>3;
    }
    if(mask & 0xff00) {
      ppu[1].powered       = data>>9;
      video.ppu0Screen     = data>>15;
    }
    return;
    
  // Toon edge table
  case 0x0330: case 0x0334: case 0x0338: case 0x033c:
    return gpu.regRenderEdgeTable((addr - 0x4000330)/4, data, mask);
    
  // Misc
  case 0x0340: return gpu.regRenderMinAlpha(data, mask);
  case 0x0350: return gpu.regRenderClearColor(data, mask);
  case 0x0354: return gpu.regRenderClearCoord(data, mask);
  case 0x0358: return gpu.regRenderFogColor(data, mask);
  case 0x035c: return gpu.regRenderFogCoord(data, mask);
    
  // Fog table
  case 0x0360: case 0x0364: case 0x0368: case 0x036c:
  case 0x0370: case 0x0374: case 0x0378: case 0x037c:
    return gpu.regRenderFogTable((addr - 0x4000360)/4, data, mask);
    
  // Toon shade table
  case 0x0380: case 0x0384: case 0x0388: case 0x038c:
  case 0x0390: case 0x0394: case 0x0398: case 0x039c:
  case 0x03a0: case 0x03a4: case 0x03a8: case 0x03ac:
  case 0x03b0: case 0x03b4: case 0x03b8: case 0x03bc:
    return gpu.regRenderToonTable((addr - 0x4000380)/4, data, mask);
  
  // GPU command pipe - buffered - mirrored to support STM
  case 0x0400: case 0x0404: case 0x0408: case 0x040c: case 0x0410: case 0x0414: case 0x0418: case 0x041c:
  case 0x0420: case 0x0424: case 0x0428: case 0x042c: case 0x0430: case 0x0434: case 0x0438: case 0x043c:
    return gpu.sendGeomBuffered(data);
    
  // GPU command pipe - immediate - the address itself is used as command number.
  case 0x0440: case 0x0444: case 0x0448: case 0x044c: case 0x0450: case 0x0454: case 0x0458: case 0x045c:
  case 0x0460: case 0x0464: case 0x0468: case 0x046c: case 0x0470: case 0x0474: case 0x0478: case 0x047c:
  case 0x0480: case 0x0484: case 0x0488: case 0x048c: case 0x0490: case 0x0494: case 0x0498: case 0x049c:
  case 0x04a0: case 0x04a4: case 0x04a8: case 0x04ac: case 0x04b0: case 0x04b4: case 0x04b8: case 0x04bc:
  case 0x04c0: case 0x04c4: case 0x04c8: case 0x04cc: case 0x04d0: case 0x04d4: case 0x04d8: case 0x04dc:
  case 0x04e0: case 0x04e4: case 0x04e8: case 0x04ec: case 0x04f0: case 0x04f4: case 0x04f8: case 0x04fc:
  case 0x0500: case 0x0504: case 0x0508: case 0x050c: case 0x0510: case 0x0514: case 0x0518: case 0x051c:
  case 0x0520: case 0x0524: case 0x0528: case 0x052c: case 0x0530: case 0x0534: case 0x0538: case 0x053c:
  case 0x0540: case 0x0544: case 0x0548: case 0x054c: case 0x0550: case 0x0554: case 0x0558: case 0x055c:
  case 0x0560: case 0x0564: case 0x0568: case 0x056c: case 0x0570: case 0x0574: case 0x0578: case 0x057c:
  case 0x0580: case 0x0584: case 0x0588: case 0x058c: case 0x0590: case 0x0594: case 0x0598: case 0x059c:
  case 0x05a0: case 0x05a4: case 0x05a8: case 0x05ac: case 0x05b0: case 0x05b4: case 0x05b8: case 0x05bc:
  case 0x05c0: case 0x05c4: case 0x05c8: case 0x05cc: case 0x05d0: case 0x05d4: case 0x05d8: case 0x05dc:
  case 0x05e0: case 0x05e4: case 0x05e8: case 0x05ec: case 0x05f0: case 0x05f4: case 0x05f8: case 0x05fc:
    return gpu.sendGeomImmediate(addr>>2 & 0x7f, data);
  
  // Geometry engine
  case 0x0600: return gpu.regGeomStatus(data, mask);
  case 0x0610: return gpu.regGeomMaxPointDepth(data, mask);
  
  // zelda gallery writes 0x2468ace0 here - ???
  case 0x0640: break;
  }
  return CPUCore::writeReg(addr, size, data);
}


void ARM946ES::updateTcm() {
  uint32 itcmAddr = 0,                   itcmSize = itcmRegion>>1 & 0x1f;
  uint32 dtcmAddr = dtcmRegion & ~0xfff, dtcmSize = dtcmRegion>>1 & 0x1f;
  
  // Disable by default
  itcmRCompare = itcmWCompare = ~0;
  dtcmRCompare = dtcmWCompare = ~0;
  itcmRMask    = itcmWMask    = 0;
  dtcmRMask    = dtcmWMask    = 0;
  
  // Enable bit enables access; load bit disables reading
  if(control.itcm)                      itcmWCompare = itcmAddr,  itcmWMask = -1 << 9+itcmSize;
  if(control.dtcm)                      dtcmWCompare = dtcmAddr,  dtcmWMask = -1 << 9+dtcmSize;
  if(control.itcm && !control.itcmLoad) itcmRCompare = itcmAddr,  itcmRMask = -1 << 9+itcmSize;
  if(control.dtcm && !control.dtcmLoad) dtcmRCompare = dtcmAddr,  dtcmRMask = -1 << 9+dtcmSize;
}



#include "math.cpp"
