//opcode_functions.cpp was generated via bpp -> opcode_functions.bpp














































































//===============
//opcode_move.bpp
//===============

  void SMPcore::op_mov_a_x() {
    op_io();
    regs.a = regs.x;
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_a_y() {
    op_io();
    regs.a = regs.y;
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_x_a() {
    op_io();
    regs.x = regs.a;
    regs.p.n = (regs.x & 0x80);
    regs.p.z = (regs.x == 0);
  }

  void SMPcore::op_mov_y_a() {
    op_io();
    regs.y = regs.a;
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
  }

  void SMPcore::op_mov_x_sp() {
    op_io();
    regs.x = regs.sp;
    regs.p.n = (regs.x & 0x80);
    regs.p.z = (regs.x == 0);
  }

  void SMPcore::op_mov_sp_x() {
    op_io();
    regs.sp = regs.x;
  }


  void SMPcore::op_mov_a_const() {
    regs.a = op_readpc();
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_x_const() {
    regs.x = op_readpc();
    regs.p.n = (regs.x & 0x80);
    regs.p.z = (regs.x == 0);
  }

  void SMPcore::op_mov_y_const() {
    regs.y = op_readpc();
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
  }


  void SMPcore::op_mov_a_ix() {
    op_io();
    regs.a = op_readdp(regs.x);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_a_ixinc() {
    op_io();
    regs.a = op_readdp(regs.x++);
    op_io();
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }


  void SMPcore::op_mov_a_dp() {
    sp = op_readpc();
    regs.a = op_readdp(sp);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_x_dp() {
    sp = op_readpc();
    regs.x = op_readdp(sp);
    regs.p.n = (regs.x & 0x80);
    regs.p.z = (regs.x == 0);
  }

  void SMPcore::op_mov_y_dp() {
    sp = op_readpc();
    regs.y = op_readdp(sp);
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
  }


  void SMPcore::op_mov_a_dpx() {
    sp = op_readpc();
    op_io();
    regs.a = op_readdp(sp + regs.x);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_x_dpy() {
    sp = op_readpc();
    op_io();
    regs.x = op_readdp(sp + regs.y);
    regs.p.n = (regs.x & 0x80);
    regs.p.z = (regs.x == 0);
  }

  void SMPcore::op_mov_y_dpx() {
    sp = op_readpc();
    op_io();
    regs.y = op_readdp(sp + regs.x);
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
  }


  void SMPcore::op_mov_a_addr() {
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    regs.a = op_readaddr(sp);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_x_addr() {
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    regs.x = op_readaddr(sp);
    regs.p.n = (regs.x & 0x80);
    regs.p.z = (regs.x == 0);
  }

  void SMPcore::op_mov_y_addr() {
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    regs.y = op_readaddr(sp);
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
  }


  void SMPcore::op_mov_a_addrx() {
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    op_io();
    regs.a = op_readaddr(sp + regs.x);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_a_addry() {
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    op_io();
    regs.a = op_readaddr(sp + regs.y);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }


  void SMPcore::op_mov_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    regs.a = op_readaddr(sp);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    regs.a = op_readaddr(sp + regs.y);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_mov_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    op_writedp(dp, rd);
  }

  void SMPcore::op_mov_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp, rd);
  }

  void SMPcore::op_mov_ix_a() {
    op_io();
    op_readdp(regs.x);
    op_writedp(regs.x, regs.a);
  }

  void SMPcore::op_mov_ixinc_a() {
    op_io();
    op_io();
    op_writedp(regs.x++, regs.a);
  }


  void SMPcore::op_mov_dp_a() {
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp, regs.a);
  }

  void SMPcore::op_mov_dp_x() {
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp, regs.x);
  }

  void SMPcore::op_mov_dp_y() {
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp, regs.y);
  }


  void SMPcore::op_mov_dpx_a() {
    dp  = op_readpc();
    op_io();
    dp += regs.x;
    op_readdp(dp);
    op_writedp(dp, regs.a);
  }

  void SMPcore::op_mov_dpy_x() {
    dp  = op_readpc();
    op_io();
    dp += regs.y;
    op_readdp(dp);
    op_writedp(dp, regs.x);
  }

  void SMPcore::op_mov_dpx_y() {
    dp  = op_readpc();
    op_io();
    dp += regs.x;
    op_readdp(dp);
    op_writedp(dp, regs.y);
  }


  void SMPcore::op_mov_addr_a() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_readaddr(dp);
    op_writeaddr(dp, regs.a);
  }

  void SMPcore::op_mov_addr_x() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_readaddr(dp);
    op_writeaddr(dp, regs.x);
  }

  void SMPcore::op_mov_addr_y() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_readaddr(dp);
    op_writeaddr(dp, regs.y);
  }


  void SMPcore::op_mov_addrx_a() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    dp += regs.x;
    op_readaddr(dp);
    op_writeaddr(dp, regs.a);
  }

  void SMPcore::op_mov_addry_a() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    dp += regs.y;
    op_readaddr(dp);
    op_writeaddr(dp, regs.a);
  }


  void SMPcore::op_mov_idpx_a() {
    sp  = op_readpc();
    op_io();
    sp += regs.x;
    dp  = op_readdp(sp + 0) << 0;
    dp |= op_readdp(sp + 1) << 8;
    op_readaddr(dp);
    op_writeaddr(dp, regs.a);
  }

  void SMPcore::op_mov_idpy_a() {
    sp  = op_readpc();
    dp  = op_readdp(sp + 0) << 0;
    dp |= op_readdp(sp + 1) << 8;
    op_io();
    dp += regs.y;
    op_readaddr(dp);
    op_writeaddr(dp, regs.a);
  }


  void SMPcore::op_movw_ya_dp() {
    sp = op_readpc();
    regs.a = op_readdp(sp + 0);
    op_io();
    regs.y = op_readdp(sp + 1);
    regs.p.n = (regs.ya & 0x8000);
    regs.p.z = (regs.ya == 0);
  }

  void SMPcore::op_movw_dp_ya() {
    dp = op_readpc();
    op_readdp(dp);
    op_writedp(dp + 0, regs.a);
    op_writedp(dp + 1, regs.y);
  }


  void SMPcore::op_mov1_c_bit() {
    sp  = op_readpc() << 0;
    sp |= op_readpc() << 8;
    bit = sp >> 13;
    sp &= 0x1fff;
    rd = op_readaddr(sp);
    regs.p.c = !!(rd & (1 << bit));
  }

  void SMPcore::op_mov1_bit_c() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_readaddr(dp);
    if(regs.p.c) rd |=  (1 << bit);
    else         rd &= ~(1 << bit);
    op_io();
    op_writeaddr(dp, rd);
  }


