#define g65816_sta_data_b() gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dest_addr, gx816->regs.a.b)
#define g65816_sta_data_w() gx816->mem_write(MEMMODE_NONE, MEMSIZE_WORD, dest_addr, gx816->regs.a.w)

void g65816_op_sta_addrb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  g65816_sta_data_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_sta_addrw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDR);
  g65816_sta_data_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_sta_addrxb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  g65816_sta_data_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_sta_addrxw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRX);
  g65816_sta_data_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_sta_dpb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_dpw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DP);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_idpb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDP);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_idpw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDP);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_ildpb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDP);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_ildpw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDP);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_longb(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONG);
  g65816_sta_data_b();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_sta_longw(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONG);
  g65816_sta_data_w();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_sta_longxb(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONGX);
  g65816_sta_data_b();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(1, dest_addr);
}

void g65816_op_sta_longxw(void) {
g65816_prefetch(3);
g65816_getaddr(MEMMODE_LONGX);
  g65816_sta_data_w();
  g65816_incpc(4);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_mcycles(2, dest_addr);
}

void g65816_op_sta_addryb(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  g65816_sta_data_b();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_sta_addryw(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ADDRY);
  g65816_sta_data_w();
  g65816_incpc(3);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION4);
}

void g65816_op_sta_dpxb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_sta_dpxw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_DPX);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_sta_idpxb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPX);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_sta_idpxw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPX);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1, TIMING_CONDITION2);
}

void g65816_op_sta_idpyb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPY);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2 | TIMING_CONDITION4);
}

void g65816_op_sta_idpyw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_IDPY);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(2, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2 | TIMING_CONDITION4);
}

void g65816_op_sta_ildpyb(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDPY);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_ildpyw(void) {
g65816_prefetch(1);
g65816_getiaddr(MEMMODE_ILDPY);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_mcycles(3, base_addr);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(0, TIMING_CONDITION2);
}

void g65816_op_sta_srb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_SR);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(1);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_sta_srw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_SR);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
}

void g65816_op_sta_isryb(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_ISRY);
  g65816_sta_data_b();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_mcycles(1, dest_addr);
  snes_time->add_cpu_icycles(2);
}

void g65816_op_sta_isryw(void) {
g65816_prefetch(1);
g65816_getaddr(MEMMODE_ISRY);
  g65816_sta_data_w();
  g65816_incpc(2);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(2);
}
