void spc700_op_clrc(void) {
  spc700_clrc();
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_setc(void) {
  spc700_setc();
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_notc(void) {
  spc700->regs.p ^= SPF_C;
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_clrv(void) {
  spc700_clrv();
  spc700_clrh();
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_clrp(void) {
  spc700_clrp();
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_setp(void) {
  spc700_setp();
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_di(void) {
  spc700_clri();
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_ei(void) {
  spc700_seti();
  spc700_incpc(1);
  add_apu_cycles(2);
}
