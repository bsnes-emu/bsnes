#define bcd_add_adjust_byte()             \
  if(gx816->regs.p & PF_D) {              \
    if(((r     ) & 15) > 9)r += 6;        \
    if(((r >> 4) & 15) > 9)r += 6 << 4;   \
  }

#define bcd_add_adjust_word()             \
  if(gx816->regs.p & PF_D) {              \
    if(((r      ) & 15) > 9)r += 6;       \
    if(((r >>  4) & 15) > 9)r += 6 <<  4; \
    if(((r >>  8) & 15) > 9)r += 6 <<  8; \
    if(((r >> 12) & 15) > 9)r += 6 << 12; \
  }

#define g65816_if_adc_b()                                                    \
byte c = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr);             \
int  r = gx816->regs.a.b + c + (gx816->regs.p & PF_C);                       \
  bcd_add_adjust_byte()                                                      \
  g65816_testn(r & 0x80);                                                    \
  g65816_testv(~(gx816->regs.a.b ^ c) & (gx816->regs.a.b ^ (byte)r) & 0x80); \
  g65816_testz((byte)r == 0);                                                \
  g65816_testc(r > 0xff);                                                    \
  gx816->regs.a.b = r

#define g65816_if_adc_w()                                                      \
word c = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);               \
int  r = gx816->regs.a.w + c + (gx816->regs.p & PF_C);                         \
  bcd_add_adjust_word()                                                        \
  g65816_testn(r & 0x8000);                                                    \
  g65816_testv(~(gx816->regs.a.w ^ c) & (gx816->regs.a.w ^ (word)r) & 0x8000); \
  g65816_testz((word)r == 0);                                                  \
  g65816_testc(r > 0xffff);                                                    \
  gx816->regs.a.w = r

void g65816_op_adc_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  g65816_if_adc_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_adc_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  g65816_if_adc_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_adc_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  g65816_if_adc_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_adc_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  g65816_if_adc_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_adc_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_idpb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDP);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_idpw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDP);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_ildpb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDP);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_ildpw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDP);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_longb(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONG);
  g65816_if_adc_b();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_adc_longw(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONG);
  g65816_if_adc_w();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_adc_longxb(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONGX);
  g65816_if_adc_b();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_adc_longxw(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONGX);
  g65816_if_adc_w();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_adc_addryb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  g65816_if_adc_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_adc_addryw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  g65816_if_adc_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_adc_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_adc_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_adc_idpxb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPX);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_adc_idpxw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPX);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_adc_idpyb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPY);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2 | TIMING_CONDITION4);
}

void g65816_op_adc_idpyw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPY);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2 | TIMING_CONDITION4);
}

void g65816_op_adc_ildpyb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDPY);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_ildpyw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDPY);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_adc_srb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_SR);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_adc_srw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_SR);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_adc_isryb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_ISRY);
  g65816_if_adc_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_adc_isryw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_ISRY);
  g65816_if_adc_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_adc_constb(void) {
g65816_prefetch(1);
int r = gx816->regs.a.b + arg + (gx816->regs.p & PF_C);
  bcd_add_adjust_byte()
  g65816_testn(r & 0x80);
  g65816_testv(~(gx816->regs.a.b ^ arg) & (gx816->regs.a.b ^ (byte)r) & 0x80);
  g65816_testz((byte)r == 0);
  g65816_testc(r > 0xff);
  gx816->regs.a.b = r;
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
}

void g65816_op_adc_constw(void) {
g65816_prefetch(2);
int r = gx816->regs.a.w + arg + (gx816->regs.p & PF_C);
  bcd_add_adjust_word()
  g65816_testn(r & 0x8000);
  g65816_testv(~(gx816->regs.a.w ^ arg) & (gx816->regs.a.w ^ (word)r) & 0x8000);
  g65816_testz((word)r == 0);
  g65816_testc(r > 0xffff);
  gx816->regs.a.w = r;
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
}
