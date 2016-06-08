#define call (this->*op)

auto SPC700::op_adjust(fps op, reg r) {
  io();
  r = call(r);
}

auto SPC700::op_adjust_addr(fps op) {
  dp.l = readPC();
  dp.h = readPC();
  rd = read(dp);
  rd = call(rd);
  write(dp, rd);
}

auto SPC700::op_adjust_dp(fps op) {
  dp = readPC();
  rd = readDP(dp);
  rd = call(rd);
  writeDP(dp, rd);
}

auto SPC700::op_adjust_dpw(int n) {
  dp = readPC();
  rd.w = readDP(dp) + n;
  writeDP(dp++, rd.l);
  rd.h += readDP(dp);
  writeDP(dp++, rd.h);
  regs.p.n = rd & 0x8000;
  regs.p.z = rd == 0;
}

auto SPC700::op_adjust_dpx(fps op) {
  dp = readPC();
  io();
  rd = readDP(dp + regs.x);
  rd = call(rd);
  writeDP(dp + regs.x, rd);
}

auto SPC700::op_branch(bool condition) {
  rd = readPC();
  if(!condition) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_branch_bit() {
  dp = readPC();
  sp = readDP(dp);
  rd = readPC();
  io();
  if((bool)(sp & (1 << (opcode >> 5))) == (bool)(opcode & 0x10)) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_pull(reg r) {
  io();
  io();
  r = readSP();
}

auto SPC700::op_push(uint8 r) {
  io();
  io();
  writeSP(r);
}

auto SPC700::op_read_addr(fpb op, reg r) {
  dp.l = readPC();
  dp.h = readPC();
  rd = read(dp);
  r = call(r, rd);
}

auto SPC700::op_read_addri(fpb op, reg r) {
  dp.l = readPC();
  dp.h = readPC();
  io();
  rd = read(dp + r);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_read_const(fpb op, reg r) {
  rd = readPC();
  r = call(r, rd);
}

auto SPC700::op_read_dp(fpb op, reg r) {
  dp = readPC();
  rd = readDP(dp);
  r = call(r, rd);
}

auto SPC700::op_read_dpi(fpb op, reg r, reg i) {
  dp = readPC();
  io();
  rd = readDP(dp + i);
  r = call(r, rd);
}

auto SPC700::op_read_dpw(fpw op) {
  dp = readPC();
  rd.l = readDP(dp++);
  if(op != &SPC700::op_cpw) io();
  rd.h = readDP(dp++);
  regs.ya = call(regs.ya, rd);
}

auto SPC700::op_read_idpx(fpb op) {
  dp = readPC() + regs.x;
  io();
  sp.l = readDP(dp++);
  sp.h = readDP(dp++);
  rd = read(sp);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_read_idpy(fpb op) {
  dp = readPC();
  io();
  sp.l = readDP(dp++);
  sp.h = readDP(dp++);
  rd = read(sp + regs.y);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_read_ix(fpb op) {
  io();
  rd = readDP(regs.x);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_set_addr_bit() {
  dp.l = readPC();
  dp.h = readPC();
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = read(dp);
  switch(opcode >> 5) {
  case 0:  //orc  addr:bit
  case 1:  //orc !addr:bit
    io();
    regs.p.c |= (rd & (1 << bit)) ^ (bool)(opcode & 0x20);
    break;
  case 2:  //and  addr:bit
  case 3:  //and !addr:bit
    regs.p.c &= (rd & (1 << bit)) ^ (bool)(opcode & 0x20);
    break;
  case 4:  //eor  addr:bit
    io();
    regs.p.c ^= (bool)(rd & (1 << bit));
    break;
  case 5:  //ldc  addr:bit
    regs.p.c  = (rd & (1 << bit));
    break;
  case 6:  //stc  addr:bit
    io();
    rd = (rd & ~(1 << bit)) | (regs.p.c << bit);
    write(dp, rd);
    break;
  case 7:  //not  addr:bit
    rd ^= 1 << bit;
    write(dp, rd);
    break;
  }
}

auto SPC700::op_set_bit() {
  dp = readPC();
  rd = readDP(dp) & ~(1 << (opcode >> 5));
  writeDP(dp, rd | (!(opcode & 0x10) << (opcode >> 5)));
}

auto SPC700::op_set_flag(uint bit, bool value) {
  io();
  if(bit == regs.p.i.bit) io();
  regs.p = value ? (regs.p | (1 << bit)) : (regs.p & ~(1 << bit));
}

auto SPC700::op_test_addr(bool set) {
  dp.l = readPC();
  dp.h = readPC();
  rd = read(dp);
  regs.p.n = (regs.a - rd) & 0x80;
  regs.p.z = (regs.a - rd) == 0;
  read(dp);
  write(dp, set ? rd | regs.a : rd & ~regs.a);
}

auto SPC700::op_transfer(reg from, reg to) {
  io();
  to = from;
  if(&to == &regs.s) return;
  regs.p.n = (to & 0x80);
  regs.p.z = (to == 0);
}

auto SPC700::op_write_addr(reg r) {
  dp.l = readPC();
  dp.h = readPC();
  read(dp);
  write(dp, r);
}

auto SPC700::op_write_addri(reg i) {
  dp.l = readPC();
  dp.h = readPC();
  io();
  dp += i;
  read(dp);
  write(dp, regs.a);
}

auto SPC700::op_write_dp(reg r) {
  dp = readPC();
  readDP(dp);
  writeDP(dp, r);
}

auto SPC700::op_write_dpi(reg r, reg i) {
  dp = readPC() + i;
  io();
  readDP(dp);
  writeDP(dp, r);
}

auto SPC700::op_write_dp_const(fpb op) {
  rd = readPC();
  dp = readPC();
  wr = readDP(dp);
  wr = call(wr, rd);
  op != &SPC700::op_cmp ? writeDP(dp, wr) : io();
}

auto SPC700::op_write_dp_dp(fpb op) {
  sp = readPC();
  rd = readDP(sp);
  dp = readPC();
  if(op != &SPC700::op_st) wr = readDP(dp);
  wr = call(wr, rd);
  op != &SPC700::op_cmp ? writeDP(dp, wr) : io();
}

auto SPC700::op_write_ix_iy(fpb op) {
  io();
  rd = readDP(regs.y);
  wr = readDP(regs.x);
  wr = call(wr, rd);
  op != &SPC700::op_cmp ? writeDP(regs.x, wr) : io();
}

//

auto SPC700::op_bne_dp() {
  dp = readPC();
  sp = readDP(dp);
  rd = readPC();
  io();
  if(regs.a == sp) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_bne_dpdec() {
  dp = readPC();
  wr = readDP(dp);
  writeDP(dp, --wr);
  rd = readPC();
  if(wr == 0) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_bne_dpx() {
  dp = readPC();
  io();
  sp = readDP(dp + regs.x);
  rd = readPC();
  io();
  if(regs.a == sp) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_bne_ydec() {
  rd = readPC();
  io();
  io();
  if(--regs.y == 0) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_brk() {
  rd.l = read(0xffde);
  rd.h = read(0xffdf);
  io();
  io();
  writeSP(regs.pc.h);
  writeSP(regs.pc.l);
  writeSP(regs.p);
  regs.pc = rd;
  regs.p.b = 1;
  regs.p.i = 0;
}

auto SPC700::op_clv() {
  io();
  regs.p.v = 0;
  regs.p.h = 0;
}

auto SPC700::op_cmc() {
  io();
  io();
  regs.p.c = !regs.p.c;
}

auto SPC700::op_daa() {
  io();
  io();
  if(regs.p.c || (regs.a) > 0x99) {
    regs.a += 0x60;
    regs.p.c = 1;
  }
  if(regs.p.h || (regs.a & 15) > 0x09) {
    regs.a += 0x06;
  }
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto SPC700::op_das() {
  io();
  io();
  if(!regs.p.c || (regs.a) > 0x99) {
    regs.a -= 0x60;
    regs.p.c = 0;
  }
  if(!regs.p.h || (regs.a & 15) > 0x09) {
    regs.a -= 0x06;
  }
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto SPC700::op_div_ya_x() {
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  ya = regs.ya;
  //overflow set if quotient >= 256
  regs.p.v = (regs.y >= regs.x);
  regs.p.h = ((regs.y & 15) >= (regs.x & 15));
  if(regs.y < (regs.x << 1)) {
    //if quotient is <= 511 (will fit into 9-bit result)
    regs.a = ya / regs.x;
    regs.y = ya % regs.x;
  } else {
    //otherwise, the quotient won't fit into regs.p.v + regs.a
    //this emulates the odd behavior of the S-SMP in this case
    regs.a = 255    - (ya - (regs.x << 9)) / (256 - regs.x);
    regs.y = regs.x + (ya - (regs.x << 9)) % (256 - regs.x);
  }
  //result is set based on a (quotient) only
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto SPC700::op_jmp_addr() {
  rd.l = readPC();
  rd.h = readPC();
  regs.pc = rd;
}

auto SPC700::op_jmp_iaddrx() {
  dp.l = readPC();
  dp.h = readPC();
  io();
  dp += regs.x;
  rd.l = read(dp++);
  rd.h = read(dp++);
  regs.pc = rd;
}

auto SPC700::op_jsp_dp() {
  rd = readPC();
  io();
  io();
  writeSP(regs.pc.h);
  writeSP(regs.pc.l);
  regs.pc = 0xff00 | rd;
}

auto SPC700::op_jsr_addr() {
  rd.l = readPC();
  rd.h = readPC();
  io();
  io();
  io();
  writeSP(regs.pc.h);
  writeSP(regs.pc.l);
  regs.pc = rd;
}

auto SPC700::op_jst() {
  dp = 0xffde - ((opcode >> 4) << 1);
  rd.l = read(dp++);
  rd.h = read(dp++);
  io();
  io();
  io();
  writeSP(regs.pc.h);
  writeSP(regs.pc.l);
  regs.pc = rd;
}

auto SPC700::op_lda_ixinc() {
  io();
  regs.a = readDP(regs.x++);
  io();
  regs.p.n = regs.a & 0x80;
  regs.p.z = regs.a == 0;
}

auto SPC700::op_mul_ya() {
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  io();
  ya = regs.y * regs.a;
  regs.a = ya;
  regs.y = ya >> 8;
  //result is set based on y (high-byte) only
  regs.p.n = (regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

auto SPC700::op_nop() {
  io();
}

auto SPC700::op_plp() {
  io();
  io();
  regs.p = readSP();
}

auto SPC700::op_rti() {
  regs.p = readSP();
  rd.l = readSP();
  rd.h = readSP();
  io();
  io();
  regs.pc = rd;
}

auto SPC700::op_rts() {
  rd.l = readSP();
  rd.h = readSP();
  io();
  io();
  regs.pc = rd;
}

auto SPC700::op_sta_idpx() {
  sp = readPC() + regs.x;
  io();
  dp.l = readDP(sp++);
  dp.h = readDP(sp++);
  read(dp);
  write(dp, regs.a);
}

auto SPC700::op_sta_idpy() {
  sp = readPC();
  dp.l = readDP(sp++);
  dp.h = readDP(sp++);
  io();
  dp += regs.y;
  read(dp);
  write(dp, regs.a);
}

auto SPC700::op_sta_ix() {
  io();
  readDP(regs.x);
  writeDP(regs.x, regs.a);
}

auto SPC700::op_sta_ixinc() {
  io();
  io();
  writeDP(regs.x++, regs.a);
}

auto SPC700::op_stw_dp() {
  dp = readPC();
  readDP(dp);
  writeDP(dp++, regs.a);
  writeDP(dp++, regs.y);
}

auto SPC700::op_wait() {
  while(true) {
    io();
    io();
  }
}

auto SPC700::op_xcn() {
  io();
  io();
  io();
  io();
  regs.a = (regs.a >> 4) | (regs.a << 4);
  regs.p.n = regs.a & 0x80;
  regs.p.z = regs.a == 0;
}

#undef call
