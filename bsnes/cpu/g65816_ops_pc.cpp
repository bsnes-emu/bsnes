/***********
 *** jmp ***
 **********/

void g65816_op_jmp_addr(void) {
g65816_prefetch(2);
  snes_time->add_cpu_pcycles(3);
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | arg;
}

void g65816_op_jmp_long(void) {
g65816_prefetch(3);
  snes_time->add_cpu_pcycles(4);
  gx816->regs.pc = arg;
}

void g65816_op_jmp_iaddr(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_IADDR_PC);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
}

void g65816_op_jmp_iaddrx(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_IADDRX_PC);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(2, dest_addr);
  snes_time->add_cpu_icycles(1);
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dest_addr);
}

void g65816_op_jmp_iladdr(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_ILADDR_PC);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_mcycles(3, dest_addr);
  gx816->regs.pc = gx816->mem_read(MEMMODE_NONE, MEMSIZE_LONG, dest_addr);
}

/***********
 *** jsr ***
 **********/

//Note: The address pushed onto the stack is one byte less than the operand size.
//Upon returning, the address counter is incremented one more to move to the next
//instruction. The third/fourth cycles store the pc address on the stack, before
//the second operand byte is read in.

void g65816_op_jsr_addr(void) {
g65816_prefetch(2);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.pc + 2);
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | arg;
}

void g65816_op_jsr_long(void) {
g65816_prefetch(3);
  snes_time->add_cpu_pcycles(4);
  snes_time->add_cpu_scycles(3);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_LONG, gx816->regs.pc + 3);
  gx816->regs.pc = arg;
}

void g65816_op_jsr_iaddrx(void) {
g65816_prefetch(2);
g65816_getaddr(MEMMODE_IADDRX_PC);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(1);
  g65816_stackwrite(MEMSIZE_WORD, gx816->regs.pc + 2);
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | gx816->mem_read(MEMMODE_IADDRX_PC, MEMSIZE_WORD, arg);
  snes_time->add_cpu_pcycles(2);
}

/***********
 *** ret ***
 **********/

void g65816_op_rtie(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(3);
  gx816->regs.p  = g65816_stackread(MEMSIZE_BYTE);
  gx816->regs.pc = (gx816->regs.pc & 0xff0000) | g65816_stackread(MEMSIZE_WORD);
  if(gx816->regs.p & PF_X) { gx816->regs.x &= 0xff; gx816->regs.y &= 0xff; }
  gx816->regs.p &= ~ PF_I;
  snes_time->add_cpu_icycles(2);
}

void g65816_op_rtin(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(4);
  gx816->regs.p  = g65816_stackread(MEMSIZE_BYTE);
  gx816->regs.pc = g65816_stackread(MEMSIZE_LONG);
  if(gx816->regs.p & PF_X) { gx816->regs.x &= 0xff; gx816->regs.y &= 0xff; }
  snes_time->add_cpu_icycles(2);
}

void g65816_op_rts(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(2);
  snes_time->add_cpu_icycles(3);
  gx816->regs.pc = ((gx816->regs.pc & 0xff0000) | g65816_stackread(MEMSIZE_WORD)) + 1;
}

void g65816_op_rtl(void) {
  snes_time->add_cpu_pcycles(1);
  snes_time->add_cpu_scycles(3);
  snes_time->add_cpu_icycles(2);
  gx816->regs.pc = g65816_stackread(MEMSIZE_LONG) + 1;
}

/***********
 *** bra ***
 **********/

//Need to add condition (5) to cycle counts: if e=1, add one (i)cycle if page boundary crossed

void g65816_op_bra(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  snes_time->add_cpu_icycles(1);
  gx816->regs.pc += (signed char)(arg + 2);
}

void g65816_op_brl(void) {
g65816_prefetch(2);
  snes_time->add_cpu_pcycles(3);
  snes_time->add_cpu_icycles(1);
  gx816->regs.pc += (signed short)(arg + 3);
}

void g65816_op_bcc(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(!(gx816->regs.p & PF_C)) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_bcs(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(gx816->regs.p & PF_C) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_bne(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(!(gx816->regs.p & PF_Z)) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_beq(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(gx816->regs.p & PF_Z) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_bpl(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(!(gx816->regs.p & PF_N)) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_bmi(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(gx816->regs.p & PF_N) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_bvc(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(!(gx816->regs.p & PF_V)) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}

void g65816_op_bvs(void) {
g65816_prefetch(1);
  snes_time->add_cpu_pcycles(2);
  if(gx816->regs.p & PF_V) {
    snes_time->add_cpu_icycles(1);
    gx816->regs.pc += (signed char)(arg + 2);
  } else {
    g65816_incpc(2);
  }
}
