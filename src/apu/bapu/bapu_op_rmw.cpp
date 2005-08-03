void bAPU::op_inc_a() {
  switch(status.cycle_pos++) {
  case 1:
    regs.a = op_inc(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_inc_x() {
  switch(status.cycle_pos++) {
  case 1:
    regs.x = op_inc(regs.x);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_inc_y() {
  switch(status.cycle_pos++) {
  case 1:
    regs.y = op_inc(regs.y);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_dec_a() {
  switch(status.cycle_pos++) {
  case 1:
    regs.a = op_dec(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_dec_x() {
  switch(status.cycle_pos++) {
  case 1:
    regs.x = op_dec(regs.x);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_dec_y() {
  switch(status.cycle_pos++) {
  case 1:
    regs.y = op_dec(regs.y);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_asl_a() {
  switch(status.cycle_pos++) {
  case 1:
    regs.a = op_asl(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_lsr_a() {
  switch(status.cycle_pos++) {
  case 1:
    regs.a = op_lsr(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_rol_a() {
  switch(status.cycle_pos++) {
  case 1:
    regs.a = op_rol(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_ror_a() {
  switch(status.cycle_pos++) {
  case 1:
    regs.a = op_ror(regs.a);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_inc_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd = op_inc(rd);
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_dec_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd = op_dec(rd);
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_asl_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd = op_asl(rd);
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_lsr_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd = op_lsr(rd);
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_rol_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd = op_rol(rd);
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_ror_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd = op_ror(rd);
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_inc_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    break;
  case 3:
    rd = op_read(OPMODE_DP, dp + regs.x);
    break;
  case 4:
    rd = op_inc(rd);
    op_write(OPMODE_DP, dp + regs.x, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_dec_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    break;
  case 3:
    rd = op_read(OPMODE_DP, dp + regs.x);
    break;
  case 4:
    rd = op_dec(rd);
    op_write(OPMODE_DP, dp + regs.x, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_asl_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    break;
  case 3:
    rd = op_read(OPMODE_DP, dp + regs.x);
    break;
  case 4:
    rd = op_asl(rd);
    op_write(OPMODE_DP, dp + regs.x, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_lsr_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    break;
  case 3:
    rd = op_read(OPMODE_DP, dp + regs.x);
    break;
  case 4:
    rd = op_lsr(rd);
    op_write(OPMODE_DP, dp + regs.x, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_rol_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    break;
  case 3:
    rd = op_read(OPMODE_DP, dp + regs.x);
    break;
  case 4:
    rd = op_rol(rd);
    op_write(OPMODE_DP, dp + regs.x, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_ror_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    break;
  case 3:
    rd = op_read(OPMODE_DP, dp + regs.x);
    break;
  case 4:
    rd = op_ror(rd);
    op_write(OPMODE_DP, dp + regs.x, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_inc_addr() {
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
    rd = op_inc(rd);
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_dec_addr() {
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
    rd = op_dec(rd);
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_asl_addr() {
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
    rd = op_asl(rd);
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_lsr_addr() {
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
    rd = op_lsr(rd);
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_rol_addr() {
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
    rd = op_rol(rd);
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_ror_addr() {
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
    rd = op_ror(rd);
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_incw_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd  = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |= op_read(OPMODE_DP, dp + 1) << 8;
    break;
  case 4:
    rd = op_incw(rd);
    op_write(OPMODE_DP, dp + 1, rd >> 8);
    break;
  case 5:
    op_write(OPMODE_DP, dp,     rd);
    status.cycle_pos = 0;
    break;
  }
}

void bAPU::op_decw_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    rd  = op_read(OPMODE_DP, dp);
    break;
  case 3:
    rd |= op_read(OPMODE_DP, dp + 1) << 8;
    break;
  case 4:
    rd = op_decw(rd);
    op_write(OPMODE_DP, dp + 1, rd >> 8);
    break;
  case 5:
    op_write(OPMODE_DP, dp,     rd);
    status.cycle_pos = 0;
    break;
  }
}

