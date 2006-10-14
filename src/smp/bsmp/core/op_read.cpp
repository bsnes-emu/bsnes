void bSMP::op_adc_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_x_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.x = op_cmp(regs.x, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_y_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.y = op_cmp(regs.y, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_x_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.x = op_cmp(regs.x, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_y_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.y = op_cmp(regs.y, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, dp);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, dp + regs.x);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, dp + regs.x);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, dp + regs.x);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, dp + regs.x);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, dp + regs.x);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, dp + regs.x);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_x_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.x = op_cmp(regs.x, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_y_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.y = op_cmp(regs.y, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    rd = op_read(OPMODE_ADDR, dp);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.x);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.y);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.x);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.y);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.x);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.y);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.x);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.y);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.x);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.y);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.x);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    rd = op_read(OPMODE_ADDR, dp + regs.y);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp + regs.y);
    regs.a = op_adc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp + regs.y);
    regs.a = op_and(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp + regs.y);
    regs.a = op_cmp(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp + regs.y);
    regs.a = op_eor(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp + regs.y);
    regs.a = op_or(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    rd = op_read(OPMODE_ADDR, sp + regs.y);
    regs.a = op_sbc(regs.a, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_ix_iy() {
  switch(status.cycle_pos++) {
  case 1: {
    wr = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, regs.y);
    } break;
  case 3: {
    wr = op_adc(wr, rd);
    } break;
  case 4: {
    if(1)op_write(OPMODE_DP, regs.x, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_ix_iy() {
  switch(status.cycle_pos++) {
  case 1: {
    wr = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, regs.y);
    } break;
  case 3: {
    wr = op_and(wr, rd);
    } break;
  case 4: {
    if(1)op_write(OPMODE_DP, regs.x, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_ix_iy() {
  switch(status.cycle_pos++) {
  case 1: {
    wr = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, regs.y);
    } break;
  case 3: {
    wr = op_cmp(wr, rd);
    } break;
  case 4: {
    if(0)op_write(OPMODE_DP, regs.x, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_ix_iy() {
  switch(status.cycle_pos++) {
  case 1: {
    wr = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, regs.y);
    } break;
  case 3: {
    wr = op_eor(wr, rd);
    } break;
  case 4: {
    if(1)op_write(OPMODE_DP, regs.x, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_ix_iy() {
  switch(status.cycle_pos++) {
  case 1: {
    wr = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, regs.y);
    } break;
  case 3: {
    wr = op_or(wr, rd);
    } break;
  case 4: {
    if(1)op_write(OPMODE_DP, regs.x, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_ix_iy() {
  switch(status.cycle_pos++) {
  case 1: {
    wr = op_read(OPMODE_DP, regs.x);
    } break;
  case 2: {
    rd = op_read(OPMODE_DP, regs.y);
    } break;
  case 3: {
    wr = op_sbc(wr, rd);
    } break;
  case 4: {
    if(1)op_write(OPMODE_DP, regs.x, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 5: {
    wr = op_adc(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 5: {
    wr = op_and(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 5: {
    wr = op_cmp(wr, rd);
    if(0)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 5: {
    wr = op_eor(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 5: {
    wr = op_or(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 5: {
    wr = op_sbc(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_adc_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr = op_adc(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr = op_and(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmp_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr = op_cmp(wr, rd);
    if(0)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr = op_eor(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr = op_or(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_sbc_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr = op_sbc(wr, rd);
    if(1)op_write(OPMODE_DP, dp, wr);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_addw_ya_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    rd  = op_read(OPMODE_DP, dp);
    } break;
  case 3: {
    rd |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 4: {
    regs.ya = op_addw(regs.ya, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_cmpw_ya_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    rd  = op_read(OPMODE_DP, dp);
    } break;
  case 3: {
    rd |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 4: {
    regs.ya = op_cmpw(regs.ya, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_subw_ya_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    rd  = op_read(OPMODE_DP, dp);
    } break;
  case 3: {
    rd |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 4: {
    regs.ya = op_subw(regs.ya, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and1_bit() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    regs.p.c = regs.p.c & !!(rd & (1 << bit));
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_and1_notbit() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    regs.p.c = regs.p.c & !(rd & (1 << bit));
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_eor1_bit() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    } break;
  case 4: {
    regs.p.c = regs.p.c ^ !!(rd & (1 << bit));
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_not1_bit() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    rd ^= (1 << bit);
    } break;
  case 4: {
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or1_bit() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    } break;
  case 4: {
    regs.p.c = regs.p.c | !!(rd & (1 << bit));
    status.cycle_pos = 0;
    } break;
  }
}

void bSMP::op_or1_notbit() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    } break;
  case 4: {
    regs.p.c = regs.p.c | !(rd & (1 << bit));
    status.cycle_pos = 0;
    } break;
  }
}

