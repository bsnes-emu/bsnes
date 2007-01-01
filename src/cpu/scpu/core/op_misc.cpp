//nop
case 0xea: {
  last_cycle();
  op_io();
} break;

//wdm
case 0x42: {
  last_cycle();
  op_readpc();
} break;

//xba
case 0xeb: {
  op_io();
  last_cycle();
  op_io();
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
} break;

//mvn
case 0x54: {
  dp = op_readpc();
  sp = op_readpc();
  regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
  op_writelong((dp << 16) | regs.y.w, rd.l);
  op_io();
  if(regs.p.x) {
    regs.x.l ++;
    regs.y.l ++;
  } else {
    regs.x.w ++;
    regs.y.w ++;
  }
  last_cycle();
  op_io();
  if(regs.a.w--)regs.pc.w -= 3;
} break;

//mvp
case 0x44: {
  dp = op_readpc();
  sp = op_readpc();
  regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
  op_writelong((dp << 16) | regs.y.w, rd.l);
  op_io();
  if(regs.p.x) {
    regs.x.l --;
    regs.y.l --;
  } else {
    regs.x.w --;
    regs.y.w --;
  }
  last_cycle();
  op_io();
  if(regs.a.w--)regs.pc.w -= 3;
} break;

//brk
case 0x00: {
  op_readpc();
  if(!regs.e)op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.p);
  rd.l = op_readlong((regs.e) ? 0xfffe : 0xffe6);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  last_cycle();
  rd.h = op_readlong((regs.e) ? 0xffff : 0xffe7);
  regs.pc.w = rd.w;
} break;

//cop
case 0x02: {
  op_readpc();
  if(!regs.e)op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.p);
  rd.l = op_readlong((regs.e) ? 0xfff4 : 0xffe4);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  last_cycle();
  rd.h = op_readlong((regs.e) ? 0xfff5 : 0xffe5);
  regs.pc.w = rd.w;
} break;

//stp
case 0xdb: {
  op_io();
  last_cycle();
  while(1) { op_io(); }
} break;

//wai
case 0xcb: {
  op_io();
  event.wai = true;
  last_cycle();
  op_io();
  while(event.wai) {
    last_cycle();
    op_io();
  }
} break;

//xce
case 0xfb: {
  last_cycle();
  op_io();
bool carry = regs.p.c;
  regs.p.c = regs.e;
  regs.e = carry;
  if(regs.e) {
    regs.p |= 0x30;
    regs.s.h = 0x01;
  }
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
} break;

//clc
case 0x18: {
  last_cycle();
  op_io();
  regs.p.c = 0;
} break;

//cld
case 0xd8: {
  last_cycle();
  op_io();
  regs.p.d = 0;
} break;

//cli
case 0x58: {
  last_cycle();
  op_io();
  regs.p.i = 0;
} break;

//clv
case 0xb8: {
  last_cycle();
  op_io();
  regs.p.v = 0;
} break;

//sec
case 0x38: {
  last_cycle();
  op_io();
  regs.p.c = 1;
} break;

//sed
case 0xf8: {
  last_cycle();
  op_io();
  regs.p.d = 1;
} break;

//sei
case 0x78: {
  last_cycle();
  op_io();
  regs.p.i = 1;
} break;

//rep
case 0xc2: {
  rd.l = op_readpc();
  last_cycle();
  op_io();
  regs.p &=~ rd.l;
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
} break;

//sep
case 0xe2: {
  rd.l = op_readpc();
  last_cycle();
  op_io();
  regs.p |= rd.l;
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
} break;

//tax
case 0xaa: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l = regs.a.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.a.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
} break;

//tay
case 0xa8: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l = regs.a.l;
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w = regs.a.w;
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
} break;

//txa
case 0x8a: {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l = regs.x.l;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w = regs.x.w;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//txy
case 0x9b: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l = regs.x.l;
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w = regs.x.w;
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
} break;