//=============
//opcode_pc.bpp
//=============

  void SMPcore::op_bra() {
    rd = op_readpc();
    if(0) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_beq() {
    rd = op_readpc();
    if(!regs.p.z) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bne() {
    rd = op_readpc();
    if(regs.p.z) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bcs() {
    rd = op_readpc();
    if(!regs.p.c) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bcc() {
    rd = op_readpc();
    if(regs.p.c) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bvs() {
    rd = op_readpc();
    if(!regs.p.v) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bvc() {
    rd = op_readpc();
    if(regs.p.v) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bmi() {
    rd = op_readpc();
    if(!regs.p.n) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bpl() {
    rd = op_readpc();
    if(regs.p.n) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }


  void SMPcore::op_bbs0() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x01) != 0x01) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc0() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x01) == 0x01) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs1() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x02) != 0x02) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc1() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x02) == 0x02) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs2() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x04) != 0x04) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc2() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x04) == 0x04) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs3() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x08) != 0x08) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc3() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x08) == 0x08) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs4() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x10) != 0x10) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc4() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x10) == 0x10) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs5() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x20) != 0x20) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc5() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x20) == 0x20) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs6() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x40) != 0x40) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc6() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x40) == 0x40) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbs7() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x80) != 0x80) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_bbc7() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if((sp & 0x80) == 0x80) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }


  void SMPcore::op_cbne_dp() {
    dp = op_readpc();
    sp = op_readdp(dp);
    rd = op_readpc();
    op_io();
    if(regs.a == sp) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_cbne_dpx() {
    dp = op_readpc();
    op_io();
    sp = op_readdp(dp + regs.x);
    rd = op_readpc();
    op_io();
    if(regs.a == sp) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_dbnz_dp() {
    dp = op_readpc();
    wr = op_readdp(dp);
    op_writedp(dp, --wr);
    rd = op_readpc();
    if(wr == 0) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_dbnz_y() {
    rd = op_readpc();
    op_io();
    regs.y--;
    op_io();
    if(regs.y == 0) return;
    op_io();
    op_io();
    regs.pc += (int8_t)rd;
  }

  void SMPcore::op_jmp_addr() {
    rd  = op_readpc() << 0;
    rd |= op_readpc() << 8;
    regs.pc = rd;
  }

  void SMPcore::op_jmp_iaddrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    dp += regs.x;
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    regs.pc = rd;
  }

  void SMPcore::op_call() {
    rd  = op_readpc() << 0;
    rd |= op_readpc() << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_pcall() {
    rd = op_readpc();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = 0xff00 | rd;
  }


  void SMPcore::op_tcall_0() {
    dp  = 0xffde - (0 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_1() {
    dp  = 0xffde - (1 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_2() {
    dp  = 0xffde - (2 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_3() {
    dp  = 0xffde - (3 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_4() {
    dp  = 0xffde - (4 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_5() {
    dp  = 0xffde - (5 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_6() {
    dp  = 0xffde - (6 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_7() {
    dp  = 0xffde - (7 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_8() {
    dp  = 0xffde - (8 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_9() {
    dp  = 0xffde - (9 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_10() {
    dp  = 0xffde - (10 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_11() {
    dp  = 0xffde - (11 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_12() {
    dp  = 0xffde - (12 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_13() {
    dp  = 0xffde - (13 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_14() {
    dp  = 0xffde - (14 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }

  void SMPcore::op_tcall_15() {
    dp  = 0xffde - (15 << 1);
    rd  = op_readaddr(dp + 0) << 0;
    rd |= op_readaddr(dp + 1) << 8;
    op_io();
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    regs.pc = rd;
  }


  void SMPcore::op_brk() {
    rd  = op_readaddr(0xffde) << 0;
    rd |= op_readaddr(0xffdf) << 8;
    op_io();
    op_io();
    op_writestack(regs.pc >> 8);
    op_writestack(regs.pc >> 0);
    op_writestack(regs.p);
    regs.pc = rd;
    regs.p.b = 1;
    regs.p.i = 0;
  }

  void SMPcore::op_ret() {
    rd  = op_readstack() << 0;
    rd |= op_readstack() << 8;
    op_io();
    op_io();
    regs.pc = rd;
  }

  void SMPcore::op_reti() {
    regs.p = op_readstack();
    rd  = op_readstack() << 0;
    rd |= op_readstack() << 8;
    op_io();
    op_io();
    regs.pc = rd;
  }


//===============
//opcode_read.bpp
//===============

  void SMPcore::op_adc_a_const() {
    rd = op_readpc();
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_const() {
    rd = op_readpc();
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_const() {
    rd = op_readpc();
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_cmp_x_const() {
    rd = op_readpc();
    regs.x = op_cmp(regs.x, rd);
  }

  void SMPcore::op_cmp_y_const() {
    rd = op_readpc();
    regs.y = op_cmp(regs.y, rd);
  }

  void SMPcore::op_eor_a_const() {
    rd = op_readpc();
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_const() {
    rd = op_readpc();
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_const() {
    rd = op_readpc();
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_ix() {
    op_io();
    rd = op_readdp(regs.x);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_ix() {
    op_io();
    rd = op_readdp(regs.x);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_ix() {
    op_io();
    rd = op_readdp(regs.x);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_eor_a_ix() {
    op_io();
    rd = op_readdp(regs.x);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_ix() {
    op_io();
    rd = op_readdp(regs.x);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_ix() {
    op_io();
    rd = op_readdp(regs.x);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_cmp_x_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.x = op_cmp(regs.x, rd);
  }

  void SMPcore::op_cmp_y_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.y = op_cmp(regs.y, rd);
  }

  void SMPcore::op_eor_a_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_eor_a_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_cmp_x_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.x = op_cmp(regs.x, rd);
  }

  void SMPcore::op_cmp_y_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.y = op_cmp(regs.y, rd);
  }

  void SMPcore::op_eor_a_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_addrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.x);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_adc_a_addry() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.y);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_addrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.x);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_and_a_addry() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.y);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_addrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.x);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_cmp_a_addry() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.y);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_eor_a_addrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.x);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_eor_a_addry() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.y);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_addrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.x);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_or_a_addry() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.y);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_addrx() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.x);
    regs.a = op_sbc(regs.a, rd);
  }

  void SMPcore::op_sbc_a_addry() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    op_io();
    rd  = op_readaddr(dp + regs.y);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_eor_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_idpx() {
    dp  = op_readpc() + regs.x;
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp + regs.y);
    regs.a = op_adc(regs.a, rd);
  }

  void SMPcore::op_and_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp + regs.y);
    regs.a = op_and(regs.a, rd);
  }

  void SMPcore::op_cmp_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp + regs.y);
    regs.a = op_cmp(regs.a, rd);
  }

  void SMPcore::op_eor_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp + regs.y);
    regs.a = op_eor(regs.a, rd);
  }

  void SMPcore::op_or_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp + regs.y);
    regs.a = op_or(regs.a, rd);
  }

  void SMPcore::op_sbc_a_idpy() {
    dp  = op_readpc();
    op_io();
    sp  = op_readdp(dp + 0) << 0;
    sp |= op_readdp(dp + 1) << 8;
    rd  = op_readaddr(sp + regs.y);
    regs.a = op_sbc(regs.a, rd);
  }


  void SMPcore::op_adc_ix_iy() {
    op_io();
    rd = op_readdp(regs.y);
    wr = op_readdp(regs.x);
    wr = op_adc(wr, rd);
    true ? op_writedp(regs.x, wr) : op_io();
  }

  void SMPcore::op_and_ix_iy() {
    op_io();
    rd = op_readdp(regs.y);
    wr = op_readdp(regs.x);
    wr = op_and(wr, rd);
    true ? op_writedp(regs.x, wr) : op_io();
  }

  void SMPcore::op_cmp_ix_iy() {
    op_io();
    rd = op_readdp(regs.y);
    wr = op_readdp(regs.x);
    wr = op_cmp(wr, rd);
    false ? op_writedp(regs.x, wr) : op_io();
  }

  void SMPcore::op_eor_ix_iy() {
    op_io();
    rd = op_readdp(regs.y);
    wr = op_readdp(regs.x);
    wr = op_eor(wr, rd);
    true ? op_writedp(regs.x, wr) : op_io();
  }

  void SMPcore::op_or_ix_iy() {
    op_io();
    rd = op_readdp(regs.y);
    wr = op_readdp(regs.x);
    wr = op_or(wr, rd);
    true ? op_writedp(regs.x, wr) : op_io();
  }

  void SMPcore::op_sbc_ix_iy() {
    op_io();
    rd = op_readdp(regs.y);
    wr = op_readdp(regs.x);
    wr = op_sbc(wr, rd);
    true ? op_writedp(regs.x, wr) : op_io();
  }


  void SMPcore::op_adc_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_adc(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_and_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_and(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_cmp_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_cmp(wr, rd);
    false ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_eor_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_eor(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_or_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_or(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_sbc_dp_dp() {
    sp = op_readpc();
    rd = op_readdp(sp);
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_sbc(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }


  void SMPcore::op_adc_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_adc(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_and_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_and(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_cmp_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_cmp(wr, rd);
    false ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_eor_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_eor(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_or_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_or(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }

  void SMPcore::op_sbc_dp_const() {
    rd = op_readpc();
    dp = op_readpc();
    wr = op_readdp(dp);
    wr = op_sbc(wr, rd);
    true ? op_writedp(dp, wr) : op_io();
  }


  void SMPcore::op_addw_ya_dp() {
    dp  = op_readpc();
    rd  = op_readdp(dp + 0) << 0;
    op_io();
    rd |= op_readdp(dp + 1) << 8;
    regs.ya = op_addw(regs.ya, rd);
  }

  void SMPcore::op_subw_ya_dp() {
    dp  = op_readpc();
    rd  = op_readdp(dp + 0) << 0;
    op_io();
    rd |= op_readdp(dp + 1) << 8;
    regs.ya = op_subw(regs.ya, rd);
  }

  void SMPcore::op_cmpw_ya_dp() {
    dp  = op_readpc();
    rd  = op_readdp(dp + 0) << 0;
    rd |= op_readdp(dp + 1) << 8;
    op_cmpw(regs.ya, rd);
  }


  void SMPcore::op_and1_bit() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_readaddr(dp);
    regs.p.c = regs.p.c & !!(rd & (1 << bit));
  }

  void SMPcore::op_and1_notbit() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_readaddr(dp);
    regs.p.c = regs.p.c & !(rd & (1 << bit));
  }

  void SMPcore::op_eor1_bit() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_readaddr(dp);
    op_io();
    regs.p.c = regs.p.c ^ !!(rd & (1 << bit));
  }

  void SMPcore::op_not1_bit() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_readaddr(dp);
    rd ^= (1 << bit);
    op_writeaddr(dp, rd);
  }

  void SMPcore::op_or1_bit() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_readaddr(dp);
    op_io();
    regs.p.c = regs.p.c | !!(rd & (1 << bit));
  }

  void SMPcore::op_or1_notbit() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    bit = dp >> 13;
    dp &= 0x1fff;
    rd  = op_readaddr(dp);
    op_io();
    regs.p.c = regs.p.c | !(rd & (1 << bit));
  }


//==============
//opcode_rmw.bpp
//==============

  void SMPcore::op_inc_a() {
    op_io();
    regs.a = op_inc(regs.a);
  }

  void SMPcore::op_inc_x() {
    op_io();
    regs.x = op_inc(regs.x);
  }

  void SMPcore::op_inc_y() {
    op_io();
    regs.y = op_inc(regs.y);
  }

  void SMPcore::op_dec_a() {
    op_io();
    regs.a = op_dec(regs.a);
  }

  void SMPcore::op_dec_x() {
    op_io();
    regs.x = op_dec(regs.x);
  }

  void SMPcore::op_dec_y() {
    op_io();
    regs.y = op_dec(regs.y);
  }

  void SMPcore::op_asl_a() {
    op_io();
    regs.a = op_asl(regs.a);
  }

  void SMPcore::op_lsr_a() {
    op_io();
    regs.a = op_lsr(regs.a);
  }

  void SMPcore::op_rol_a() {
    op_io();
    regs.a = op_rol(regs.a);
  }

  void SMPcore::op_ror_a() {
    op_io();
    regs.a = op_ror(regs.a);
  }


  void SMPcore::op_inc_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd = op_inc(rd);
    op_writedp(dp, rd);
  }

  void SMPcore::op_dec_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd = op_dec(rd);
    op_writedp(dp, rd);
  }

  void SMPcore::op_asl_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd = op_asl(rd);
    op_writedp(dp, rd);
  }

  void SMPcore::op_lsr_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd = op_lsr(rd);
    op_writedp(dp, rd);
  }

  void SMPcore::op_rol_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd = op_rol(rd);
    op_writedp(dp, rd);
  }

  void SMPcore::op_ror_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd = op_ror(rd);
    op_writedp(dp, rd);
  }


  void SMPcore::op_inc_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    rd = op_inc(rd);
    op_writedp(dp + regs.x, rd);
  }

  void SMPcore::op_dec_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    rd = op_dec(rd);
    op_writedp(dp + regs.x, rd);
  }

  void SMPcore::op_asl_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    rd = op_asl(rd);
    op_writedp(dp + regs.x, rd);
  }

  void SMPcore::op_lsr_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    rd = op_lsr(rd);
    op_writedp(dp + regs.x, rd);
  }

  void SMPcore::op_rol_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    rd = op_rol(rd);
    op_writedp(dp + regs.x, rd);
  }

  void SMPcore::op_ror_dpx() {
    dp = op_readpc();
    op_io();
    rd = op_readdp(dp + regs.x);
    rd = op_ror(rd);
    op_writedp(dp + regs.x, rd);
  }


  void SMPcore::op_inc_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    rd  = op_inc(rd);
    op_writeaddr(dp, rd);
  }

  void SMPcore::op_dec_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    rd  = op_dec(rd);
    op_writeaddr(dp, rd);
  }

  void SMPcore::op_asl_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    rd  = op_asl(rd);
    op_writeaddr(dp, rd);
  }

  void SMPcore::op_lsr_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    rd  = op_lsr(rd);
    op_writeaddr(dp, rd);
  }

  void SMPcore::op_rol_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    rd  = op_rol(rd);
    op_writeaddr(dp, rd);
  }

  void SMPcore::op_ror_addr() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    rd  = op_ror(rd);
    op_writeaddr(dp, rd);
  }


  void SMPcore::op_tset_addr_a() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.p.n = ((regs.a - rd) & 0x80);
    regs.p.z = ((regs.a - rd) == 0);
    op_readaddr(dp);
    op_writeaddr(dp, rd | regs.a);
  }

  void SMPcore::op_tclr_addr_a() {
    dp  = op_readpc() << 0;
    dp |= op_readpc() << 8;
    rd  = op_readaddr(dp);
    regs.p.n = ((regs.a - rd) & 0x80);
    regs.p.z = ((regs.a - rd) == 0);
    op_readaddr(dp);
    op_writeaddr(dp, rd &~ regs.a);
  }


  void SMPcore::op_incw_dp() {
    dp  = op_readpc();
    rd  = op_readdp(dp);
    rd ++;
    op_writedp(dp++, rd);
    rd += op_readdp(dp) << 8;
    op_writedp(dp, rd >> 8);
    regs.p.n = (rd & 0x8000);
    regs.p.z = (rd == 0);
  }

  void SMPcore::op_decw_dp() {
    dp  = op_readpc();
    rd  = op_readdp(dp);
    rd --;
    op_writedp(dp++, rd);
    rd += op_readdp(dp) << 8;
    op_writedp(dp, rd >> 8);
    regs.p.n = (rd & 0x8000);
    regs.p.z = (rd == 0);
  }


//===============
//opcode_misc.bpp
//===============

  void SMPcore::op_nop() {
    op_io();
  }

  void SMPcore::op_sleep() {
    op_io();
    op_io();
    regs.pc--;
  }

  void SMPcore::op_stop() {
    op_io();
    op_io();
    regs.pc--;
  }

  void SMPcore::op_xcn() {
    op_io();
    op_io();
    op_io();
    op_io();
    regs.a = (regs.a >> 4) | (regs.a << 4);
    regs.p.n = (regs.a & 0x80);
    regs.p.z = (regs.a == 0);
  }

  void SMPcore::op_daa() {
    op_io();
    op_io();
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

  void SMPcore::op_das() {
    op_io();
    op_io();
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


  void SMPcore::op_clrc() {
    op_io();
    regs.p.c=0;
  }

  void SMPcore::op_clrp() {
    op_io();
    regs.p.p=0;
  }

  void SMPcore::op_setc() {
    op_io();
    regs.p.c=1;
  }

  void SMPcore::op_setp() {
    op_io();
    regs.p.p=1;
  }

  void SMPcore::op_clrv() {
    op_io();
    regs.p.v=regs.p.h=0;
  }

  void SMPcore::op_notc() {
    op_io();
    op_io();
    regs.p.c = !regs.p.c;
  }


  void SMPcore::op_ei() {
    op_io();
    op_io();
    regs.p.i = 1;
  }

  void SMPcore::op_di() {
    op_io();
    op_io();
    regs.p.i = 0;
  }


  void SMPcore::op_set0_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x01;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr0_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x01;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set1_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x02;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr1_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x02;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set2_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x04;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr2_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x04;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set3_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x08;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr3_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x08;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set4_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x10;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr4_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x10;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set5_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x20;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr5_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x20;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set6_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x40;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr6_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x40;
    op_writedp(dp, rd);
  }

  void SMPcore::op_set7_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd |=0x80;
    op_writedp(dp, rd);
  }

  void SMPcore::op_clr7_dp() {
    dp = op_readpc();
    rd = op_readdp(dp);
    rd &=~0x80;
    op_writedp(dp, rd);
  }


  void SMPcore::op_push_a() {
    op_io();
    op_io();
    op_writestack(regs.a);
  }

  void SMPcore::op_push_x() {
    op_io();
    op_io();
    op_writestack(regs.x);
  }

  void SMPcore::op_push_y() {
    op_io();
    op_io();
    op_writestack(regs.y);
  }

  void SMPcore::op_push_p() {
    op_io();
    op_io();
    op_writestack(regs.p);
  }


  void SMPcore::op_pop_a() {
    op_io();
    op_io();
    regs.a = op_readstack();
  }

  void SMPcore::op_pop_x() {
    op_io();
    op_io();
    regs.x = op_readstack();
  }

  void SMPcore::op_pop_y() {
    op_io();
    op_io();
    regs.y = op_readstack();
  }

  void SMPcore::op_pop_p() {
    op_io();
    op_io();
    regs.p = op_readstack();
  }


  void SMPcore::op_mul_ya() {
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    ya = regs.y * regs.a;
    regs.a = ya;
    regs.y = ya >> 8;
    //result is set based on y (high-byte) only
    regs.p.n = (regs.y & 0x80);
    regs.p.z = (regs.y == 0);
  }

  void SMPcore::op_div_ya_x() {
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
    op_io();
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



