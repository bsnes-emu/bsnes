/**********
*** asl ***
**********/

void g65816_op_aslb(void) {
  g65816_testc(gx816->regs.a.b & 0x80);
  gx816->regs.a.b <<= 1;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_aslw(void) {
  g65816_testc(gx816->regs.a.w & 0x8000);
  gx816->regs.a.w <<= 1;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_asl_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m <<= 1;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_asl_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m <<= 1;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_asl_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m <<= 1;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_asl_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m <<= 1;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_asl_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m <<= 1;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_asl_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m <<= 1;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_asl_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m <<= 1;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

void g65816_op_asl_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m <<= 1;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

/**********
*** lsr ***
**********/

void g65816_op_lsrb(void) {
  g65816_testc(gx816->regs.a.b & 0x01);
  gx816->regs.a.b >>= 1;
  g65816_clrn();
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_lsrw(void) {
  g65816_testc(gx816->regs.a.w & 0x0001);
  gx816->regs.a.w >>= 1;
  g65816_clrn();
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_lsr_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_lsr_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_lsr_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_lsr_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_lsr_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_lsr_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_lsr_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

void g65816_op_lsr_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m >>= 1;
  g65816_clrn();
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

/**********
*** rol ***
**********/

void g65816_op_rolb(void) {
byte c = gx816->regs.p & PF_C;
  g65816_testc(gx816->regs.a.b & 0x80);
  gx816->regs.a.b = (gx816->regs.a.b << 1) | c;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_rolw(void) {
byte c = gx816->regs.p & PF_C;
  g65816_testc(gx816->regs.a.w & 0x8000);
  gx816->regs.a.w = (gx816->regs.a.w << 1) | c;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_rol_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte c = gx816->regs.p & PF_C;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m = (m << 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_rol_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte c = gx816->regs.p & PF_C;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m = (m << 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_rol_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte c = gx816->regs.p & PF_C;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m = (m << 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_rol_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte c = gx816->regs.p & PF_C;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m = (m << 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_rol_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte c = gx816->regs.p & PF_C;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m = (m << 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_rol_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte c = gx816->regs.p & PF_C;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x8000);
  m = (m << 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_rol_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte c = gx816->regs.p & PF_C;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x80);
  m = (m << 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

void g65816_op_rol_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte c = gx816->regs.p & PF_C;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, arg);
  g65816_testc(m & 0x8000);
  m = (m << 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, arg, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

/**********
*** ror ***
**********/

void g65816_op_rorb(void) {
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
  g65816_testc(gx816->regs.a.b & 0x01);
  gx816->regs.a.b = (gx816->regs.a.b >> 1) | c;
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_rorw(void) {
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
  g65816_testc(gx816->regs.a.w & 0x0001);
  gx816->regs.a.w = (gx816->regs.a.w >> 1) | c;
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_ror_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m = (m >> 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_ror_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m = (m >> 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_ror_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m = (m >> 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_ror_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m = (m >> 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_ror_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m = (m >> 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_ror_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m = (m >> 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_REGD);
}

void g65816_op_ror_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
byte c = (gx816->regs.p & PF_C)?0x80:0x00;
byte m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);
  g65816_testc(m & 0x01);
  m = (m >> 1) | c;
  g65816_testn(m & 0x80);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}

void g65816_op_ror_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
word c = (gx816->regs.p & PF_C)?0x8000:0x0000;
word m = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
  g65816_testc(m & 0x0001);
  m = (m >> 1) | c;
  g65816_testn(m & 0x8000);
  g65816_testz(m == 0);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, m);
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(4, dest_addr);
  snes_time->add_cpu_icycles(2, TIMING_REGD);
}
