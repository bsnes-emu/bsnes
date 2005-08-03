bra(0x2f, 0),
beq(0xf0, !regs.p.z),
bne(0xd0, regs.p.z),
bcs(0xb0, !regs.p.c),
bcc(0x90, regs.p.c),
bvs(0x70, !regs.p.v),
bvc(0x50, regs.p.v),
bmi(0x30, !regs.p.n),
bpl(0x10, regs.p.n) {
1:rd = op_read();
  if($1)end;
2:
3:regs.pc += (int8)rd;
}

bbs0(0x03, 0x01, !=),
bbc0(0x13, 0x01, ==),
bbs1(0x23, 0x02, !=),
bbc1(0x33, 0x02, ==),
bbs2(0x43, 0x04, !=),
bbc2(0x53, 0x04, ==),
bbs3(0x63, 0x08, !=),
bbc3(0x73, 0x08, ==),
bbs4(0x83, 0x10, !=),
bbc4(0x93, 0x10, ==),
bbs5(0xa3, 0x20, !=),
bbc5(0xb3, 0x20, ==),
bbs6(0xc3, 0x40, !=),
bbc6(0xd3, 0x40, ==),
bbs7(0xe3, 0x80, !=),
bbc7(0xf3, 0x80, ==) {
1:dp = op_read();
2:rd = op_read();
3:sp = op_read(OPMODE_DP, dp);
4:if((sp & $1) $2 $1)end;
5:
6:regs.pc += (int8)rd;
}

cbne_dp(0x2e, 0),
cbne_dpx(0xde, regs.x) {
1:dp = op_read();
2:rd = op_read();
3:sp = op_read(OPMODE_DP, dp + $1);
4:if(regs.a == sp)end;
5:
6:regs.pc += (int8)rd;
}

dbnz_dp(0x6e) {
1:dp = op_read();
2:rd = op_read();
3:wr = op_read(OPMODE_DP, dp);
4:wr--;
  op_write(OPMODE_DP, dp, wr);
  if(wr == 0x00)end;
5:
6:regs.pc += (int8)rd;
}

dbnz_y(0xfe) {
1:rd = op_read();
2:regs.y--;
3:if(regs.y == 0x00)end;
4:
5:regs.pc += (int8)rd;
}

jmp_addr(0x5f) {
1:rd  = op_read();
2:rd |= op_read() << 8;
  regs.pc = rd;
}

jmp_iaddrx(0x1f) {
1:dp  = op_read();
2:dp |= op_read() << 8;
3:dp += regs.x;
4:rd  = op_read(OPMODE_ADDR, dp);
5:rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
  regs.pc = rd;
}

call(0x3f) {
1:rd  = op_read();
2:rd |= op_read() << 8;
3:
4:
5:
6:stack_write(regs.pc >> 8);
7:stack_write(regs.pc);
  regs.pc = rd;
}

pcall(0x4f) {
1:rd = op_read();
2:
3:
4:stack_write(regs.pc >> 8);
5:stack_write(regs.pc);
  regs.pc = 0xff00 | rd;
}

tcall_0(0x01, 0),
tcall_1(0x11, 1),
tcall_2(0x21, 2),
tcall_3(0x31, 3),
tcall_4(0x41, 4),
tcall_5(0x51, 5),
tcall_6(0x61, 6),
tcall_7(0x71, 7),
tcall_8(0x81, 8),
tcall_9(0x91, 9),
tcall_10(0xa1, 10),
tcall_11(0xb1, 11),
tcall_12(0xc1, 12),
tcall_13(0xd1, 13),
tcall_14(0xe1, 14),
tcall_15(0xf1, 15) {
1:dp = 0xffde - ($1 << 1);
  rd  = op_read(OPMODE_ADDR, dp);
2:rd |= op_read(OPMODE_ADDR, dp + 1) << 8;
3:
4:
5:
6:stack_write(regs.pc >> 8);
7:stack_write(regs.pc);
  regs.pc = rd;
}

brk(0x0f) {
1:rd  = op_read(OPMODE_ADDR, 0xffde);
2:rd |= op_read(OPMODE_ADDR, 0xffdf) << 8;
3:
4:
5:stack_write(regs.pc >> 8);
6:stack_write(regs.pc);
7:stack_write(regs.p);
  regs.pc = rd;
  regs.p.b = 1;
  regs.p.i = 0;
}

ret(0x6f) {
1:rd  = stack_read();
2:rd |= stack_read() << 8;
3:
4:regs.pc = rd;
}

reti(0x7f) {
1:regs.p = stack_read();
2:rd  = stack_read();
3:rd |= stack_read() << 8;
4:
5:regs.pc = rd;
}
