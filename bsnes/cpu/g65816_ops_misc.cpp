/***********
 *** bit ***
 **********/

void g65816_op_bit_constb(void) {
g65816_prefetch(1);
  g65816_testn(arg & 0x80);
  g65816_testv(arg & 0x40);
  g65816_testz((arg & gx816->regs.a.b) == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_bit_constw(void) {
g65816_prefetch(2);
  g65816_testn(arg & 0x8000);
  g65816_testv(arg & 0x4000);
  g65816_testz((arg & gx816->regs.a.w) == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}

void g65816_op_bit_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(arg & 0x80);
  g65816_testv(arg & 0x40);
  g65816_testz((arg & gx816->regs.a.b) == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_bit_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(arg & 0x8000);
  g65816_testv(arg & 0x4000);
  g65816_testz((arg & gx816->regs.a.w) == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_bit_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(arg & 0x80);
  g65816_testv(arg & 0x40);
  g65816_testz((arg & gx816->regs.a.b) == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_bit_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(arg & 0x8000);
  g65816_testv(arg & 0x4000);
  g65816_testz((arg & gx816->regs.a.w) == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_bit_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(arg & 0x80);
  g65816_testv(arg & 0x40);
  g65816_testz((arg & gx816->regs.a.b) == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_bit_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(arg & 0x8000);
  g65816_testv(arg & 0x4000);
  g65816_testz((arg & gx816->regs.a.w) == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_bit_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(arg & 0x80);
  g65816_testv(arg & 0x40);
  g65816_testz((arg & gx816->regs.a.b) == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_bit_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  arg = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(arg & 0x8000);
  g65816_testv(arg & 0x4000);
  g65816_testz((arg & gx816->regs.a.w) == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

/***********
 *** cpx ***
 **********/

void g65816_op_cpx_constb(void) {
g65816_prefetch(1);
int r = gx816->regs.x - arg;
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_cpx_constw(void) {
g65816_prefetch(2);
int r = gx816->regs.x - arg;
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}

void g65816_op_cpx_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
int r = gx816->regs.x - gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_cpx_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
int r = gx816->regs.x - gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_cpx_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
int r = gx816->regs.x - gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_cpx_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
int r = gx816->regs.x - gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

/***********
 *** cpy ***
 **********/

void g65816_op_cpy_constb(void) {
g65816_prefetch(1);
int r = gx816->regs.y - arg;
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_cpy_constw(void) {
g65816_prefetch(2);
int r = gx816->regs.y - arg;
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}

void g65816_op_cpy_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
int r = gx816->regs.y - gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_cpy_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
int r = gx816->regs.y - gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_cpy_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
int r = gx816->regs.y - gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(r & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_cpy_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
int r = gx816->regs.y - gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(r & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r >= 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

/***********
 *** ldx ***
 **********/

void g65816_op_ldx_constb(void) {
g65816_prefetch(1);
  gx816->regs.x = arg;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_ldx_constw(void) {
g65816_prefetch(2);
  gx816->regs.x = arg;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}

void g65816_op_ldx_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_ldx_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_ldx_addryb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ldx_addryw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ldx_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ldx_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ldx_dpyb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPY);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_ldx_dpyw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPY);
  gx816->regs.x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

/***********
 *** ldy ***
 **********/

void g65816_op_ldy_constb(void) {
g65816_prefetch(1);
  gx816->regs.y = (gx816->regs.y & 0xff00) | arg;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_ldy_constw(void) {
g65816_prefetch(2);
  gx816->regs.y = arg;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}

void g65816_op_ldy_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_ldy_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_ldy_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ldy_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_ldy_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ldy_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_ldy_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_ldy_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  gx816->regs.y = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

/***********
 *** stx ***
 **********/

void g65816_op_stx_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.x);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_stx_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.x);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_stx_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.x);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_stx_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.x);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_stx_dpyb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPY);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.x);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_stx_dpyw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPY);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.x);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

/***********
 *** sty ***
 **********/

void g65816_op_sty_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.y);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_sty_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.y);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_sty_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.y);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sty_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.y);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sty_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.y);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_sty_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.y);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

/***********
 *** stz ***
 **********/

void g65816_op_stz_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_stz_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_stz_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_stz_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_stz_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_stz_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_stz_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_stz_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

/***********
 *** xba ***
 **********/

void g65816_op_xba(void) {
  gx816->regs.a.p.l ^= gx816->regs.a.p.h;
  gx816->regs.a.p.h ^= gx816->regs.a.p.l;
  gx816->regs.a.p.l ^= gx816->regs.a.p.h;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(2);
}

/***********
 *** trb ***
 **********/

void g65816_op_trb_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testz((m & gx816->regs.a.b) == 0);
  m &= ~gx816->regs.a.b;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_trb_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testz((m & gx816->regs.a.w) == 0);
  m &= ~gx816->regs.a.w;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_trb_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testz((m & gx816->regs.a.b) == 0);
  m &= ~gx816->regs.a.b;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_trb_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testz((m & gx816->regs.a.w) == 0);
  m &= ~gx816->regs.a.w;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

/***********
 *** tsb ***
 **********/

void g65816_op_tsb_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testz((m & gx816->regs.a.b) == 0);
  m |= gx816->regs.a.b;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tsb_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testz((m & gx816->regs.a.w) == 0);
  m |= gx816->regs.a.w;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tsb_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testz((m & gx816->regs.a.b) == 0);
  m |= gx816->regs.a.b;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_tsb_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testz((m & gx816->regs.a.w) == 0);
  m |= gx816->regs.a.w;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

/***********
 *** mvn ***
 **********/

void g65816_op_mvn(void) {
g65816_prefetch(2);
ulong sp, dp;
byte m;
  if(gx816->regs.a.w != 0xffff) {
    gx816->regs.a.w--;
    dp = (arg) & 0xff;
    sp = (arg >> 8) & 0xff;
    dp = (dp << 16) | gx816->regs.y;
    sp = (sp << 16) | gx816->regs.x;
    m = gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, sp);
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, dp, m);
    gx816->regs.x++;
    gx816->regs.y++;
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & PF_X))) {
      gx816->regs.x &= 0xff;
      gx816->regs.y &= 0xff;
    }
  }
  if(gx816->regs.a.w == 0xffff) {
    g65816_incpc(3);
  }
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, sp);
  snes_time->add_cpu_mcycles(1, dp);
  snes_time->add_cpu_icycles(2);
}

