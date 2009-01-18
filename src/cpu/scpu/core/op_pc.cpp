#ifdef SCPU_CPP

//bcc
case 0x90: {
  if(!!regs.p.c) last_cycle();
  rd.l = op_readpc();
  if(!regs.p.c) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bcs
case 0xb0: {
  if(!regs.p.c) last_cycle();
  rd.l = op_readpc();
  if(regs.p.c) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bne
case 0xd0: {
  if(!!regs.p.z) last_cycle();
  rd.l = op_readpc();
  if(!regs.p.z) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//beq
case 0xf0: {
  if(!regs.p.z) last_cycle();
  rd.l = op_readpc();
  if(regs.p.z) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bpl
case 0x10: {
  if(!!regs.p.n) last_cycle();
  rd.l = op_readpc();
  if(!regs.p.n) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bmi
case 0x30: {
  if(!regs.p.n) last_cycle();
  rd.l = op_readpc();
  if(regs.p.n) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bvc
case 0x50: {
  if(!!regs.p.v) last_cycle();
  rd.l = op_readpc();
  if(!regs.p.v) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bvs
case 0x70: {
  if(!regs.p.v) last_cycle();
  rd.l = op_readpc();
  if(regs.p.v) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    break;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//bra
case 0x80: {
  rd.l = op_readpc();
  aa.w = regs.pc.d + (int8)rd.l;
  regs.pc.w = aa.w;
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
} break;

//brl
case 0x82: {
  rd.l = op_readpc();
  rd.h = op_readpc();
  last_cycle();
  op_io();
  regs.pc.w = regs.pc.d + (int16)rd.w;
} break;

//jmp_addr
case 0x4c: {
  rd.l = op_readpc();
  last_cycle();
  rd.h = op_readpc();
  regs.pc.w = rd.w;
} break;

//jmp_long
case 0x5c: {
  rd.l = op_readpc();
  rd.h = op_readpc();
  last_cycle();
  rd.b = op_readpc();
  regs.pc.d = rd.d & 0xffffff;
} break;

//jmp_iaddr
case 0x6c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readaddr(aa.w);
  last_cycle();
  rd.h = op_readaddr(aa.w + 1);
  regs.pc.w = rd.w;
} break;

//jmp_iaddrx
case 0x7c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readpbr(aa.w + regs.x.w);
  last_cycle();
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
} break;

//jmp_iladdr
case 0xdc: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readaddr(aa.w);
  rd.h = op_readaddr(aa.w + 1);
  last_cycle();
  rd.b = op_readaddr(aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
} break;

//jsr_addr
case 0x20: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  regs.pc.w--;
  op_writestack(regs.pc.h);
  last_cycle();
  op_writestack(regs.pc.l);
  regs.pc.w = aa.w;
} break;

//jsr_long
case 0x22: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(regs.pc.b);
  op_io();
  aa.b = op_readpc();
  regs.pc.w--;
  op_writestackn(regs.pc.h);
  last_cycle();
  op_writestackn(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
  if(regs.e) regs.s.h = 0x01;
} break;

//jsr_iaddrx
case 0xfc: {
  aa.l = op_readpc();
  op_writestackn(regs.pc.h);
  op_writestackn(regs.pc.l);
  aa.h = op_readpc();
  op_io();
  rd.l = op_readpbr(aa.w + regs.x.w);
  last_cycle();
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
  if(regs.e) regs.s.h = 0x01;
} break;

//rti
case 0x40: {
  op_io();
  op_io();
  regs.p = op_readstack();
  if(regs.e) regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
  rd.l = op_readstack();
  if(regs.e) last_cycle();
  rd.h = op_readstack();
  if(regs.e) {
    regs.pc.w = rd.w;
    break;
  }
  last_cycle();
  rd.b = op_readstack();
  regs.pc.d = rd.d & 0xffffff;
} break;

//rts
case 0x60: {
  op_io();
  op_io();
  rd.l = op_readstack();
  rd.h = op_readstack();
  last_cycle();
  op_io();
  regs.pc.w = rd.w;
  regs.pc.w++;
} break;

//rtl
case 0x6b: {
  op_io();
  op_io();
  rd.l = op_readstackn();
  rd.h = op_readstackn();
  last_cycle();
  rd.b = op_readstackn();
  regs.pc.d = rd.d & 0xffffff;
  regs.pc.w++;
  if(regs.e) regs.s.h = 0x01;
} break;

#endif
