void bAPU::op_nop() {
  switch(status.cycle_pos++) {
  case 1:
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_sleep() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    regs.pc--;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_stop() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    regs.pc--;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_xcn() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    regs.a = (regs.a >> 4) | (regs.a << 4);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_daa() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    if(regs.p.c || (regs.a) > 0x99) {
      regs.a += 0x60;
      regs.p.c = 1;
    }
    if(regs.p.h || (regs.a & 15) > 0x09) {
      regs.a += 0x06;
    }
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_das() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    if(!regs.p.c || (regs.a) > 0x99) {
      regs.a -= 0x60;
      regs.p.c = 0;
    }
    if(!regs.p.h || (regs.a & 15) > 0x09) {
      regs.a -= 0x06;
    }
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clrc() {
  switch(status.cycle_pos++) {
  case 1:
    regs.p.c = 0;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clrp() {
  switch(status.cycle_pos++) {
  case 1:
    regs.p.p = 0;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_setc() {
  switch(status.cycle_pos++) {
  case 1:
    regs.p.c = 1;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_setp() {
  switch(status.cycle_pos++) {
  case 1:
    regs.p.p = 1;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clrv() {
  switch(status.cycle_pos++) {
  case 1:
    regs.p.v = 0;
    regs.p.h = 0;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_notc() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    regs.p.c ^= 1;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_ei() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    regs.p.i = 1;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_di() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    regs.p.i = 0;
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set0_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x01;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr0_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x01;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set1_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x02;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr1_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x02;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set2_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x04;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr2_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x04;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set3_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x08;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr3_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x08;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set4_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x10;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr4_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x10;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set5_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x20;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr5_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x20;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set6_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x40;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr6_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x40;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_set7_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |=  0x80;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_clr7_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd &= ~0x80;
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_tset_addr_a() {
  switch(status.cycle_pos++) {
  case 1:
    dp  = op_read();
    break;
  case 2:
    dp |= op_read() << 8;
    break;
  case 3:
    rd = op_read(OPMODE_ADDR, dp);
    break;
  case 4:
    regs.p.n = !!((rd & regs.a) & 0x80);
    regs.p.z = ((rd & regs.a) == 0);
    rd |= regs.a;
    break;
  case 5:
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_tclr_addr_a() {
  switch(status.cycle_pos++) {
  case 1:
    dp  = op_read();
    break;
  case 2:
    dp |= op_read() << 8;
    break;
  case 3:
    rd = op_read(OPMODE_ADDR, dp);
    break;
  case 4:
    regs.p.n = !!((rd & regs.a) & 0x80);
    regs.p.z = ((rd & regs.a) == 0);
    rd &=~ regs.a;
    break;
  case 5:
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_push_a() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    stack_write(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_push_x() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    stack_write(regs.x);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_push_y() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    stack_write(regs.y);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_push_p() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    stack_write(regs.p);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_pop_a() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    regs.a = stack_read();
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_pop_x() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    regs.x = stack_read();
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_pop_y() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    regs.y = stack_read();
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_pop_p() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    regs.p = stack_read();
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_mul_ya() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    break;
  case 7:
    break;
  case 8:
    ya = regs.y * regs.a;
    regs.a = ya;
    regs.y = ya >> 8;
  //result is set based on y (high-byte) only
    regs.p.n = !!(regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_div_ya_x() {
  switch(status.cycle_pos++) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    break;
  case 5:
    break;
  case 6:
    break;
  case 7:
    break;
  case 8:
    break;
  case 9:
    break;
  case 10:
    break;
  case 11:
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
    //this emulates the odd behavior of the SPC700 in this case
      regs.a = 255    - (ya - (regs.x << 9)) / (256 - regs.x);
      regs.y = regs.x + (ya - (regs.x << 9)) % (256 - regs.x);
    }
  //result is set based on a (quotient) only
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    break;
  }
}

