auto R65816::op_branch(bool flag, bool value) {
  if(flag != value) {
L   rd.l = readPC();
  } else {
    rd.l = readPC();
    aa.w = r.pc.d + (int8)rd.l;
    idle6(aa.w);
L   idle();
    r.pc.w = aa.w;
  }
}

auto R65816::op_bra() {
  rd.l = readPC();
  aa.w = r.pc.d + (int8)rd.l;
  idle6(aa.w);
L idle();
  r.pc.w = aa.w;
}

auto R65816::op_brl() {
  rd.l = readPC();
  rd.h = readPC();
L idle();
  r.pc.w = r.pc.d + (int16)rd.w;
}

auto R65816::op_jmp_addr() {
  rd.l = readPC();
L rd.h = readPC();
  r.pc.w = rd.w;
}

auto R65816::op_jmp_long() {
  rd.l = readPC();
  rd.h = readPC();
L rd.b = readPC();
  r.pc.d = rd.d;
}

auto R65816::op_jmp_iaddr() {
  aa.l = readPC();
  aa.h = readPC();
  rd.l = readAddr(aa.w + 0);
L rd.h = readAddr(aa.w + 1);
  r.pc.w = rd.w;
}

auto R65816::op_jmp_iaddrx() {
  aa.l = readPC();
  aa.h = readPC();
  idle();
  rd.l = readPB(aa.w + r.x.w + 0);
L rd.h = readPB(aa.w + r.x.w + 1);
  r.pc.w = rd.w;
}

auto R65816::op_jmp_iladdr() {
  aa.l = readPC();
  aa.h = readPC();
  rd.l = readAddr(aa.w + 0);
  rd.h = readAddr(aa.w + 1);
L rd.b = readAddr(aa.w + 2);
  r.pc.d = rd.d;
}

auto R65816::op_jsr_addr() {
  aa.l = readPC();
  aa.h = readPC();
  idle();
  r.pc.w--;
  writeSP(r.pc.h);
L writeSP(r.pc.l);
  r.pc.w = aa.w;
}

auto R65816::op_jsr_long() {
  aa.l = readPC();
  aa.h = readPC();
  writeSPn(r.pc.b);
  idle();
  aa.b = readPC();
  r.pc.w--;
  writeSPn(r.pc.h);
L writeSPn(r.pc.l);
  r.pc.d = aa.d;
E r.s.h = 0x01;
}

auto R65816::op_jsr_iaddrx() {
  aa.l = readPC();
  writeSPn(r.pc.h);
  writeSPn(r.pc.l);
  aa.h = readPC();
  idle();
  rd.l = readPB(aa.w + r.x.w + 0);
L rd.h = readPB(aa.w + r.x.w + 1);
  r.pc.w = rd.w;
E r.s.h = 0x01;
}

auto R65816::op_rti() {
  idle();
  idle();
  r.p = readSP();
E r.p.m = 1, r.p.x = 1;
  if(r.p.x) {
    r.x.h = 0x00;
    r.y.h = 0x00;
  }
  r.pc.l = readSP();
  if(r.e) {
  L r.pc.h = readSP();
  } else {
    r.pc.h = readSP();
  L r.pc.b = readSP();
  }
}

auto R65816::op_rts() {
  idle();
  idle();
  rd.l = readSP();
  rd.h = readSP();
L idle();
  r.pc.w = ++rd.w;
}

auto R65816::op_rtl() {
  idle();
  idle();
  rd.l = readSPn();
  rd.h = readSPn();
L rd.b = readSPn();
  r.pc.b = rd.b;
  r.pc.w = ++rd.w;
E r.s.h = 0x01;
}
