/***********
 *** inc ***
 **********/

void g65816_op_incb(void) {
  gx816->regs.a.b++;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_incw(void) {
  gx816->regs.a.w++;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_inc_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_inc_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_inc_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_inc_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_inc_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_inc_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_inc_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

void g65816_op_inc_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m++;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

/***********
 *** inx ***
 **********/

void g65816_op_inxb(void) {
  gx816->regs.x = (gx816->regs.x + 1) & 0xff;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_inxw(void) {
  gx816->regs.x += 1;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

/***********
 *** iny ***
 **********/

void g65816_op_inyb(void) {
  gx816->regs.y = (gx816->regs.y + 1) & 0xff;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz((gx816->regs.y & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_inyw(void) {
  gx816->regs.y += 1;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

/***********
 *** dec ***
 **********/

void g65816_op_decb(void) {
  gx816->regs.a.b--;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_decw(void) {
  gx816->regs.a.w--;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_dec_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_dec_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_dec_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_dec_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_dec_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_dec_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_dec_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

void g65816_op_dec_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  m--;
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

/***********
 *** dex ***
 **********/

void g65816_op_dexb(void) {
  gx816->regs.x = (gx816->regs.x - 1) & 0xff;
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_dexw(void) {
  gx816->regs.x -= 1;
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

/***********
 *** dey ***
 **********/

void g65816_op_deyb(void) {
  gx816->regs.y = (gx816->regs.y - 1) & 0xff;
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz((gx816->regs.y & 0xff) == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_deyw(void) {
  gx816->regs.y -= 1;
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}
