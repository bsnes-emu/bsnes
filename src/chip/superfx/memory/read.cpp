uint8 SuperFX::mmio_read(uint addr) {
  addr &= 0xffff;

  switch(addr) {
  case 0x3000: return regs.r0.l;
  case 0x3001: return regs.r0.h;
  case 0x3002: return regs.r1.l;
  case 0x3003: return regs.r1.h;
  case 0x3004: return regs.r2.l;
  case 0x3005: return regs.r2.h;
  case 0x3006: return regs.r3.l;
  case 0x3007: return regs.r3.h;
  case 0x3008: return regs.r4.l;
  case 0x3009: return regs.r4.h;
  case 0x300a: return regs.r5.l;
  case 0x300b: return regs.r5.h;
  case 0x300c: return regs.r6.l;
  case 0x300d: return regs.r6.h;
  case 0x300e: return regs.r7.l;
  case 0x300f: return regs.r7.h;

  case 0x3010: return regs.r8.l;
  case 0x3011: return regs.r8.h;
  case 0x3012: return regs.r9.l;
  case 0x3013: return regs.r9.h;
  case 0x3014: return regs.r10.l;
  case 0x3015: return regs.r10.h;
  case 0x3016: return regs.r11.l;
  case 0x3017: return regs.r11.h;
  case 0x3018: return regs.r12.l;
  case 0x3019: return regs.r12.h;
  case 0x301a: return regs.r13.l;
  case 0x301b: return regs.r13.h;
  case 0x301c: return regs.r14.l;
  case 0x301d: return regs.r14.h;
  case 0x301e: return regs.r15.l;
  case 0x301f: return regs.r15.h;

//0x3020 - 0x302f unused

  case 0x3030: return regs.sfr;
  case 0x3031: return regs.sfr >> 8;
  case 0x3032: return 0x00; //unused
  case 0x3033: return 0x00; //BRAMR (write only)
  case 0x3034: return regs.pbr;
  case 0x3035: return 0x00; //unused
  case 0x3036: return regs.rombr;
  case 0x3037: return 0x00; //CFGR (write only)
  case 0x3038: return 0x00; //SCBR (write only)
  case 0x3039: return 0x00; //CLSR (write only)
  case 0x303a: return 0x00; //SCMR (write only)
  case 0x303b: return regs.vcr;
  case 0x303c: return regs.rambr;
  case 0x303d: return 0x00; //unused
  case 0x303e: return regs.cbr;
  case 0x303f: return regs.cbr >> 8;

//0x3040 - 0x30ff unused
  }

  if(addr >= 0x3100 && addr <= 0x32ff) {
    return cache[addr - 0x3100];
  }

  return 0x00;
}
