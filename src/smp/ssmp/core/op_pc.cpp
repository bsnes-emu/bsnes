void sSMP::op_bra() {
  rd = op_readpc();
  if(0)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_beq() {
  rd = op_readpc();
  if(!regs.p.z)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bne() {
  rd = op_readpc();
  if(regs.p.z)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bcs() {
  rd = op_readpc();
  if(!regs.p.c)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bcc() {
  rd = op_readpc();
  if(regs.p.c)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bvs() {
  rd = op_readpc();
  if(!regs.p.v)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bvc() {
  rd = op_readpc();
  if(regs.p.v)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bmi() {
  rd = op_readpc();
  if(!regs.p.n)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bpl() {
  rd = op_readpc();
  if(regs.p.n)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs0() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x01) != 0x01)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc0() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x01) == 0x01)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs1() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x02) != 0x02)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc1() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x02) == 0x02)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs2() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x04) != 0x04)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc2() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x04) == 0x04)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs3() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x08) != 0x08)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc3() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x08) == 0x08)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs4() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x10) != 0x10)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc4() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x10) == 0x10)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs5() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x20) != 0x20)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc5() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x20) == 0x20)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs6() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x40) != 0x40)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc6() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x40) == 0x40)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbs7() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x80) != 0x80)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_bbc7() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if((sp & 0x80) == 0x80)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_cbne_dp() {
  dp = op_readpc();
  sp = op_readdp(dp);
  rd = op_readpc();
  op_io();
  if(regs.a == sp)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_cbne_dpx() {
  dp = op_readpc();
  op_io();
  sp = op_readdp(dp + regs.x);
  rd = op_readpc();
  op_io();
  if(regs.a == sp)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_dbnz_dp() {
  dp = op_readpc();
  wr = op_readdp(dp);
  op_writedp(dp, --wr);
  rd = op_readpc();
  if(wr == 0x00)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_dbnz_y() {
  rd = op_readpc();
  op_io();
  regs.y--;
  op_io();
  if(regs.y == 0x00)return;
  op_io();
  op_io();
  regs.pc += (int8)rd;
}

void sSMP::op_jmp_addr() {
  rd  = op_readpc();
  rd |= op_readpc() << 8;
  regs.pc = rd;
}

void sSMP::op_jmp_iaddrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  dp += regs.x;
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  regs.pc = rd;
}

void sSMP::op_call() {
  rd  = op_readpc();
  rd |= op_readpc() << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_pcall() {
  rd = op_readpc();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = 0xff00 | rd;
}

void sSMP::op_tcall_0() {
  dp = 0xffde - (0 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_1() {
  dp = 0xffde - (1 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_2() {
  dp = 0xffde - (2 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_3() {
  dp = 0xffde - (3 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_4() {
  dp = 0xffde - (4 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_5() {
  dp = 0xffde - (5 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_6() {
  dp = 0xffde - (6 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_7() {
  dp = 0xffde - (7 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_8() {
  dp = 0xffde - (8 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_9() {
  dp = 0xffde - (9 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_10() {
  dp = 0xffde - (10 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_11() {
  dp = 0xffde - (11 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_12() {
  dp = 0xffde - (12 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_13() {
  dp = 0xffde - (13 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_14() {
  dp = 0xffde - (14 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_tcall_15() {
  dp = 0xffde - (15 << 1);
  rd  = op_readaddr(dp);
  rd |= op_readaddr(dp + 1) << 8;
  op_io();
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  regs.pc = rd;
}

void sSMP::op_brk() {
  rd  = op_readaddr(0xffde);
  rd |= op_readaddr(0xffdf) << 8;
  op_io();
  op_io();
  op_writestack(regs.pc >> 8);
  op_writestack(regs.pc);
  op_writestack(regs.p);
  regs.pc = rd;
  regs.p.b = 1;
  regs.p.i = 0;
}

void sSMP::op_ret() {
  rd  = op_readstack();
  rd |= op_readstack() << 8;
  op_io();
  op_io();
  regs.pc = rd;
}

void sSMP::op_reti() {
  regs.p = op_readstack();
  rd  = op_readstack();
  rd |= op_readstack() << 8;
  op_io();
  op_io();
  regs.pc = rd;
}

