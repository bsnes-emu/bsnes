bcc(0x90, !regs.p.c),
bcs(0xb0, regs.p.c),
bne(0xd0, !regs.p.z),
beq(0xf0, regs.p.z),
bpl(0x10, !regs.p.n),
bmi(0x30, regs.p.n),
bvc(0x50, !regs.p.v),
bvs(0x70, regs.p.v) {
1:if(!$1)last_cycle();
  rd.l = op_readpc();
  if($1) {
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
  } else {
    end;
  }
2:op_io_cond6(aa.w);
3:last_cycle();
  op_io();
}

bra(0x80) {
1:rd.l = op_readpc();
  aa.w = regs.pc.d + (int8)rd.l;
  regs.pc.w = aa.w;
2:op_io_cond6(aa.w);
3:last_cycle();
  op_io();
}

brl(0x82) {
1:rd.l = op_readpc();
2:rd.h = op_readpc();
3:last_cycle();
  op_io();
  regs.pc.w = regs.pc.d + (int16)rd.w;
}

jmp_addr(0x4c) {
1:rd.l = op_readpc();
2:last_cycle();
  rd.h = op_readpc();
  regs.pc.w = rd.w;
}

jmp_long(0x5c) {
1:rd.l = op_readpc();
2:rd.h = op_readpc();
3:last_cycle();
  rd.b = op_readpc();
  regs.pc.d = rd.d & 0xffffff;
}

jmp_iaddr(0x6c) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:rd.l = op_readaddr(aa.w);
4:last_cycle();
  rd.h = op_readaddr(aa.w + 1);
  regs.pc.w = rd.w;
}

jmp_iaddrx(0x7c) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io();
4:rd.l = op_readpbr(aa.w + regs.x.w);
5:last_cycle();
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

jmp_iladdr(0xdc) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:rd.l = op_readaddr(aa.w);
4:rd.h = op_readaddr(aa.w + 1);
5:last_cycle();
  rd.b = op_readaddr(aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
}

jsr_addr(0x20) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io();
4:regs.pc.w--;
  op_writestack(regs.pc.h);
5:last_cycle();
  op_writestack(regs.pc.l);
  regs.pc.w = aa.w;
}

jsr_long(0x22) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_writestack(regs.pc.b);
4:op_io();
5:aa.b = op_readpc();
6:regs.pc.w--;
  op_writestack(regs.pc.h);
7:last_cycle();
  op_writestack(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
}

jsr_iaddrx(0xfc) {
1:aa.l = op_readpc();
2:op_writestack(regs.pc.h);
3:op_writestack(regs.pc.l);
4:aa.h = op_readpc();
5:op_io();
6:rd.l = op_readpbr(aa.w + regs.x.w);
7:last_cycle();
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

rti(0x40) {
1:op_io();
2:op_io();
3:regs.p = op_readstack();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
4:rd.l = op_readstack();
5:if(regs.e)last_cycle();
  rd.h = op_readstack();
  if(regs.e) {
    regs.pc.w = rd.w;
    end;
  }
6:last_cycle();
  rd.b = op_readstack();
  regs.pc.d = rd.d & 0xffffff;
}

rts(0x60) {
1:op_io();
2:op_io();
3:rd.l = op_readstack();
4:rd.h = op_readstack();
5:last_cycle();
  op_io();
  regs.pc.w = rd.w;
  regs.pc.w++;
}

rtl(0x6b) {
1:op_io();
2:op_io();
3:rd.l = op_readstack();
4:rd.h = op_readstack();
5:last_cycle();
  rd.b = op_readstack();
  regs.pc.d = rd.d & 0xffffff;
  regs.pc.w++;
}
