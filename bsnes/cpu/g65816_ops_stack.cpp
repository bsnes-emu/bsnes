ulong g65816_stackread(byte size) {
ulong r = 0;
  if(size == MEMSIZE_BYTE) {
    gx816->regs.s++;
    r  = gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s);
  } else if(size == MEMSIZE_WORD) {
    gx816->regs.s++;
    r  = gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s);
    gx816->regs.s++;
    r |= gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s) << 8;
  } else if(size == MEMSIZE_LONG) {
    gx816->regs.s++;
    r  = gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s);
    gx816->regs.s++;
    r |= gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s) << 8;
    gx816->regs.s++;
    r |= gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s) << 16;
  }
  if(gx816->regs.e == true) {
    gx816->regs.s = 0x0100 | (gx816->regs.s & 0xff);
  }
  return r;
}

void g65816_stackwrite(byte size, ulong value) {
  if(size == MEMSIZE_BYTE) {
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s, value);
    gx816->regs.s--;
  } else if(size == MEMSIZE_WORD) {
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s, value >> 8);
    gx816->regs.s--;
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s, value);
    gx816->regs.s--;
  } else if(size == MEMSIZE_LONG) {
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s, value >> 16);
    gx816->regs.s--;
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s, value >> 8);
    gx816->regs.s--;
    gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, gx816->regs.s, value);
    gx816->regs.s--;
  }
  if(gx816->regs.e == true) {
    gx816->regs.s = 0x0100 | (gx816->regs.s & 0xff);
  }
}

void g65816_op_phab(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.a.b);
  g65816_incpc(1);
}

void g65816_op_phaw(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.a.w);
  g65816_incpc(1);
}

void g65816_op_phb(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.db);
  g65816_incpc(1);
}

void g65816_op_phd(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.d);
  g65816_incpc(1);
}

void g65816_op_phk(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.pc >> 16);
  g65816_incpc(1);
}

void g65816_op_php(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.p);
  g65816_incpc(1);
}

void g65816_op_phxb(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.x);
  g65816_incpc(1);
}

void g65816_op_phxw(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.x);
  g65816_incpc(1);
}

void g65816_op_phyb(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_BYTE, gx816->regs.y);
  g65816_incpc(1);
}

void g65816_op_phyw(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.y);
  g65816_incpc(1);
}

void g65816_op_plab(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(2);
  gx816->regs.a.b = g65816_stackread(MEMSIZE_BYTE);
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
  g65816_incpc(1);
}

void g65816_op_plaw(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(2);
  gx816->regs.a.w = g65816_stackread(MEMSIZE_WORD);
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
  g65816_incpc(1);
}

void g65816_op_plb(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(2);
  gx816->regs.db = g65816_stackread(MEMSIZE_BYTE);
  g65816_testn(gx816->regs.db & 0x80);
  g65816_testz(gx816->regs.db == 0);
  g65816_incpc(1);
}

void g65816_op_pld(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(2);
  gx816->regs.d = g65816_stackread(MEMSIZE_WORD);
  g65816_testn(gx816->regs.d & 0x8000);
  g65816_testz(gx816->regs.d == 0);
  g65816_incpc(1);
}

void g65816_op_plp(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(2);
  gx816->regs.p = g65816_stackread(MEMSIZE_BYTE);
  g65816_incpc(1);
  if(gx816->regs.e == true)gx816->regs.p |= 0x30;
  if(gx816->regs.p & PF_X) { gx816->regs.x &= 0xff; gx816->regs.y &= 0xff; }
}

void g65816_op_plxb(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(2);
  gx816->regs.x = g65816_stackread(MEMSIZE_BYTE);
  g65816_testn(gx816->regs.x & 0x80);
  g65816_testz((gx816->regs.x & 0xff) == 0);
  g65816_incpc(1);
}

void g65816_op_plxw(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(2);
  gx816->regs.x = g65816_stackread(MEMSIZE_WORD);
  g65816_testn(gx816->regs.x & 0x8000);
  g65816_testz(gx816->regs.x == 0);
  g65816_incpc(1);
}

void g65816_op_plyb(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(2);
  gx816->regs.y = g65816_stackread(MEMSIZE_BYTE);
  g65816_testn(gx816->regs.y & 0x80);
  g65816_testz((gx816->regs.y & 0xff) == 0);
  g65816_incpc(1);
}

void g65816_op_plyw(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(2);
  gx816->regs.y = g65816_stackread(MEMSIZE_WORD);
  g65816_testn(gx816->regs.y & 0x8000);
  g65816_testz(gx816->regs.y == 0);
  g65816_incpc(1);
}

void g65816_op_pea(void) {
g65816_prefetch(2);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_scycles(2);
  g65816_stackwrite(MEMSIZE_WORD, arg);
  g65816_incpc(3);
}

void g65816_op_pei(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(0, TIMING_REGD);
  g65816_stackwrite(MEMSIZE_WORD, gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr));
  g65816_incpc(2);
}

void g65816_op_per(void) {
g65816_prefetch(2);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.pc + arg + 3);
  g65816_incpc(3);
}