/***********
 *** mvp ***
 **********/

void g65816_op_mvp(void) {
g65816_prefetch(2);
ulong sp, dp;
byte m;
  if(gx816->regs.a.w != 0xffff) {
    gx816->regs.a.w--;
    dp = (arg) & 0xff;
    sp = (arg >> 8) & 0xff;
    dp = (dp << 16) | gx816->regs.y;
    sp = (sp << 16) | gx816->regs.x;
    m = gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, sp);
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, dp, m);
    gx816->regs.x--;
    gx816->regs.y--;
    if(gx816->regs.e == true || (gx816->regs.e == false && (gx816->regs.p & PF_X))) {
      gx816->regs.x &= 0xff;
      gx816->regs.y &= 0xff;
    }
  }
  if(gx816->regs.a.w == 0xffff) {
    g65816_incpc(3);
  }
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, sp);
  snes_time->add_cpu_mcycles(1, dp);
  snes_time->add_cpu_icycles(2);
}

/***********
 *** brk ***
 **********/

void g65816_op_brke(void) {
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(3);
  snes_time->add_cpu_mcycles(2, 0x00ffe6);
  g65816_incpc(2);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.pc);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.p);
  g65816_seti();
  g65816_clrd();
  gx816->regs.pc = gx816->mem_read(MEMMODE_LONG, MEMSIZE_WORD, 0x00ffe6);
}

void g65816_op_brkn(void) {
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(4);
  snes_time->add_cpu_mcycles(2, 0x00ffe6);
  g65816_incpc(2);
  g65816_stackwrite(MEMSIZE_LONG, gx816->regs.pc);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.p);
  g65816_seti();
  g65816_clrd();
  gx816->regs.pc = gx816->mem_read(MEMMODE_LONG, MEMSIZE_WORD, 0x00ffe6);
}

/***********
 *** cop ***
 **********/

void g65816_op_cope(void) {
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(3);
  snes_time->add_cpu_mcycles(2, 0x00fff4);
  g65816_incpc(2);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.pc);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.p);
  g65816_seti();
  g65816_clrd();
  gx816->regs.pc = gx816->mem_read(MEMMODE_LONG, MEMSIZE_WORD, 0x00fff4);
}

