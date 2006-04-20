void bAPU::op_bra() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(0)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_beq() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(!regs.p.z)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bne() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(regs.p.z)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bcs() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(!regs.p.c)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bcc() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(regs.p.c)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bvs() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(!regs.p.v)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bvc() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(regs.p.v)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bmi() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(!regs.p.n)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bpl() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    if(regs.p.n)status.cycle_pos = 0;
    } break;
  case 2: {
    } break;
  case 3: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs0() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x01) != 0x01)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc0() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x01) == 0x01)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs1() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x02) != 0x02)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc1() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x02) == 0x02)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs2() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x04) != 0x04)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc2() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x04) == 0x04)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs3() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x08) != 0x08)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc3() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x08) == 0x08)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs4() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x10) != 0x10)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc4() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x10) == 0x10)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs5() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x20) != 0x20)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc5() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x20) == 0x20)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs6() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x40) != 0x40)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc6() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x40) == 0x40)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbs7() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x80) != 0x80)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_bbc7() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if((sp & 0x80) == 0x80)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_cbne_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    sp = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    if(regs.a == sp)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_cbne_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    } break;
  case 4: {
    sp = op_read(OPMODE_DP, dp + regs.x);
    } break;
  case 5: {
    if(regs.a == sp)status.cycle_pos = 0;
    } break;
  case 6: {
    } break;
  case 7: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_dbnz_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    rd = op_read();
    } break;
  case 3: {
    wr = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    wr--;
    op_write(OPMODE_DP, dp, wr);
    if(wr == 0x00)status.cycle_pos = 0;
    } break;
  case 5: {
    } break;
  case 6: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_dbnz_y() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    regs.y--;
    } break;
  case 3: {
    if(regs.y == 0x00)status.cycle_pos = 0;
    } break;
  case 4: {
    } break;
  case 5: {
    regs.pc += (int8)rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_jmp_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    rd  = op_read();
    } break;
  case 2: {
    rd |= op_read() << 8;
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_jmp_iaddrx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    dp += regs.x;
    } break;
  case 4: {
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 5: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_call() {
  switch(status.cycle_pos++) {
  case 1: {
    rd  = op_read();
    } break;
  case 2: {
    rd |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_pcall() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    } break;
  case 4: {
    stack_write(regs.pc >> 8);
    } break;
  case 5: {
    stack_write(regs.pc);
    regs.pc = 0xff00 | rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_0() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (0 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_1() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (1 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_2() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (2 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_3() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (3 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_4() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (4 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_5() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (5 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_6() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (6 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_7() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (7 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_8() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (8 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_9() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (9 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_10() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (10 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_11() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (11 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_12() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (12 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_13() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (13 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_14() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (14 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_tcall_15() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = 0xffde - (15 << 1);
    rd  = op_read(OPMODE_ADDR, dp);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    } break;
  case 6: {
    stack_write(regs.pc >> 8);
    } break;
  case 7: {
    stack_write(regs.pc);
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_brk() {
  switch(status.cycle_pos++) {
  case 1: {
    rd  = op_read(OPMODE_ADDR, 0xffde);
    } break;
  case 2: {
    rd |= op_read(OPMODE_ADDR, 0xffdf) << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    stack_write(regs.pc >> 8);
    } break;
  case 6: {
    stack_write(regs.pc);
    } break;
  case 7: {
    stack_write(regs.p);
    regs.pc = rd;
    regs.p.b = 1;
    regs.p.i = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_ret() {
  switch(status.cycle_pos++) {
  case 1: {
    rd  = stack_read();
    } break;
  case 2: {
    rd |= stack_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_reti() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.p = stack_read();
    } break;
  case 2: {
    rd  = stack_read();
    } break;
  case 3: {
    rd |= stack_read() << 8;
    } break;
  case 4: {
    } break;
  case 5: {
    regs.pc = rd;
    status.cycle_pos = 0;
    } break;
  }
}