//tya
case 0x98: {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l = regs.y.l;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w = regs.y.w;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//tyx
case 0xbb: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l = regs.y.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.y.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
} break;

//tcd
case 0x5b: {
  last_cycle();
  op_io();
  regs.d.w = regs.a.w;
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
} break;

//tcs
case 0x1b: {
  last_cycle();
  op_io();
  regs.s.w = regs.a.w;
  if(regs.e)regs.s.h = 0x01;
} break;

//tdc
case 0x7b: {
  last_cycle();
  op_io();
  regs.a.w = regs.d.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
} break;

//tsc
case 0x3b: {
  last_cycle();
  op_io();
  regs.a.w = regs.s.w;
  if(regs.e) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//tsx
case 0xba: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l = regs.s.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.s.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
} break;

//txs
case 0x9a: {
  last_cycle();
  op_io();
  if(regs.e) {
    regs.s.l = regs.x.l;
  } else {
    regs.s.w = regs.x.w;
  }
} break;

//pha
case 0x48: {
  op_io();
  if(!regs.p.m)op_writestack(regs.a.h);
  last_cycle();
  op_writestack(regs.a.l);
} break;

//phx
case 0xda: {
  op_io();
  if(!regs.p.x)op_writestack(regs.x.h);
  last_cycle();
  op_writestack(regs.x.l);
} break;

//phy
case 0x5a: {
  op_io();
  if(!regs.p.x)op_writestack(regs.y.h);
  last_cycle();
  op_writestack(regs.y.l);
} break;

//phd
case 0x0b: {
  op_io();
  op_writestackn(regs.d.h);
  last_cycle();
  op_writestackn(regs.d.l);
  if(regs.e)regs.s.h = 0x01;
} break;

//phb
case 0x8b: {
  op_io();
  last_cycle();
  op_writestack(regs.db);
} break;

//phk
case 0x4b: {
  op_io();
  last_cycle();
  op_writestack(regs.pc.b);
} break;

//php
case 0x08: {
  op_io();
  last_cycle();
  op_writestack(regs.p);
} break;

//pla
case 0x68: {
  op_io();
  op_io();
  if(regs.p.m)last_cycle();
  regs.a.l = op_readstack();
  if(regs.p.m) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
    break;
  }
  last_cycle();
  regs.a.h = op_readstack();
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
} break;

//plx
case 0xfa: {
  op_io();
  op_io();
  if(regs.p.x)last_cycle();
  regs.x.l = op_readstack();
  if(regs.p.x) {
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
    break;
  }
  last_cycle();
  regs.x.h = op_readstack();
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
} break;

//ply
case 0x7a: {
  op_io();
  op_io();
  if(regs.p.x)last_cycle();
  regs.y.l = op_readstack();
  if(regs.p.x) {
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
    break;
  }
  last_cycle();
  regs.y.h = op_readstack();
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
} break;

//pld
case 0x2b: {
  op_io();
  op_io();
  regs.d.l = op_readstackn();
  last_cycle();
  regs.d.h = op_readstackn();
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
  if(regs.e)regs.s.h = 0x01;
} break;

//plb
case 0xab: {
  op_io();
  op_io();
  last_cycle();
  regs.db = op_readstack();
  regs.p.n = !!(regs.db & 0x80);
  regs.p.z = (regs.db == 0);
} break;

//plp
case 0x28: {
  op_io();
  op_io();
  last_cycle();
  regs.p = op_readstack();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
} break;

//pea
case 0xf4: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(aa.h);
  last_cycle();
  op_writestackn(aa.l);
  if(regs.e)regs.s.h = 0x01;
} break;

//pei
case 0xd4: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_writestackn(aa.h);
  last_cycle();
  op_writestackn(aa.l);
  if(regs.e)regs.s.h = 0x01;
} break;

//per
case 0x62: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.w = regs.pc.d + (int16)aa.w;
  op_writestackn(rd.h);
  last_cycle();
  op_writestackn(rd.l);
  if(regs.e)regs.s.h = 0x01;
} break;

