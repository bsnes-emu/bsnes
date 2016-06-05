#define call (this->*op)

auto SPC700::op_adjust(fps op, uint8_t& r) -> void {
  io();
  r = call(r);
}

auto SPC700::op_adjust_addr(fps op) -> void {
  dp.l = readPC();
  dp.h = readPC();
  rd = read(dp);
  rd = call(rd);
  write(dp, rd);
}

auto SPC700::op_adjust_dp(fps op) -> void {
  dp = readPC();
  rd = readDP(dp);
  rd = call(rd);
  writeDP(dp, rd);
}

auto SPC700::op_adjust_dpw(int n) -> void {
  dp = readPC();
  rd.w = readDP(dp) + n;
  writeDP(dp++, rd.l);
  rd.h += readDP(dp);
  writeDP(dp++, rd.h);
  regs.p.n = rd & 0x8000;
  regs.p.z = rd == 0;
}

auto SPC700::op_adjust_dpx(fps op) -> void {
  dp = readPC();
  io();
  rd = readDP(dp + regs.x);
  rd = call(rd);
  writeDP(dp + regs.x, rd);
}

auto SPC700::op_branch(bool condition) -> void {
  rd = readPC();
  if(!condition) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_branch_bit() -> void {
  dp = readPC();
  sp = readDP(dp);
  rd = readPC();
  io();
  if((bool)(sp & (1 << (opcode >> 5))) == (bool)(opcode & 0x10)) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_pull(uint8_t& r) -> void {
  io();
  io();
  r = readSP();
}

auto SPC700::op_push(uint8 r) -> void {
  io();
  io();
  writeSP(r);
}

auto SPC700::op_read_addr(fpb op, uint8_t& r) -> void {
  dp.l = readPC();
  dp.h = readPC();
  rd = read(dp);
  r = call(r, rd);
}

auto SPC700::op_read_addri(fpb op, uint8_t& r) -> void {
  dp.l = readPC();
  dp.h = readPC();
  io();
  rd = read(dp + r);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_read_const(fpb op, uint8_t& r) -> void {
  rd = readPC();
  r = call(r, rd);
}

auto SPC700::op_read_dp(fpb op, uint8_t& r) -> void {
  dp = readPC();
  rd = readDP(dp);
  r = call(r, rd);
}

auto SPC700::op_read_dpi(fpb op, uint8_t& r, uint8_t& i) -> void {
  dp = readPC();
  io();
  rd = readDP(dp + i);
  r = call(r, rd);
}

auto SPC700::op_read_dpw(fpw op) -> void {
  dp = readPC();
  rd.l = readDP(dp++);
  if(op != &SPC700::op_cpw) io();
  rd.h = readDP(dp++);
  regs.ya = call(regs.ya, rd);
}

auto SPC700::op_read_idpx(fpb op) -> void {
  dp = readPC() + regs.x;
  io();
  sp.l = readDP(dp++);
  sp.h = readDP(dp++);
  rd = read(sp);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_read_idpy(fpb op) -> void {
  dp = readPC();
  io();
  sp.l = readDP(dp++);
  sp.h = readDP(dp++);
  rd = read(sp + regs.y);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_read_ix(fpb op) -> void {
  io();
  rd = readDP(regs.x);
  regs.a = call(regs.a, rd);
}

auto SPC700::op_set_addr_bit() -> void {
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

auto SPC700::op_set_bit() -> void {
  dp = readPC();
  rd = readDP(dp) & ~(1 << (opcode >> 5));
  writeDP(dp, rd | (!(opcode & 0x10) << (opcode >> 5)));
}

auto SPC700::op_set_flag(bool& flag, bool data) -> void {
  io();
  if(&flag == &regs.p.i) io();
  flag = data;
}

auto SPC700::op_test_addr(bool set) -> void {
  dp.l = readPC();
  dp.h = readPC();
  rd = read(dp);
  regs.p.n = (regs.a - rd) & 0x80;
  regs.p.z = (regs.a - rd) == 0;
  read(dp);
  write(dp, set ? rd | regs.a : rd & ~regs.a);
}

auto SPC700::op_transfer(uint8_t& from, uint8_t& to) -> void {
  io();
  to = from;
  if(&to == &regs.s) return;
  regs.p.n = (to & 0x80);
  regs.p.z = (to == 0);
}

auto SPC700::op_write_addr(uint8_t& r) -> void {
  dp.l = readPC();
  dp.h = readPC();
  read(dp);
  write(dp, r);
}

auto SPC700::op_write_addri(uint8_t& i) -> void {
  dp.l = readPC();
  dp.h = readPC();
  io();
  dp += i;
  read(dp);
  write(dp, regs.a);
}

auto SPC700::op_write_dp(uint8_t& r) -> void {
  dp = readPC();
  readDP(dp);
  writeDP(dp, r);
}

auto SPC700::op_write_dpi(uint8_t& r, uint8_t& i) -> void {
  dp = readPC() + i;
  io();
  readDP(dp);
  writeDP(dp, r);
}

auto SPC700::op_write_dp_const(fpb op) -> void {
  rd = readPC();
  dp = readPC();
  wr = readDP(dp);
  wr = call(wr, rd);
  op != &SPC700::op_cmp ? writeDP(dp, wr) : io();
}

auto SPC700::op_write_dp_dp(fpb op) -> void {
  sp = readPC();
  rd = readDP(sp);
  dp = readPC();
  if(op != &SPC700::op_st) wr = readDP(dp);
  wr = call(wr, rd);
  op != &SPC700::op_cmp ? writeDP(dp, wr) : io();
}

auto SPC700::op_write_ix_iy(fpb op) -> void {
  io();
  rd = readDP(regs.y);
  wr = readDP(regs.x);
  wr = call(wr, rd);
  op != &SPC700::op_cmp ? writeDP(regs.x, wr) : io();
}

//

auto SPC700::op_bne_dp() -> void {
  dp = readPC();
  sp = readDP(dp);
  rd = readPC();
  io();
  if(regs.a == sp) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_bne_dpdec() -> void {
  dp = readPC();
  wr = readDP(dp);
  writeDP(dp, --wr);
  rd = readPC();
  if(wr == 0) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_bne_dpx() -> void {
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

auto SPC700::op_bne_ydec() -> void {
  rd = readPC();
  io();
  io();
  if(--regs.y == 0) return;
  io();
  io();
  regs.pc += (int8)rd;
}

auto SPC700::op_brk() -> void {
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

auto SPC700::op_clv() -> void {
  io();
  regs.p.v = 0;
  regs.p.h = 0;
}

auto SPC700::op_cmc() -> void {
  io();
  io();
  regs.p.c = !regs.p.c;
}

auto SPC700::op_daa() -> void {
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

auto SPC700::op_das() -> void {
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

auto SPC700::op_div_ya_x() -> void {
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

auto SPC700::op_jmp_addr() -> void {
  rd.l = readPC();
  rd.h = readPC();
  regs.pc = rd;
}

auto SPC700::op_jmp_iaddrx() -> void {
  dp.l = readPC();
  dp.h = readPC();
  io();
  dp += regs.x;
  rd.l = read(dp++);
  rd.h = read(dp++);
  regs.pc = rd;
}

auto SPC700::op_jsp_dp() -> void {
  rd = readPC();
  io();
  io();
  writeSP(regs.pc.h);
  writeSP(regs.pc.l);
  regs.pc = 0xff00 | rd;
}

auto SPC700::op_jsr_addr() -> void {
  rd.l = readPC();
  rd.h = readPC();
  io();
  io();
  io();
  writeSP(regs.pc.h);
  writeSP(regs.pc.l);
  regs.pc = rd;
}

auto SPC700::op_jst() -> void {
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

auto SPC700::op_lda_ixinc() -> void {
  io();
  regs.a = readDP(regs.x++);
  io();
  regs.p.n = regs.a & 0x80;
  regs.p.z = regs.a == 0;
}

auto SPC700::op_mul_ya() -> void {
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

auto SPC700::op_nop() -> void {
  io();
}

auto SPC700::op_plp() -> void {
  io();
  io();
  regs.p = readSP();
}

auto SPC700::op_rti() -> void {
  regs.p = readSP();
  rd.l = readSP();
  rd.h = readSP();
  io();
  io();
  regs.pc = rd;
}

auto SPC700::op_rts() -> void {
  rd.l = readSP();
  rd.h = readSP();
  io();
  io();
  regs.pc = rd;
}

auto SPC700::op_sta_idpx() -> void {
  sp = readPC() + regs.x;
  io();
  dp.l = readDP(sp++);
  dp.h = readDP(sp++);
  read(dp);
  write(dp, regs.a);
}

auto SPC700::op_sta_idpy() -> void {
  sp = readPC();
  dp.l = readDP(sp++);
  dp.h = readDP(sp++);
  io();
  dp += regs.y;
  read(dp);
  write(dp, regs.a);
}

auto SPC700::op_sta_ix() -> void {
  io();
  readDP(regs.x);
  writeDP(regs.x, regs.a);
}

auto SPC700::op_sta_ixinc() -> void {
  io();
  io();
  writeDP(regs.x++, regs.a);
}

auto SPC700::op_stw_dp() -> void {
  dp = readPC();
  readDP(dp);
  writeDP(dp++, regs.a);
  writeDP(dp++, regs.y);
}

auto SPC700::op_wait() -> void {
  while(true) {
    io();
    io();
  }
}

auto SPC700::op_xcn() -> void {
  io();
  io();
  io();
  io();
  regs.a = (regs.a >> 4) | (regs.a << 4);
  regs.p.n = regs.a & 0x80;
  regs.p.z = regs.a == 0;
}

#undef call
