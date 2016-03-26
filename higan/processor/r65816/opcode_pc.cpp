auto R65816::op_branch(bool flag, bool value) {
  if(flag != value) {
L   rd.l = readpc();
  } else {
    rd.l = readpc();
    aa.w = regs.pc.d + (int8)rd.l;
    io6(aa.w);
L   io();
    regs.pc.w = aa.w;
  }
}

auto R65816::op_bra() {
  rd.l = readpc();
  aa.w = regs.pc.d + (int8)rd.l;
  io6(aa.w);
L io();
  regs.pc.w = aa.w;
}

auto R65816::op_brl() {
  rd.l = readpc();
  rd.h = readpc();
L io();
  regs.pc.w = regs.pc.d + (int16)rd.w;
}

auto R65816::op_jmp_addr() {
  rd.l = readpc();
L rd.h = readpc();
  regs.pc.w = rd.w;
}

auto R65816::op_jmp_long() {
  rd.l = readpc();
  rd.h = readpc();
L rd.b = readpc();
  regs.pc.d = rd.d & 0xffffff;
}

auto R65816::op_jmp_iaddr() {
  aa.l = readpc();
  aa.h = readpc();
  rd.l = readaddr(aa.w + 0);
L rd.h = readaddr(aa.w + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_jmp_iaddrx() {
  aa.l = readpc();
  aa.h = readpc();
  io();
  rd.l = readpbr(aa.w + regs.x.w + 0);
L rd.h = readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_jmp_iladdr() {
  aa.l = readpc();
  aa.h = readpc();
  rd.l = readaddr(aa.w + 0);
  rd.h = readaddr(aa.w + 1);
L rd.b = readaddr(aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
}

auto R65816::op_jsr_addr() {
  aa.l = readpc();
  aa.h = readpc();
  io();
  regs.pc.w--;
  writestack(regs.pc.h);
L writestack(regs.pc.l);
  regs.pc.w = aa.w;
}

auto R65816::op_jsr_long_e() {
  aa.l = readpc();
  aa.h = readpc();
  writestackn(regs.pc.b);
  io();
  aa.b = readpc();
  regs.pc.w--;
  writestackn(regs.pc.h);
L writestackn(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
  regs.s.h = 0x01;
}

auto R65816::op_jsr_long_n() {
  aa.l = readpc();
  aa.h = readpc();
  writestackn(regs.pc.b);
  io();
  aa.b = readpc();
  regs.pc.w--;
  writestackn(regs.pc.h);
L writestackn(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
}

auto R65816::op_jsr_iaddrx_e() {
  aa.l = readpc();
  writestackn(regs.pc.h);
  writestackn(regs.pc.l);
  aa.h = readpc();
  io();
  rd.l = readpbr(aa.w + regs.x.w + 0);
L rd.h = readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
  regs.s.h = 0x01;
}

auto R65816::op_jsr_iaddrx_n() {
  aa.l = readpc();
  writestackn(regs.pc.h);
  writestackn(regs.pc.l);
  aa.h = readpc();
  io();
  rd.l = readpbr(aa.w + regs.x.w + 0);
L rd.h = readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_rti_e() {
  io();
  io();
  regs.p = readstack() | 0x30;
  rd.l = readstack();
L rd.h = readstack();
  regs.pc.w = rd.w;
}

auto R65816::op_rti_n() {
  io();
  io();
  regs.p = readstack();
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
  rd.l = readstack();
  rd.h = readstack();
L rd.b = readstack();
  regs.pc.d = rd.d & 0xffffff;
}

auto R65816::op_rts() {
  io();
  io();
  rd.l = readstack();
  rd.h = readstack();
L io();
  regs.pc.w = ++rd.w;
}

auto R65816::op_rtl_e() {
  io();
  io();
  rd.l = readstackn();
  rd.h = readstackn();
L rd.b = readstackn();
  regs.pc.b = rd.b;
  regs.pc.w = ++rd.w;
  regs.s.h = 0x01;
}

auto R65816::op_rtl_n() {
  io();
  io();
  rd.l = readstackn();
  rd.h = readstackn();
L rd.b = readstackn();
  regs.pc.b = rd.b;
  regs.pc.w = ++rd.w;
}
