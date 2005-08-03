void bCPU::op_bra() {
l1:
  rd.l = op_read();
  if(1) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bcc() {
l1:
  rd.l = op_read();
  if(!regs.p.c) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bcs() {
l1:
  rd.l = op_read();
  if(regs.p.c) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bne() {
l1:
  rd.l = op_read();
  if(!regs.p.z) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_beq() {
l1:
  rd.l = op_read();
  if(regs.p.z) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bpl() {
l1:
  rd.l = op_read();
  if(!regs.p.n) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bmi() {
l1:
  rd.l = op_read();
  if(regs.p.n) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bvc() {
l1:
  rd.l = op_read();
  if(!regs.p.v) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_bvs() {
l1:
  rd.l = op_read();
  if(regs.p.v) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
l2:
  cpu_c6(aa.w);
l3:
  cpu_io();
}

void bCPU::op_brl() {
l1:
  rd.l = op_read();
l2:
  rd.h = op_read();
l3:
  cpu_io();
  regs.pc.w = regs.pc.d + (int16)rd.w;
}

void bCPU::op_jmp_addr() {
l1:
  rd.l = op_read();
l2:
  rd.h = op_read();
  regs.pc.w = rd.w;
}

void bCPU::op_jmp_long() {
l1:
  rd.l = op_read();
l2:
  rd.h = op_read();
l3:
  rd.b = op_read();
  regs.pc.d = rd.d & 0xffffff;
}

void bCPU::op_jmp_iaddr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_ADDR, aa.w);
l4:
  rd.h = op_read(OPMODE_ADDR, aa.w + 1);
  regs.pc.w = rd.w;
}

void bCPU::op_jmp_iaddrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);
l5:
  rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

void bCPU::op_jmp_iladdr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_ADDR, aa.w);
l4:
  rd.h = op_read(OPMODE_ADDR, aa.w + 1);
l5:
  rd.b = op_read(OPMODE_ADDR, aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
}

void bCPU::op_jsr_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  regs.pc.w--;
  stack_write(regs.pc.h);
l5:
  stack_write(regs.pc.l);
  regs.pc.w = aa.w;
}

void bCPU::op_jsr_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  stack_write(regs.pc.b);
l4:
  cpu_io();
l5:
  aa.b = op_read();
l6:
  regs.pc.w--;
  stack_write(regs.pc.h);
l7:
  stack_write(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
}

void bCPU::op_jsr_iaddrx() {
l1:
  aa.l = op_read();
l2:
  stack_write(regs.pc.h);
l3:
  stack_write(regs.pc.l);
l4:
  aa.h = op_read();
l5:
  cpu_io();
l6:
  rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);
l7:
  rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

void bCPU::op_rti() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.p = stack_read();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
l4:
  rd.l = stack_read();
l5:
  rd.h = stack_read();
  if(regs.e) {
    regs.pc.w = rd.w;
    return;
  }
l6:
  rd.b = stack_read();
  regs.pc.d = rd.d & 0xffffff;
}

void bCPU::op_rts() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  rd.l = stack_read();
l4:
  rd.h = stack_read();
l5:
  cpu_io();
  regs.pc.w = rd.w;
  regs.pc.w++;
}

void bCPU::op_rtl() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  rd.l = stack_read();
l4:
  rd.h = stack_read();
l5:
  rd.b = stack_read();
  regs.pc.d = rd.d & 0xffffff;
  regs.pc.w++;
}

