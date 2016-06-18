auto R6502::op_branch(bool condition) {
  if(!condition) {
L   rd = readPC();
  } else {
    rd = readPC();
    aa = regs.pc + (int8)rd;
    ioPage(regs.pc, aa);
L   io();
    regs.pc = aa;
  }
}

auto R6502::op_clear_flag(uint bit) {
L io();
  regs.p &= ~(1 << bit);
}

auto R6502::op_decrement(uint8& r) {
L io();
  r--;
  regs.p.n = (r & 0x80);
  regs.p.z = (r == 0);
}

auto R6502::op_increment(uint8& r) {
L io();
  r++;
  regs.p.n = (r & 0x80);
  regs.p.z = (r == 0);
}

auto R6502::op_pull(uint8& r) {
  io();
  io();
L r = readSP();
  regs.p.n = (r & 0x80);
  regs.p.z = (r == 0);
}

auto R6502::op_push(uint8& r) {
  io();
L writeSP(r);
}

auto R6502::op_read_absolute(fp op) {
  abs.l = readPC();
  abs.h = readPC();
L rd = read(abs.w);
  call(op);
}

auto R6502::op_read_absolute_x(fp op) {
  abs.l = readPC();
  abs.h = readPC();
  ioPage(abs.w, abs.w + regs.x);
L rd = read(abs.w + regs.x);
  call(op);
}

auto R6502::op_read_absolute_y(fp op) {
  abs.l = readPC();
  abs.h = readPC();
  ioPage(abs.w, abs.w + regs.y);
L rd = read(abs.w + regs.y);
  call(op);
}

auto R6502::op_read_immediate(fp op) {
L rd = readPC();
  call(op);
}

auto R6502::op_read_indirect_zero_page_x(fp op) {
  zp = readPC();
  readZP(zp);
  abs.l = readZP(zp++ + regs.x);
  abs.h = readZP(zp++ + regs.x);
L rd = read(abs.w);
  call(op);
}

auto R6502::op_read_indirect_zero_page_y(fp op) {
  rd = readPC();
  abs.l = readZP(rd++);
  abs.h = readZP(rd++);
  ioPage(abs.w, abs.w + regs.y);
L rd = read(abs.w + regs.y);
  call(op);
}

auto R6502::op_read_zero_page(fp op) {
  zp = readPC();
L rd = readZP(zp);
  call(op);
}

auto R6502::op_read_zero_page_x(fp op) {
  zp = readPC();
  readZP(zp);
L rd = readZP(zp + regs.x);
  call(op);
}

auto R6502::op_read_zero_page_y(fp op) {
  zp = readPC();
  readZP(zp);
L rd = readZP(zp + regs.y);
  call(op);
}

auto R6502::op_rmw_absolute(fp op) {
  abs.l = readPC();
  abs.h = readPC();
  rd = read(abs.w);
  write(abs.w, rd);
  call(op);
L write(abs.w, rd);
}

auto R6502::op_rmw_absolute_x(fp op) {
  abs.l = readPC();
  abs.h = readPC();
  ioPageAlways(abs.w, abs.w + regs.x);
  rd = read(abs.w + regs.x);
  write(abs.w + regs.x, rd);
  call(op);
L write(abs.w + regs.x, rd);
}

auto R6502::op_rmw_zero_page(fp op) {
  zp = readPC();
  rd = readZP(zp);
  writeZP(zp, rd);
  call(op);
L writeZP(zp, rd);
}

auto R6502::op_rmw_zero_page_x(fp op) {
  zp = readPC();
  readZP(zp);
  rd = readZP(zp + regs.x);
  writeZP(zp + regs.x, rd);
  call(op);
L writeZP(zp + regs.x, rd);
}

auto R6502::op_set_flag(uint bit) {
L io();
  regs.p |= 1 << bit;
}

auto R6502::op_shift(fp op) {
L io();
  call(op);
}

auto R6502::op_store_absolute(uint8& r) {
  abs.l = readPC();
  abs.h = readPC();
L write(abs.w, r);
}

