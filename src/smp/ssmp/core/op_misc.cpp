void sSMP::op_nop() {
  op_io();
}

void sSMP::op_sleep() {
  op_io();
  op_io();
  regs.pc--;
}

void sSMP::op_stop() {
  op_io();
  op_io();
  regs.pc--;
}

void sSMP::op_xcn() {
  op_io();
  op_io();
  op_io();
  op_io();
  regs.a = (regs.a >> 4) | (regs.a << 4);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_daa() {
  op_io();
  op_io();
  if(regs.p.c || (regs.a) > 0x99) {
    regs.a += 0x60;
    regs.p.c = 1;
  }
  if(regs.p.h || (regs.a & 15) > 0x09) {
    regs.a += 0x06;
  }
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_das() {
  op_io();
  op_io();
  if(!regs.p.c || (regs.a) > 0x99) {
    regs.a -= 0x60;
    regs.p.c = 0;
  }
  if(!regs.p.h || (regs.a & 15) > 0x09) {
    regs.a -= 0x06;
  }
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_clrc() {
  op_io();
  regs.p.c = 0;
}

void sSMP::op_clrp() {
  op_io();
  regs.p.p = 0;
}

void sSMP::op_setc() {
  op_io();
  regs.p.c = 1;
}

void sSMP::op_setp() {
  op_io();
  regs.p.p = 1;
}

void sSMP::op_clrv() {
  op_io();
  regs.p.v = 0;
  regs.p.h = 0;
}

void sSMP::op_notc() {
  op_io();
  op_io();
  regs.p.c ^= 1;
}

void sSMP::op_ei() {
  op_io();
  op_io();
  regs.p.i = 1;
}

void sSMP::op_di() {
  op_io();
  op_io();
  regs.p.i = 0;
}

void sSMP::op_set0_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x01;
  op_writedp(dp, rd);
}

void sSMP::op_clr0_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x01;
  op_writedp(dp, rd);
}

void sSMP::op_set1_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x02;
  op_writedp(dp, rd);
}

void sSMP::op_clr1_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x02;
  op_writedp(dp, rd);
}

void sSMP::op_set2_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x04;
  op_writedp(dp, rd);
}

void sSMP::op_clr2_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x04;
  op_writedp(dp, rd);
}

void sSMP::op_set3_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x08;
  op_writedp(dp, rd);
}

void sSMP::op_clr3_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x08;
  op_writedp(dp, rd);
}

void sSMP::op_set4_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x10;
  op_writedp(dp, rd);
}

void sSMP::op_clr4_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x10;
  op_writedp(dp, rd);
}

void sSMP::op_set5_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x20;
  op_writedp(dp, rd);
}

void sSMP::op_clr5_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x20;
  op_writedp(dp, rd);
}

void sSMP::op_set6_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x40;
  op_writedp(dp, rd);
}

void sSMP::op_clr6_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x40;
  op_writedp(dp, rd);
}

void sSMP::op_set7_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd |=  0x80;
  op_writedp(dp, rd);
}

void sSMP::op_clr7_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd &= ~0x80;
  op_writedp(dp, rd);
}

void sSMP::op_push_a() {
  op_io();
  op_io();
  op_writestack(regs.a);
}

void sSMP::op_push_x() {
  op_io();
  op_io();
  op_writestack(regs.x);
}

void sSMP::op_push_y() {
  op_io();
  op_io();
  op_writestack(regs.y);
}

void sSMP::op_push_p() {
  op_io();
  op_io();
  op_writestack(regs.p);
}

void sSMP::op_pop_a() {
  op_io();
  op_io();
  regs.a = op_readstack();
}

void sSMP::op_pop_x() {
  op_io();
  op_io();
  regs.x = op_readstack();
}

void sSMP::op_pop_y() {
  op_io();
  op_io();
  regs.y = op_readstack();
}

void sSMP::op_pop_p() {
  op_io();
  op_io();
  regs.p = op_readstack();
}

void sSMP::op_mul_ya() {
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
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void sSMP::op_div_ya_x() {
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
  regs.p.v = !!(regs.y >= regs.x);
  regs.p.h = !!((regs.y & 15) >= (regs.x & 15));
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
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

