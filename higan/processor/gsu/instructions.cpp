//$00 stop
auto GSU::op_stop() {
  if(regs.cfgr.irq == 0) {
    regs.sfr.irq = 1;
    stop();
  }
  regs.sfr.g = 0;
  regs.pipeline = 0x01;  //nop
  regs.reset();
}

//$01 nop
auto GSU::op_nop() {
  regs.reset();
}

//$02 cache
auto GSU::op_cache() {
  if(regs.cbr != (regs.r[15] & 0xfff0)) {
    regs.cbr = regs.r[15] & 0xfff0;
    cache_flush();
  }
  regs.reset();
}

//$03 lsr
auto GSU::op_lsr() {
  regs.sfr.cy = (regs.sr() & 1);
  regs.dr() = regs.sr() >> 1;
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$04 rol
auto GSU::op_rol() {
  bool carry = (regs.sr() & 0x8000);
  regs.dr() = (regs.sr() << 1) | regs.sfr.cy;
  regs.sfr.s  = (regs.dr() & 0x8000);
  regs.sfr.cy = carry;
  regs.sfr.z  = (regs.dr() == 0);
  regs.reset();
}

//$05 bra e
//$06 blt e
//$07 bge e
//$08 bne e
//$09 beq e
//$0a bpl e
//$0b bmi e
//$0c bcc e
//$0d bcs e
//$0e bvc e
//$0f bvs e
auto GSU::op_branch(bool c) {
  auto d = (int8)pipe();
  if(c) regs.r[15] += d;
}

//$10-1f(b0) to rN
//$10-1f(b1) move rN
auto GSU::op_to_move(uint n) {
  if(!regs.sfr.b) {
    regs.dreg = n;
  } else {
    regs.r[n] = regs.sr();
    regs.reset();
  }
}

//$20-2f with rN
auto GSU::op_with(uint n) {
  regs.sreg = n;
  regs.dreg = n;
  regs.sfr.b = 1;
}

//$30-3b(alt0) stw (rN)
//$30-3b(alt1) stb (rN)
auto GSU::op_store(uint n) {
  regs.ramaddr = regs.r[n];
  rambuffer_write(regs.ramaddr, regs.sr());
  if(!regs.sfr.alt1) rambuffer_write(regs.ramaddr ^ 1, regs.sr() >> 8);
  regs.reset();
}

//$3c loop
auto GSU::op_loop() {
  regs.r[12]--;
  regs.sfr.s = (regs.r[12] & 0x8000);
  regs.sfr.z = (regs.r[12] == 0);
  if(!regs.sfr.z) regs.r[15] = regs.r[13];
  regs.reset();
}

//$3d alt1
auto GSU::op_alt1() {
  regs.sfr.b = 0;
  regs.sfr.alt1 = 1;
}

//$3e alt2
auto GSU::op_alt2() {
  regs.sfr.b = 0;
  regs.sfr.alt2 = 1;
}

//$3f alt3
auto GSU::op_alt3() {
  regs.sfr.b = 0;
  regs.sfr.alt1 = 1;
  regs.sfr.alt2 = 1;
}

//$40-4b(alt0) ldw (rN)
//$40-4b(alt1) ldb (rN)
auto GSU::op_load(uint n) {
  regs.ramaddr = regs.r[n];
  regs.dr() = rambuffer_read(regs.ramaddr);
  if(!regs.sfr.alt1) regs.dr() |= rambuffer_read(regs.ramaddr ^ 1) << 8;
  regs.reset();
}

//$4c(alt0) plot
//$4c(alt1) rpix
auto GSU::op_plot_rpix() {
  if(!regs.sfr.alt1) {
    plot(regs.r[1], regs.r[2]);
    regs.r[1]++;
  } else {
    regs.dr() = rpix(regs.r[1], regs.r[2]);
    regs.sfr.s = (regs.dr() & 0x8000);
    regs.sfr.z = (regs.dr() == 0);
  }
  regs.reset();
}

//$4d swap
auto GSU::op_swap() {
  regs.dr() = (regs.sr() >> 8) | (regs.sr() << 8);
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$4e(alt0) color
//$4e(alt1) cmode
auto GSU::op_color_cmode() {
  if(!regs.sfr.alt1) {
    regs.colr = color(regs.sr());
  } else {
    regs.por = regs.sr();
  }
  regs.reset();
}

//$4f not
auto GSU::op_not() {
  regs.dr() = ~regs.sr();
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$50-5f(alt0) add rN
//$50-5f(alt1) adc rN
//$50-5f(alt2) add #N
//$50-5f(alt3) adc #N
auto GSU::op_add_adc(uint n) {
  if(!regs.sfr.alt2) n = regs.r[n];
  int r = regs.sr() + n + (regs.sfr.alt1 ? regs.sfr.cy : 0);
  regs.sfr.ov = ~(regs.sr() ^ n) & (n ^ r) & 0x8000;
  regs.sfr.s  = (r & 0x8000);
  regs.sfr.cy = (r >= 0x10000);
  regs.sfr.z  = ((uint16)r == 0);
  regs.dr() = r;
  regs.reset();
}

//$60-6f(alt0) sub rN
//$60-6f(alt1) sbc rN
//$60-6f(alt2) sub #N
//$60-6f(alt3) cmp rN
auto GSU::op_sub_sbc_cmp(uint n) {
  if(!regs.sfr.alt2 || regs.sfr.alt1) n = regs.r[n];
  int r = regs.sr() - n - (!regs.sfr.alt2 && regs.sfr.alt1 ? !regs.sfr.cy : 0);
  regs.sfr.ov = (regs.sr() ^ n) & (regs.sr() ^ r) & 0x8000;
  regs.sfr.s  = (r & 0x8000);
  regs.sfr.cy = (r >= 0);
  regs.sfr.z  = ((uint16)r == 0);
  if(!regs.sfr.alt2 || !regs.sfr.alt1) regs.dr() = r;
  regs.reset();
}

//$70 merge
auto GSU::op_merge() {
  regs.dr() = (regs.r[7] & 0xff00) | (regs.r[8] >> 8);
  regs.sfr.ov = (regs.dr() & 0xc0c0);
  regs.sfr.s  = (regs.dr() & 0x8080);
  regs.sfr.cy = (regs.dr() & 0xe0e0);
  regs.sfr.z  = (regs.dr() & 0xf0f0);
  regs.reset();
}

//$71-7f(alt0) and rN
//$71-7f(alt1) bic rN
//$71-7f(alt2) and #N
//$71-7f(alt3) bic #N
auto GSU::op_and_bic(uint n) {
  if(!regs.sfr.alt2) n = regs.r[n];
  regs.dr() = regs.sr() & (regs.sfr.alt1 ? ~n : n);
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$80-8f(alt0) mult rN
//$80-8f(alt1) umult rN
//$80-8f(alt2) mult #N
//$80-8f(alt3) umult #N
auto GSU::op_mult_umult(uint n) {
  if(!regs.sfr.alt2) n = regs.r[n];
  regs.dr() = (!regs.sfr.alt1 ? ((int8)regs.sr() * (int8)n) : ((uint8)regs.sr() * (uint8)n));
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
  if(!regs.cfgr.ms0) step(regs.clsr ? 1 : 2);
}

//$90 sbk
auto GSU::op_sbk() {
  rambuffer_write(regs.ramaddr ^ 0, regs.sr() >> 0);
  rambuffer_write(regs.ramaddr ^ 1, regs.sr() >> 8);
  regs.reset();
}

//$91-94 link #N
auto GSU::op_link(uint n) {
  regs.r[11] = regs.r[15] + n;
  regs.reset();
}

//$95 sex
auto GSU::op_sex() {
  regs.dr() = (int8)regs.sr();
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$96(alt0) asr
//$96(alt1) div2
auto GSU::op_asr_div2() {
  regs.sfr.cy = (regs.sr() & 1);
  regs.dr() = ((int16)regs.sr() >> 1) + (regs.sfr.alt1 ? ((regs.sr() + 1) >> 16) : 0);
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$97 ror
auto GSU::op_ror() {
  bool carry = (regs.sr() & 1);
  regs.dr() = (regs.sfr.cy << 15) | (regs.sr() >> 1);
  regs.sfr.s  = (regs.dr() & 0x8000);
  regs.sfr.cy = carry;
  regs.sfr.z  = (regs.dr() == 0);
  regs.reset();
}

//$98-9d(alt0) jmp rN
//$98-9d(alt1) ljmp rN
auto GSU::op_jmp_ljmp(uint n) {
  if(!regs.sfr.alt1) {
    regs.r[15] = regs.r[n];
  } else {
    regs.pbr = regs.r[n] & 0x7f;
    regs.r[15] = regs.sr();
    regs.cbr = regs.r[15] & 0xfff0;
    cache_flush();
  }
  regs.reset();
}

//$9e lob
auto GSU::op_lob() {
  regs.dr() = regs.sr() & 0xff;
  regs.sfr.s = (regs.dr() & 0x80);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$9f(alt0) fmult
//$9f(alt1) lmult
auto GSU::op_fmult_lmult() {
  uint32 result = (int16)regs.sr() * (int16)regs.r[6];
  if(regs.sfr.alt1) regs.r[4] = result;
  regs.dr() = result >> 16;
  regs.sfr.s  = (regs.dr() & 0x8000);
  regs.sfr.cy = (result & 0x8000);
  regs.sfr.z  = (regs.dr() == 0);
  regs.reset();
  step((regs.cfgr.ms0 ? 3 : 7) * (regs.clsr ? 1 : 2));
}

//$a0-af(alt0) ibt rN,#pp
//$a0-af(alt1) lms rN,(yy)
//$a0-af(alt2) sms (yy),rN
auto GSU::op_ibt_lms_sms(uint n) {
  if(regs.sfr.alt1) {
    regs.ramaddr = pipe() << 1;
    uint8 lo  = rambuffer_read(regs.ramaddr ^ 0) << 0;
    regs.r[n] = rambuffer_read(regs.ramaddr ^ 1) << 8 | lo;
  } else if(regs.sfr.alt2) {
    regs.ramaddr = pipe() << 1;
    rambuffer_write(regs.ramaddr ^ 0, regs.r[n] >> 0);
    rambuffer_write(regs.ramaddr ^ 1, regs.r[n] >> 8);
  } else {
    regs.r[n] = (int8)pipe();
  }
  regs.reset();
}

//$b0-bf(b0) from rN
//$b0-bf(b1) moves rN
auto GSU::op_from_moves(uint n) {
  if(!regs.sfr.b) {
    regs.sreg = n;
  } else {
    regs.dr() = regs.r[n];
    regs.sfr.ov = (regs.dr() & 0x80);
    regs.sfr.s  = (regs.dr() & 0x8000);
    regs.sfr.z  = (regs.dr() == 0);
    regs.reset();
  }
}

//$c0 hib
auto GSU::op_hib() {
  regs.dr() = regs.sr() >> 8;
  regs.sfr.s = (regs.dr() & 0x80);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$c1-cf(alt0) or rN
//$c1-cf(alt1) xor rN
//$c1-cf(alt2) or #N
//$c1-cf(alt3) xor #N
auto GSU::op_or_xor(uint n) {
  if(!regs.sfr.alt2) n = regs.r[n];
  regs.dr() = (!regs.sfr.alt1 ? (regs.sr() | n) : (regs.sr() ^ n));
  regs.sfr.s = (regs.dr() & 0x8000);
  regs.sfr.z = (regs.dr() == 0);
  regs.reset();
}

//$d0-de inc rN
auto GSU::op_inc(uint n) {
  regs.r[n]++;
  regs.sfr.s = (regs.r[n] & 0x8000);
  regs.sfr.z = (regs.r[n] == 0);
  regs.reset();
}

//$df(alt0) getc
//$df(alt2) ramb
//$df(alt3) romb
auto GSU::op_getc_ramb_romb() {
  if(!regs.sfr.alt2) {
    regs.colr = color(rombuffer_read());
  } else if(!regs.sfr.alt1) {
    rambuffer_sync();
    regs.rambr = regs.sr() & 0x01;
  } else {
    rombuffer_sync();
    regs.rombr = regs.sr() & 0x7f;
  }
  regs.reset();
}

//$e0-ee dec rN
auto GSU::op_dec(uint n) {
  regs.r[n]--;
  regs.sfr.s = (regs.r[n] & 0x8000);
  regs.sfr.z = (regs.r[n] == 0);
  regs.reset();
}

//$ef(alt0) getb
//$ef(alt1) getbh
//$ef(alt2) getbl
//$ef(alt3) getbs
auto GSU::op_getb() {
  switch(regs.sfr.alt2 << 1 | regs.sfr.alt1 << 0) {
  case 0: regs.dr() = rombuffer_read(); break;
  case 1: regs.dr() = rombuffer_read() << 8 | (uint8)regs.sr(); break;
  case 2: regs.dr() = (regs.sr() & 0xff00) | rombuffer_read(); break;
  case 3: regs.dr() = (int8)rombuffer_read(); break;
  }
  regs.reset();
}

//$f0-ff(alt0) iwt rN,#xx
//$f0-ff(alt1) lm rN,(xx)
//$f0-ff(alt2) sm (xx),rN
auto GSU::op_iwt_lm_sm(uint n) {
  if(regs.sfr.alt1) {
    regs.ramaddr  = pipe() << 0;
    regs.ramaddr |= pipe() << 8;
    uint8 lo  = rambuffer_read(regs.ramaddr ^ 0) << 0;
    regs.r[n] = rambuffer_read(regs.ramaddr ^ 1) << 8 | lo;
  } else if(regs.sfr.alt2) {
    regs.ramaddr  = pipe() << 0;
    regs.ramaddr |= pipe() << 8;
    rambuffer_write(regs.ramaddr ^ 0, regs.r[n] >> 0);
    rambuffer_write(regs.ramaddr ^ 1, regs.r[n] >> 8);
  } else {
    uint8 lo  = pipe();
    regs.r[n] = pipe() << 8 | lo;
  }
  regs.reset();
}
