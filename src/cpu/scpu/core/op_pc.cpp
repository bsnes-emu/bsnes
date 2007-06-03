void sCPU::op_bcc() {
  if(!!regs.p.c)last_cycle();
  rd.l = op_readpc();
  if(!regs.p.c) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bcs() {
  if(!regs.p.c)last_cycle();
  rd.l = op_readpc();
  if(regs.p.c) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bne() {
  if(!!regs.p.z)last_cycle();
  rd.l = op_readpc();
  if(!regs.p.z) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_beq() {
  if(!regs.p.z)last_cycle();
  rd.l = op_readpc();
  if(regs.p.z) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bpl() {
  if(!!regs.p.n)last_cycle();
  rd.l = op_readpc();
  if(!regs.p.n) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bmi() {
  if(!regs.p.n)last_cycle();
  rd.l = op_readpc();
  if(regs.p.n) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bvc() {
  if(!!regs.p.v)last_cycle();
  rd.l = op_readpc();
  if(!regs.p.v) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bvs() {
  if(!regs.p.v)last_cycle();
  rd.l = op_readpc();
  if(regs.p.v) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    return;
  }
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_bra() {
  rd.l = op_readpc();
  aa.w = regs.pc.d + (int8)rd.l;
  regs.pc.w = aa.w;
  op_io_cond6(aa.w);
  last_cycle();
  op_io();
}

void sCPU::op_brl() {
  rd.l = op_readpc();
  rd.h = op_readpc();
  last_cycle();
  op_io();
  regs.pc.w = regs.pc.d + (int16)rd.w;
}

void sCPU::op_jmp_addr() {
  rd.l = op_readpc();
  last_cycle();
  rd.h = op_readpc();
  regs.pc.w = rd.w;
}

void sCPU::op_jmp_long() {
  rd.l = op_readpc();
  rd.h = op_readpc();
  last_cycle();
  rd.b = op_readpc();
  regs.pc.d = rd.d & 0xffffff;
}

void sCPU::op_jmp_iaddr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readaddr(aa.w);
  last_cycle();
  rd.h = op_readaddr(aa.w + 1);
  regs.pc.w = rd.w;
}

void sCPU::op_jmp_iaddrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readpbr(aa.w + regs.x.w);
  last_cycle();
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

void sCPU::op_jmp_iladdr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readaddr(aa.w);
  rd.h = op_readaddr(aa.w + 1);
  last_cycle();
  rd.b = op_readaddr(aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
}

void sCPU::op_jsr_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  regs.pc.w--;
  op_writestack(regs.pc.h);
  last_cycle();
  op_writestack(regs.pc.l);
  regs.pc.w = aa.w;
}

void sCPU::op_jsr_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(regs.pc.b);
  op_io();
  aa.b = op_readpc();
  regs.pc.w--;
  op_writestackn(regs.pc.h);
  last_cycle();
  op_writestackn(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_jsr_iaddrx() {
  aa.l = op_readpc();
  op_writestackn(regs.pc.h);
  op_writestackn(regs.pc.l);
  aa.h = op_readpc();
  op_io();
  rd.l = op_readpbr(aa.w + regs.x.w);
  last_cycle();
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_rti() {
  op_io();
  op_io();
  regs.p = op_readstack();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
  rd.l = op_readstack();
  if(regs.e)last_cycle();
  rd.h = op_readstack();
  if(regs.e) {
    regs.pc.w = rd.w;
    return;
  }
  last_cycle();
  rd.b = op_readstack();
  regs.pc.d = rd.d & 0xffffff;
}

void sCPU::op_rts() {
  op_io();
  op_io();
  rd.l = op_readstack();
  rd.h = op_readstack();
  last_cycle();
  op_io();
  regs.pc.w = rd.w;
  regs.pc.w++;
}

void sCPU::op_rtl() {
  op_io();
  op_io();
  rd.l = op_readstackn();
  rd.h = op_readstackn();
  last_cycle();
  rd.b = op_readstackn();
  regs.pc.d = rd.d & 0xffffff;
  regs.pc.w++;
  if(regs.e)regs.s.h = 0x01;
}