void g65816_op_copn(void) {
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(4);
  snes_time->add_cpu_mcycles(2, 0x00fff4);
  g65816_incpc(2);
  g65816_stackwrite(MEMSIZE_LONG, gx816->regs.pc);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.p);
  g65816_seti();
  g65816_clrd();
  gx816->regs.pc = gx816->mem_read(MEMMODE_LONG, MEMSIZE_WORD, 0x00fff4);
}

/***********
 *** stp ***
 **********/

void g65816_op_stp(void) {
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(2);
  dprintf("* stp not implemented");
}

/***********
 *** wai ***
 **********/

void g65816_op_wai(void) {
byte i = 0;
  if(gx816->nmi_enabled == true)i = 1;
  if((ppu.vcounter_enabled == true || ppu.hcounter_enabled == true) && !(gx816->regs.p & PF_I))i = 1;
  if(gx816->wai_interrupt_occurred == true || i == 0) {
    gx816->wai_interrupt_occurred = false;
    g65816_incpc(1);
  }
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(2);
}

/***********
 *** xce ***
 **********/

void g65816_op_xce(void) {
bool t = gx816->regs.e;
  if(gx816->regs.p & PF_C)gx816->regs.e = true;
  else                    gx816->regs.e = false;
  g65816_setm();
  g65816_setx();
  if(t == true)g65816_setc();
  else         g65816_clrc();

  if(gx816->regs.e == true)gx816->regs.s = 0x0100 | (gx816->regs.s & 0xff);

  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

/***********
 *** nop ***
 **********/

void g65816_op_nop(void) {
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

/***********
 *** wdm ***
 **********/

void g65816_op_wdm(void) {
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

/*****************************
 *** clear/set bit opcodes ***
 ****************************/

void g65816_op_clc(void) {
  g65816_clrc();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_cld(void) {
  g65816_clrd();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_cli(void) {
  g65816_clri();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_clv(void) {
  g65816_clrv();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_sec(void) {
  g65816_setc();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_sed(void) {
  g65816_setd();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_sei(void) {
  g65816_seti();
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_rep(void) {
g65816_prefetch(1);
  gx816->regs.p &= ~arg;
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(3);

  if(gx816->regs.e == true)gx816->regs.p |= 0x30;
}

void g65816_op_sep(void) {
g65816_prefetch(1);
  gx816->regs.p |= arg;
  if(arg & 0x10) {
    gx816->regs.x &= 0xff;
    gx816->regs.y &= 0xff;
  }
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(3);
}

/************************
 *** transfer opcodes ***
 ***********************/

void g65816_op_taxb(void) {
  gx816->regs.x = gx816->regs.a.b;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_taxw(void) {
  gx816->regs.x = gx816->regs.a.w;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tayb(void) {
  gx816->regs.y = gx816->regs.a.b;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tayw(void) {
  gx816->regs.y = gx816->regs.a.w;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tcd(void) {
  gx816->regs.d = gx816->regs.a.w;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tcse(void) {
  gx816->regs.s = 0x0100 | gx816->regs.a.b;
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tcsn(void) {
  gx816->regs.s = gx816->regs.a.w;
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_tdc(void) {
  gx816->regs.a.w = gx816->regs.d;
  g65816_testn(gx816->regs.d & 0x8000);
  g65816_testz(gx816->regs.d == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tsce(void) {
  gx816->regs.a.w = gx816->regs.s;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tscn(void) {
  gx816->regs.a.w = gx816->regs.s;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tsxb(void) {
  gx816->regs.x = gx816->regs.s & 0xff;
  g65816_testn(gx816->regs.s & 0x80);
  g65816_testz((gx816->regs.s & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tsxw(void) {
  gx816->regs.x = gx816->regs.s;
  g65816_testn(gx816->regs.s & 0x8000);
  g65816_testz(gx816->regs.s == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_txab(void) {
  gx816->regs.a.b = gx816->regs.x;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_txaw(void) {
  gx816->regs.a.w = gx816->regs.x;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_txsb(void) {
  gx816->regs.s = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_txsw(void) {
  gx816->regs.s = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_txyb(void) {
  gx816->regs.y = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_txyw(void) {
  gx816->regs.y = gx816->regs.x;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tyab(void) {
  gx816->regs.a.b = gx816->regs.y;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tyaw(void) {
  gx816->regs.a.w = gx816->regs.y;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tyxb(void) {
  gx816->regs.x = gx816->regs.y;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz((gx816->regs.y & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_tyxw(void) {
  gx816->regs.x = gx816->regs.y;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}
