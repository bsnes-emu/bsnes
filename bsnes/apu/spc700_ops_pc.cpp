void spc700_op_bra(void) {
spc700_prefetchb();
  spc700->regs.pc += (signed char)arg;
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_bcc(void) {
spc700_prefetchb();
  if(!(spc700->regs.p & SPF_C)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_bcs(void) {
spc700_prefetchb();
  if((spc700->regs.p & SPF_C)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_beq(void) {
spc700_prefetchb();
  if((spc700->regs.p & SPF_Z)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_bmi(void) {
spc700_prefetchb();
  if((spc700->regs.p & SPF_N)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_bne(void) {
spc700_prefetchb();
  if(!(spc700->regs.p & SPF_Z)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_bpl(void) {
spc700_prefetchb();
  if(!(spc700->regs.p & SPF_N)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_bvc(void) {
spc700_prefetchb();
  if(!(spc700->regs.p & SPF_V)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

void spc700_op_bvs(void) {
spc700_prefetchb();
  if((spc700->regs.p & SPF_V)) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(4);
  } else {
    add_apu_cycles(2);
  }
  spc700_incpc(2);
}

#define spc700_op_bbc(__n)                                \
  void spc700_op_bbc##__n##(void) {                       \
  spc700_prefetch2b();                                    \
  byte r;                                                 \
    r = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0); \
    if(!(r & (1 << __n))) {                               \
      spc700->regs.pc += (signed char)arg1;               \
      add_apu_cycles(7);                                  \
    } else {                                              \
      add_apu_cycles(5);                                  \
    }                                                     \
    spc700_incpc(3);                                      \
  }

#define spc700_op_bbs(__n)                                \
  void spc700_op_bbs##__n##(void) {                       \
  spc700_prefetch2b();                                    \
  byte r;                                                 \
    r = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0); \
    if(r & (1 << __n)) {                                  \
      spc700->regs.pc += (signed char)arg1;               \
      add_apu_cycles(7);                                  \
    } else {                                              \
      add_apu_cycles(5);                                  \
    }                                                     \
    spc700_incpc(3);                                      \
  }

spc700_op_bbc(0)
spc700_op_bbc(1)
spc700_op_bbc(2)
spc700_op_bbc(3)
spc700_op_bbc(4)
spc700_op_bbc(5)
spc700_op_bbc(6)
spc700_op_bbc(7)

spc700_op_bbs(0)
spc700_op_bbs(1)
spc700_op_bbs(2)
spc700_op_bbs(3)
spc700_op_bbs(4)
spc700_op_bbs(5)
spc700_op_bbs(6)
spc700_op_bbs(7)

void spc700_op_cbne_dp(void) {
spc700_prefetch2b();
byte r;
  r = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0);
  if(r != spc700->regs.a) {
    spc700->regs.pc += (signed char)arg1;
    add_apu_cycles(7);
  } else {
    add_apu_cycles(5);
  }
  spc700_incpc(3);
}

void spc700_op_cbne_dpx(void) {
spc700_prefetch2b();
byte r;
  r = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg0);
  if(r != spc700->regs.a) {
    spc700->regs.pc += (signed char)arg1;
    add_apu_cycles(8);
  } else {
    add_apu_cycles(6);
  }
  spc700_incpc(3);
}

void spc700_op_dbnz_dp(void) {
spc700_prefetch2b();
byte r;
  r = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0) - 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg0, r);
  if(r != 0) {
    spc700->regs.pc += (signed char)arg1;
    add_apu_cycles(7);
  } else {
    add_apu_cycles(5);
  }
  spc700_incpc(3);
}

void spc700_op_dbnz_y(void) {
spc700_prefetchb();
  spc700->regs.y--;
  if(spc700->regs.y != 0) {
    spc700->regs.pc += (signed char)arg;
    add_apu_cycles(6);
  } else {
    add_apu_cycles(4);
  }
  spc700_incpc(2);
}

void spc700_op_jmp_addr(void) {
spc700_prefetchw();
  spc700->regs.pc = arg;
  add_apu_cycles(3);
}

void spc700_op_jmp_iaddrx(void) {
spc700_prefetchw();
  spc700->regs.pc = spc700->mem_read(APUMODE_IADDRX, MEMSIZE_WORD, arg);
  add_apu_cycles(3);
}

void spc700_op_call(void) {
spc700_prefetchw();
word r = spc700->regs.pc + 3;
  spc700_incpc(3);
  spc700->stack_write(r);
  spc700->stack_write(r >> 8);
  spc700->regs.pc = arg;
  add_apu_cycles(8);
}

void spc700_op_pcall(void) {
spc700_prefetchb();
word r = spc700->regs.pc + 2;
  spc700->stack_write(r);
  spc700->stack_write(r >> 8);
  spc700->regs.pc = 0xff00 | arg;
  add_apu_cycles(6);
}

void spc700_op_tcall_0(void) {
  dprintf("* spc700 opcode tcall 0 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_1(void) {
  dprintf("* spc700 opcode tcall 1 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_2(void) {
  dprintf("* spc700 opcode tcall 2 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_3(void) {
  dprintf("* spc700 opcode tcall 3 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_4(void) {
  dprintf("* spc700 opcode tcall 4 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_5(void) {
  dprintf("* spc700 opcode tcall 5 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_6(void) {
  dprintf("* spc700 opcode tcall 6 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_7(void) {
  dprintf("* spc700 opcode tcall 7 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_8(void) {
  dprintf("* spc700 opcode tcall 8 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_9(void) {
  dprintf("* spc700 opcode tcall 9 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_10(void) {
  dprintf("* spc700 opcode tcall 10 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_11(void) {
  dprintf("* spc700 opcode tcall 11 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_12(void) {
  dprintf("* spc700 opcode tcall 12 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_13(void) {
  dprintf("* spc700 opcode tcall 13 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_14(void) {
  dprintf("* spc700 opcode tcall 14 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_tcall_15(void) {
  dprintf("* spc700 opcode tcall 15 not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_brk(void) {
  dprintf("* spc700 opcode brk not implemented");
  spc700_incpc(1);
  add_apu_cycles(8);
}

void spc700_op_ret(void) {
word r;
  r  = spc700->stack_read();
  r |= spc700->stack_read() << 8;
  spc700->regs.pc = r;
  add_apu_cycles(5);
}

void spc700_op_reti(void) {
word r;
  spc700->regs.p = spc700->stack_read();
  r  = spc700->stack_read();
  r |= spc700->stack_read() << 8;
  spc700->regs.pc = r;
  add_apu_cycles(6);
}
