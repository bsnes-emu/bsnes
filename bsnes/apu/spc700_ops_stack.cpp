void spc700_op_push_a(void) {
  spc700->stack_write(spc700->regs.a);
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_push_x(void) {
  spc700->stack_write(spc700->regs.x);
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_push_y(void) {
  spc700->stack_write(spc700->regs.y);
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_push_p(void) {
  spc700->stack_write(spc700->regs.p);
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_pop_a(void) {
  spc700->regs.a = spc700->stack_read();
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_pop_x(void) {
  spc700->regs.x = spc700->stack_read();
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_pop_y(void) {
  spc700->regs.y = spc700->stack_read();
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_pop_p(void) {
  spc700->regs.p = spc700->stack_read();
  spc700_incpc(1);
  add_apu_cycles(4);
  spc700->regs.dp = (spc700->regs.p & SPF_P)?0x0100:0x0000;
}
