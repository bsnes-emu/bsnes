nop(0xea) {
1:last_cycle();
  op_io_irq();
}

wdm(0x42) {
1:last_cycle();
  op_readpc();
}

xba(0xeb) {
1:op_io();
2:last_cycle();
  op_io();
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

mvn(0x54, ++),
mvp(0x44, --) {
1:dp = op_readpc();
2:sp = op_readpc();
3:regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
4:op_writelong((dp << 16) | regs.y.w, rd.l);
5:op_io();
  if(regs.p.x) {
    regs.x.l $1;
    regs.y.l $1;
  } else {
    regs.x.w $1;
    regs.y.w $1;
  }
6:last_cycle();
  op_io();
  if(regs.a.w--) regs.pc.w -= 3;
}

brk(0x00, 0xfffe, 0xffff, 0xffe6, 0xffe7),
cop(0x02, 0xfff4, 0xfff5, 0xffe4, 0xffe5) {
1:op_readpc();
2:if(!regs.e) op_writestack(regs.pc.b);
3:op_writestack(regs.pc.h);
4:op_writestack(regs.pc.l);
5:op_writestack(regs.p);
6:rd.l = op_readlong(regs.e ? $1 : $3);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
7:last_cycle();
  rd.h = op_readlong(regs.e ? $2 : $4);
  regs.pc.w = rd.w;
}

stp(0xdb) {
1:op_io();
2:last_cycle();
  while(true) op_io();
}

wai(0xcb) {
//last_cycle() will clear event.wai once an NMI / IRQ edge is reached
1:event.wai = true;
  while(event.wai) {
    last_cycle();
    op_io();
  }
2:op_io();
}

xce(0xfb) {
1:last_cycle();
  op_io_irq();
  bool carry = regs.p.c;
  regs.p.c = regs.e;
  regs.e = carry;
  if(regs.e) {
    regs.p |= 0x30;
    regs.s.h = 0x01;
  }
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

clc(0x18, regs.p.c = 0),
cld(0xd8, regs.p.d = 0),
cli(0x58, regs.p.i = 0),
clv(0xb8, regs.p.v = 0),
sec(0x38, regs.p.c = 1),
sed(0xf8, regs.p.d = 1),
sei(0x78, regs.p.i = 1) {
1:last_cycle();
  op_io_irq();
  $1;
}

rep(0xc2, &=~),
sep(0xe2, |=) {
1:rd.l = op_readpc();
2:last_cycle();
  op_io();
  regs.p $1 rd.l;
  if(regs.e) regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

tax(0xaa, regs.p.x, x, a),
tay(0xa8, regs.p.x, y, a),
txa(0x8a, regs.p.m, a, x),
txy(0x9b, regs.p.x, y, x),
tya(0x98, regs.p.m, a, y),
tyx(0xbb, regs.p.x, x, y) {
1:last_cycle();
  op_io_irq();
  if($1) {
    regs.$2.l = regs.$3.l;
    regs.p.n = !!(regs.$2.l & 0x80);
    regs.p.z = (regs.$2.l == 0);
  } else {
    regs.$2.w = regs.$3.w;
    regs.p.n = !!(regs.$2.w & 0x8000);
    regs.p.z = (regs.$2.w == 0);
  }
}

tcd(0x5b) {
1:last_cycle();
  op_io_irq();
  regs.d.w = regs.a.w;
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

tcs(0x1b) {
1:last_cycle();
  op_io_irq();
  regs.s.w = regs.a.w;
  if(regs.e) regs.s.h = 0x01;
}

tdc(0x7b) {
1:last_cycle();
  op_io_irq();
  regs.a.w = regs.d.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

tsc(0x3b) {
1:last_cycle();
  op_io_irq();
  regs.a.w = regs.s.w;
  if(regs.e) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

tsx(0xba) {
1:last_cycle();
  op_io_irq();
  if(regs.p.x) {
    regs.x.l = regs.s.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.s.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
}

txs(0x9a) {
1:last_cycle();
  op_io_irq();
  if(regs.e) {
    regs.s.l = regs.x.l;
  } else {
    regs.s.w = regs.x.w;
  }
}

pha(0x48, regs.p.m, a),
phx(0xda, regs.p.x, x),
phy(0x5a, regs.p.x, y) {
1:op_io();
2:if(!$1)op_writestack(regs.$2.h);
3:last_cycle();
  op_writestack(regs.$2.l);
}

phd(0x0b) {
1:op_io();
2:op_writestackn(regs.d.h);
3:last_cycle();
  op_writestackn(regs.d.l);
  if(regs.e) regs.s.h = 0x01;
}

phb(0x8b, regs.db),
phk(0x4b, regs.pc.b),
php(0x08, regs.p) {
1:op_io();
2:last_cycle();
  op_writestack($1);
}

pla(0x68, regs.p.m, a),
plx(0xfa, regs.p.x, x),
ply(0x7a, regs.p.x, y) {
1:op_io();
2:op_io();
3:if($1)last_cycle();
  regs.$2.l = op_readstack();
  if($1) {
    regs.p.n = !!(regs.$2.l & 0x80);
    regs.p.z = (regs.$2.l == 0);
    end;
  }
4:last_cycle();
  regs.$2.h = op_readstack();
  regs.p.n = !!(regs.$2.w & 0x8000);
  regs.p.z = (regs.$2.w == 0);
}

pld(0x2b) {
1:op_io();
2:op_io();
3:regs.d.l = op_readstackn();
4:last_cycle();
  regs.d.h = op_readstackn();
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
  if(regs.e) regs.s.h = 0x01;
}

plb(0xab) {
1:op_io();
2:op_io();
3:last_cycle();
  regs.db = op_readstack();
  regs.p.n = !!(regs.db & 0x80);
  regs.p.z = (regs.db == 0);
}

plp(0x28) {
1:op_io();
2:op_io();
3:last_cycle();
  regs.p = op_readstack();
  if(regs.e) regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

pea(0xf4) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_writestackn(aa.h);
4:last_cycle();
  op_writestackn(aa.l);
  if(regs.e) regs.s.h = 0x01;
}

pei(0xd4) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:op_writestackn(aa.h);
6:last_cycle();
  op_writestackn(aa.l);
  if(regs.e) regs.s.h = 0x01;
}

per(0x62) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io();
  rd.w = regs.pc.d + (int16)aa.w;
4:op_writestackn(rd.h);
5:last_cycle();
  op_writestackn(rd.l);
  if(regs.e) regs.s.h = 0x01;
}
