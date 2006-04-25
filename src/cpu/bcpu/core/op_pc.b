bcc(0x90, !regs.p.c),
bcs(0xb0, regs.p.c),
bne(0xd0, !regs.p.z),
beq(0xf0, regs.p.z),
bpl(0x10, !regs.p.n),
bmi(0x30, regs.p.n),
bvc(0x50, !regs.p.v),
bvs(0x70, regs.p.v) {
1:if(!$1)last_cycle();
  rd.l = op_read();
  if($1) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    end;
  }
2:cpu_c6(aa.w);
3:last_cycle();
  cpu_io();
}

bra(0x80) {
1:rd.l = op_read();
  aa.w = regs.pc.d + (int8)rd.l;
  regs.pc.w = aa.w;
2:cpu_c6(aa.w);
3:last_cycle();
  cpu_io();
}

brl(0x82) {
1:rd.l = op_read();
2:rd.h = op_read();
3:last_cycle();
  cpu_io();
  regs.pc.w = regs.pc.d + (int16)rd.w;
}

jmp_addr(0x4c) {
1:rd.l = op_read();
2:last_cycle();
  rd.h = op_read();
  regs.pc.w = rd.w;
}

jmp_long(0x5c) {
1:rd.l = op_read();
2:rd.h = op_read();
3:last_cycle();
  rd.b = op_read();
  regs.pc.d = rd.d & 0xffffff;
}

jmp_iaddr(0x6c) {
1:aa.l = op_read();
2:aa.h = op_read();
3:rd.l = op_read(OPMODE_ADDR, aa.w);
4:last_cycle();
  rd.h = op_read(OPMODE_ADDR, aa.w + 1);
  regs.pc.w = rd.w;
}

jmp_iaddrx(0x7c) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_io();
4:rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);
5:last_cycle();
  rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

jmp_iladdr(0xdc) {
1:aa.l = op_read();
2:aa.h = op_read();
3:rd.l = op_read(OPMODE_ADDR, aa.w);
4:rd.h = op_read(OPMODE_ADDR, aa.w + 1);
5:last_cycle();
  rd.b = op_read(OPMODE_ADDR, aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
}

jsr_addr(0x20) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_io();
4:regs.pc.w--;
  stack_write(regs.pc.h);
5:last_cycle();
  stack_write(regs.pc.l);
  regs.pc.w = aa.w;
}

jsr_long(0x22) {
1:aa.l = op_read();
2:aa.h = op_read();
3:stack_write(regs.pc.b);
4:cpu_io();
5:aa.b = op_read();
6:regs.pc.w--;
  stack_write(regs.pc.h);
7:last_cycle();
  stack_write(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
}

jsr_iaddrx(0xfc) {
1:aa.l = op_read();
2:stack_write(regs.pc.h);
3:stack_write(regs.pc.l);
4:aa.h = op_read();
5:cpu_io();
6:rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);
7:last_cycle();
  rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

rti(0x40) {
1:cpu_io();
2:cpu_io();
3:regs.p = stack_read();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
4:rd.l = stack_read();
5:if(regs.e)last_cycle();
  rd.h = stack_read();
  if(regs.e) {
    regs.pc.w = rd.w;
    end;
  }
6:last_cycle();
  rd.b = stack_read();
  regs.pc.d = rd.d & 0xffffff;
}

rts(0x60) {
1:cpu_io();
2:cpu_io();
3:rd.l = stack_read();
4:rd.h = stack_read();
5:last_cycle();
  cpu_io();
  regs.pc.w = rd.w;
  regs.pc.w++;
}

rtl(0x6b) {
1:cpu_io();
2:cpu_io();
3:rd.l = stack_read();
4:rd.h = stack_read();
5:last_cycle();
  rd.b = stack_read();
  regs.pc.d = rd.d & 0xffffff;
  regs.pc.w++;
}