auto R6502::op_store_absolute_x(uint8& r) {
  abs.l = readPC();
  abs.h = readPC();
  ioPageAlways(abs.w, abs.w + regs.x);
L write(abs.w + regs.x, r);
}

auto R6502::op_store_absolute_y(uint8& r) {
  abs.l = readPC();
  abs.h = readPC();
  ioPageAlways(abs.w, abs.w + regs.y);
L write(abs.w + regs.y, r);
}

auto R6502::op_store_indirect_zero_page_x(uint8& r) {
  zp = readPC();
  readZP(zp);
  abs.l = readZP(zp++ + regs.x);
  abs.h = readZP(zp++ + regs.x);
L write(abs.w, r);
}

auto R6502::op_store_indirect_zero_page_y(uint8& r) {
  rd = readPC();
  abs.l = readZP(rd++);
  abs.h = readZP(rd++);
  ioPageAlways(abs.w, abs.w + regs.y);
L write(abs.w + regs.y, r);
}

auto R6502::op_store_zero_page(uint8& r) {
  zp = readPC();
L writeZP(zp, r);
}

auto R6502::op_store_zero_page_x(uint8& r) {
  zp = readPC();
  readZP(zp);
L writeZP(zp + regs.x, r);
}

auto R6502::op_store_zero_page_y(uint8& r) {
  zp = readPC();
  readZP(zp);
L writeZP(zp + regs.y, r);
}

auto R6502::op_transfer(uint8& s, uint8& d, bool flag) {
L io();
  d = s;
  if(!flag) return;
  regs.p.n = (d & 0x80);
  regs.p.z = (d == 0);
}

//

auto R6502::op_brk() {
  readPC();
  writeSP(regs.pc >> 8);
  writeSP(regs.pc >> 0);
  writeSP(regs.p | 0x30);
  abs.l = read(0xfffe);
  regs.p.i = 1;
  regs.p.d = 0;
L abs.h = read(0xffff);
  regs.pc = abs.w;
}

auto R6502::op_jmp_absolute() {
  abs.l = readPC();
L abs.h = readPC();
  regs.pc = abs.w;
}

auto R6502::op_jmp_indirect_absolute() {
  abs.l = readPC();
  abs.h = readPC();
  iabs.l = read(abs.w); abs.l++;
L iabs.h = read(abs.w); abs.l++;
  regs.pc = iabs.w;
}

auto R6502::op_jsr_absolute() {
  abs.l = readPC();
  abs.h = readPC();
  io();
  regs.pc--;
  writeSP(regs.pc >> 8);
L writeSP(regs.pc >> 0);
  regs.pc = abs.w;
}

auto R6502::op_nop() {
L io();
}

auto R6502::op_php() {
  io();
L writeSP(regs.p | 0x30);
}

auto R6502::op_plp() {
  io();
  io();
L regs.p = readSP();
}

auto R6502::op_rti() {
  io();
  io();
  regs.p = readSP();
  abs.l = readSP();
L abs.h = readSP();
  regs.pc = abs.w;
}

auto R6502::op_rts() {
  io();
  io();
  abs.l = readSP();
  abs.h = readSP();
L io();
  regs.pc = ++abs.w;
}

//illegal opcodes
//===============

auto R6502::op_arr_immediate() {
L rd = readPC();
  regs.a &= rd;
  regs.a = (regs.p.c << 7) | (regs.a >> 1);
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
  regs.p.c = (regs.a & 0x40);
  regs.p.v = regs.p.c ^ ((regs.a >> 5) & 1);
}

auto R6502::op_nop_absolute() {
  abs.l = readPC();
  abs.h = readPC();
L io();
}

auto R6502::op_nop_absolute_x() {
  abs.l = readPC();
  abs.h = readPC();
  ioPage(abs.w, abs.w + regs.x);
L io();
}

auto R6502::op_nop_immediate() {
L io();
}

auto R6502::op_nop_implied() {
L io();
}

auto R6502::op_nop_zero_page() {
  zp = readPC();
L readZP(zp);
}

auto R6502::op_nop_zero_page_x() {
  zp = readPC();
  readZP(zp);
L readZP(zp + regs.x);
}
