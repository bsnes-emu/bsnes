void bCPU::op_nop() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_wdm() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    op_read();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_xba() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    last_cycle();
    cpu_io();
    regs.a.l ^= regs.a.h;
    regs.a.h ^= regs.a.l;
    regs.a.l ^= regs.a.h;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_mvn() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    sp = op_read();
    } break;
  case 3: {
    regs.db = dp;
    rd.l = op_read(OPMODE_LONG, (sp << 16) | regs.x.w);
    } break;
  case 4: {
    op_write(OPMODE_LONG, (dp << 16) | regs.y.w, rd.l);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.x) { regs.x.l++; regs.y.l++; }
    else         { regs.x.w++; regs.y.w++; }
    } break;
  case 6: {
    last_cycle();
    cpu_io();
    if(regs.a.w--)regs.pc.w -= 3;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_mvp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    sp = op_read();
    } break;
  case 3: {
    regs.db = dp;
    rd.l = op_read(OPMODE_LONG, (sp << 16) | regs.x.w);
    } break;
  case 4: {
    op_write(OPMODE_LONG, (dp << 16) | regs.y.w, rd.l);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.x) { regs.x.l--; regs.y.l--; }
    else         { regs.x.w--; regs.y.w--; }
    } break;
  case 6: {
    last_cycle();
    cpu_io();
    if(regs.a.w--)regs.pc.w -= 3;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_brk() {
  switch(status.cycle_pos++) {
  case 1: {
    op_read();
    if(regs.e)status.cycle_pos++;
    } break;
  case 2: {
    stack_write(regs.pc.b);
    } break;
  case 3: {
    stack_write(regs.pc.h);
    } break;
  case 4: {
    stack_write(regs.pc.l);
    } break;
  case 5: {
    stack_write(regs.p);
    } break;
  case 6: {
    rd.l = op_read(OPMODE_LONG, (regs.e)?0xfffe:0xffe6);
    } break;
  case 7: {
    last_cycle();
    rd.h = op_read(OPMODE_LONG, (regs.e)?0xffff:0xffe7);
    regs.pc.b = 0x00;
    regs.pc.w = rd.w;
    regs.p.i  = 1;
    regs.p.d  = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_cop() {
  switch(status.cycle_pos++) {
  case 1: {
    op_read();
    if(regs.e)status.cycle_pos++;
    } break;
  case 2: {
    stack_write(regs.pc.b);
    } break;
  case 3: {
    stack_write(regs.pc.h);
    } break;
  case 4: {
    stack_write(regs.pc.l);
    } break;
  case 5: {
    stack_write(regs.p);
    } break;
  case 6: {
    rd.l = op_read(OPMODE_LONG, (regs.e)?0xfff4:0xffe4);
    } break;
  case 7: {
    last_cycle();
    rd.h = op_read(OPMODE_LONG, (regs.e)?0xfff5:0xffe5);
    regs.pc.b = 0x00;
    regs.pc.w = rd.w;
    regs.p.i  = 1;
    regs.p.d  = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stp() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    run_state.stp = true;
    } break;
  case 2: {
    last_cycle();
    cpu_io();
    regs.pc.w--;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_wai() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    run_state.wai = true;
    } break;
  case 2: {
    last_cycle();
    cpu_io();
    if(run_state.wai) {
    //this can be cleared within last_cycle()
      regs.pc.w--;
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_xce() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
  bool c = regs.p.c;
    regs.p.c = regs.e;
    regs.e = c;
    if(regs.e) {
      regs.p  |= 0x30;
      regs.x.h = 0x00;
      regs.y.h = 0x00;
      regs.s.h = 0x01;
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_clc() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.c = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_cld() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.d = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_cli() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.i = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_clv() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.v = 0;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sec() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.c = 1;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sed() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.d = 1;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sei() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.p.i = 1;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rep() {
  switch(status.cycle_pos++) {
  case 1: {
    rd.l = op_read();
    } break;
  case 2: {
    last_cycle();
    cpu_io();
    regs.p &=~ rd.l;
    if(regs.e)regs.p |= 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sep() {
  switch(status.cycle_pos++) {
  case 1: {
    rd.l = op_read();
    } break;
  case 2: {
    last_cycle();
    cpu_io();
    regs.p |= rd.l;
    if(regs.e)regs.p |= 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tax() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.x) {
      regs.x.l = regs.a.l;
      regs.p.n = !!(regs.x.l & 0x80);
      regs.p.z = (regs.x.l == 0);
    } else {
      regs.x.w = regs.a.w;
      regs.p.n = !!(regs.x.w & 0x8000);
      regs.p.z = (regs.x.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tay() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.x) {
      regs.y.l = regs.a.l;
      regs.p.n = !!(regs.y.l & 0x80);
      regs.p.z = (regs.y.l == 0);
    } else {
      regs.y.w = regs.a.w;
      regs.p.n = !!(regs.y.w & 0x8000);
      regs.p.z = (regs.y.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_txa() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.m) {
      regs.a.l = regs.x.l;
      regs.p.n = !!(regs.a.l & 0x80);
      regs.p.z = (regs.a.l == 0);
    } else {
      regs.a.w = regs.x.w;
      regs.p.n = !!(regs.a.w & 0x8000);
      regs.p.z = (regs.a.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_txy() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.x) {
      regs.y.l = regs.x.l;
      regs.p.n = !!(regs.y.l & 0x80);
      regs.p.z = (regs.y.l == 0);
    } else {
      regs.y.w = regs.x.w;
      regs.p.n = !!(regs.y.w & 0x8000);
      regs.p.z = (regs.y.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tya() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.m) {
      regs.a.l = regs.y.l;
      regs.p.n = !!(regs.a.l & 0x80);
      regs.p.z = (regs.a.l == 0);
    } else {
      regs.a.w = regs.y.w;
      regs.p.n = !!(regs.a.w & 0x8000);
      regs.p.z = (regs.a.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tyx() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.x) {
      regs.x.l = regs.y.l;
      regs.p.n = !!(regs.x.l & 0x80);
      regs.p.z = (regs.x.l == 0);
    } else {
      regs.x.w = regs.y.w;
      regs.p.n = !!(regs.x.w & 0x8000);
      regs.p.z = (regs.x.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tcd() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.d.w = regs.a.w;
    regs.p.n = !!(regs.d.w & 0x8000);
    regs.p.z = (regs.d.w == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tcs() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.s.w = regs.a.w;
    if(regs.e)regs.s.h = 0x01;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tdc() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.a.w = regs.d.w;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tsc() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    regs.a.w = regs.s.w;
    if(regs.e) {
      regs.p.n = !!(regs.a.l & 0x80);
      regs.p.z = (regs.a.l == 0);
    } else {
      regs.p.n = !!(regs.a.w & 0x8000);
      regs.p.z = (regs.a.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tsx() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.p.x) {
      regs.x.l = regs.s.l;
      regs.p.n = !!(regs.x.l & 0x80);
      regs.p.z = (regs.x.l == 0);
    } else {
      regs.x.w = regs.s.w;
      regs.p.n = !!(regs.x.w & 0x8000);
      regs.p.z = (regs.x.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_txs() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
    cpu_io();
    if(regs.e) {
      regs.s.l = regs.x.l;
      regs.p.n = !!(regs.s.l & 0x80);
      regs.p.z = (regs.s.l == 0);
    } else {
      regs.s.w = regs.x.w;
      regs.p.n = !!(regs.s.w & 0x8000);
      regs.p.z = (regs.s.w == 0);
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_pha() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 2: {
    stack_write(regs.a.h);
    } break;
  case 3: {
    last_cycle();
    stack_write(regs.a.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_phx() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    if(regs.p.x)status.cycle_pos++;
    } break;
  case 2: {
    stack_write(regs.x.h);
    } break;
  case 3: {
    last_cycle();
    stack_write(regs.x.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_phy() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    if(regs.p.x)status.cycle_pos++;
    } break;
  case 2: {
    stack_write(regs.y.h);
    } break;
  case 3: {
    last_cycle();
    stack_write(regs.y.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_phd() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    if(0)status.cycle_pos++;
    } break;
  case 2: {
    stack_write(regs.       d.h);
    } break;
  case 3: {
    last_cycle();
    stack_write(regs.       d.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_phb() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    last_cycle();
    stack_write(regs.db);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_phk() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    last_cycle();
    stack_write(regs.pc.b);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_php() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    last_cycle();
    stack_write(regs.p);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_pla() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    if(regs.p.m)last_cycle();
    regs.a.l = stack_read();
    if(regs.p.m) {
      regs.p.n = !!(regs.a.l & 0x80);
      regs.p.z = (regs.a.l == 0);
      status.cycle_pos = 0;
    }
    } break;
  case 4: {
    last_cycle();
    regs.a.h = stack_read();
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_plx() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    if(regs.p.x)last_cycle();
    regs.x.l = stack_read();
    if(regs.p.x) {
      regs.p.n = !!(regs.x.l & 0x80);
      regs.p.z = (regs.x.l == 0);
      status.cycle_pos = 0;
    }
    } break;
  case 4: {
    last_cycle();
    regs.x.h = stack_read();
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_ply() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    if(regs.p.x)last_cycle();
    regs.y.l = stack_read();
    if(regs.p.x) {
      regs.p.n = !!(regs.y.l & 0x80);
      regs.p.z = (regs.y.l == 0);
      status.cycle_pos = 0;
    }
    } break;
  case 4: {
    last_cycle();
    regs.y.h = stack_read();
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_pld() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    if(0)last_cycle();
    regs.       d.l = stack_read();
    if(0) {
      regs.p.n = !!(regs.       d.l & 0x80);
      regs.p.z = (regs.       d.l == 0);
      status.cycle_pos = 0;
    }
    } break;
  case 4: {
    last_cycle();
    regs.       d.h = stack_read();
    regs.p.n = !!(regs.       d.w & 0x8000);
    regs.p.z = (regs.       d.w == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_plb() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    last_cycle();
    regs.db = stack_read();
    regs.p.n = !!(regs.db & 0x80);
    regs.p.z = (regs.db == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_plp() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    last_cycle();
    regs.p = stack_read();
    if(regs.e)regs.p |= 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_pea() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    stack_write(aa.h);
    } break;
  case 4: {
    last_cycle();
    stack_write(aa.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_pei() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    aa.l = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    aa.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    stack_write(aa.h);
    } break;
  case 6: {
    last_cycle();
    stack_write(aa.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_per() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    rd.w = regs.pc.d + (int16)aa.w;
    } break;
  case 4: {
    stack_write(rd.h);
    } break;
  case 5: {
    last_cycle();
    stack_write(rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

