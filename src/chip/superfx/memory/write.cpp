void SuperFX::mmio_write(uint16 addr, uint8 data) {
  switch(addr) {
  case 0x3000: regs.r0.l  = data; return;
  case 0x3001: regs.r0.h  = data; return;
  case 0x3002: regs.r1.l  = data; return;
  case 0x3003: regs.r1.h  = data; return;
  case 0x3004: regs.r2.l  = data; return;
  case 0x3005: regs.r2.h  = data; return;
  case 0x3006: regs.r3.l  = data; return;
  case 0x3007: regs.r3.h  = data; return;
  case 0x3008: regs.r4.l  = data; return;
  case 0x3009: regs.r4.h  = data; return;
  case 0x300a: regs.r5.l  = data; return;
  case 0x300b: regs.r5.h  = data; return;
  case 0x300c: regs.r6.l  = data; return;
  case 0x300d: regs.r6.h  = data; return;
  case 0x300e: regs.r7.l  = data; return;
  case 0x300f: regs.r7.h  = data; return;

  case 0x3010: regs.r8.l  = data; return;
  case 0x3011: regs.r8.h  = data; return;
  case 0x3012: regs.r9.l  = data; return;
  case 0x3013: regs.r9.h  = data; return;
  case 0x3014: regs.r10.l = data; return;
  case 0x3015: regs.r10.h = data; return;
  case 0x3016: regs.r11.l = data; return;
  case 0x3017: regs.r11.h = data; return;
  case 0x3018: regs.r12.l = data; return;
  case 0x3019: regs.r12.h = data; return;
  case 0x301a: regs.r13.l = data; return;
  case 0x301b: regs.r13.h = data; return;
  case 0x301c: regs.r14.l = data; return;
  case 0x301d: regs.r14.h = data; return;
  case 0x301e: regs.r15.l = data; return;
  case 0x301f: regs.r15.h = data; return;

//0x3020 - 0x302f unused

  case 0x3030: regs.sfr.l = data & 0x7e; return; //mask invalid bits
  case 0x3031: regs.sfr.h = data & 0x9f; return; //mask invalid bits
  case 0x3032: return; //unused
  case 0x3033: regs.bramr = data; return;
  case 0x3034: regs.pbr = data; return;
  case 0x3035: return; //unused
  case 0x3036: return; //ROMBR (read only)
  case 0x3037: regs.cfgr = data; return;
  case 0x3038: regs.scbr = data; return;
  case 0x3039: regs.clsr = data; return;
  case 0x303a: regs.scmr = data; return;
  case 0x303b: return; //VCR (read only)
  case 0x303c: return; //RAMBR (read only)
  case 0x303d: return; //unused
  case 0x303e: return; //CBR low (read only)
  case 0x303f: return; //CBR high (read only)

//0x3040 - 0x30ff unused
  }

  if(addr >= 0x3100 && addr <= 0x32ff) {
    cache[addr - 0x3100] = data;
    return;
  }
}
