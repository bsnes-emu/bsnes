//op_read
inline void sCPU::op_adc_b() {
int32 r = regs.a.l + rd.l + regs.p.c;
  if(regs.p.d) {
  uint8 n0 = (regs.a.l     ) & 15;
  uint8 n1 = (regs.a.l >> 4) & 15;
    n0 += ((rd.l) & 15) + regs.p.c;
    if(n0 > 9) {
      n0 -= 10;
      n0 &= 15;
      n1++;
    }
    n1 += ((rd.l >> 4) & 15);
    if(n1 > 9) {
      n1 -= 10;
      n1 &= 15;
      regs.p.c = 1;
    } else {
      regs.p.c = 0;
    }
    r = (n1 << 4) | (n0);
  } else {
    r = regs.a.l + rd.l + regs.p.c;
    regs.p.c = (r > 0xff);
  }
  regs.p.n = bool(r & 0x80);
  regs.p.v = bool(~(regs.a.l ^ rd.l) & (regs.a.l ^ r) & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.a.l = r;
}

inline void sCPU::op_adc_w() {
int32 r;
  if(regs.p.d) {
  uint8 n0 = (regs.a.w      ) & 15;
  uint8 n1 = (regs.a.w >>  4) & 15;
  uint8 n2 = (regs.a.w >>  8) & 15;
  uint8 n3 = (regs.a.w >> 12) & 15;
    n0 += ((rd.w) & 15) + regs.p.c;
    if(n0 > 9) {
      n0 -= 10;
      n0 &= 15;
      n1++;
    }
    n1 += ((rd.w >> 4) & 15);
    if(n1 > 9) {
      n1 -= 10;
      n1 &= 15;
      n2++;
    }
    n2 += ((rd.w >> 8) & 15);
    if(n2 > 9) {
      n2 -= 10;
      n2 &= 15;
      n3++;
    }
    n3 += ((rd.w >> 12) & 15);
    if(n3 > 9) {
      n3 -= 10;
      n3 &= 15;
      regs.p.c = 1;
    } else {
      regs.p.c = 0;
    }
    r = (n3 << 12) | (n2 << 8) | (n1 << 4) | (n0);
  } else {
    r = regs.a.w + rd.w + regs.p.c;
    regs.p.c = (r > 0xffff);
  }
  regs.p.n = bool(r & 0x8000);
  regs.p.v = bool(~(regs.a.w ^ rd.w) & (regs.a.w ^ r) & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.a.w = r;
}

inline void sCPU::op_and_b() {
  regs.a.l &= rd.l;
  regs.p.n = bool(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

inline void sCPU::op_and_w() {
  regs.a.w &= rd.w;
  regs.p.n = bool(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

inline void sCPU::op_bit_b() {
  regs.p.n = bool(rd.l & 0x80);
  regs.p.v = bool(rd.l & 0x40);
  regs.p.z = ((rd.l & regs.a.l) == 0);
}

inline void sCPU::op_bit_w() {
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.v = bool(rd.w & 0x4000);
  regs.p.z = ((rd.w & regs.a.w) == 0);
}

inline void sCPU::op_cmp_b() {
int32 r = regs.a.l - rd.l;
  regs.p.n = bool(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
}

inline void sCPU::op_cmp_w() {
int32 r = regs.a.w - rd.w;
  regs.p.n = bool(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
}

inline void sCPU::op_cpx_b() {
int32 r = regs.x.l - rd.l;
  regs.p.n = bool(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
}

inline void sCPU::op_cpx_w() {
int32 r = regs.x.w - rd.w;
  regs.p.n = bool(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
}

inline void sCPU::op_cpy_b() {
int32 r = regs.y.l - rd.l;
  regs.p.n = bool(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
}

inline void sCPU::op_cpy_w() {
int32 r = regs.y.w - rd.w;
  regs.p.n = bool(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
}

inline void sCPU::op_eor_b() {
  regs.a.l ^= rd.l;
  regs.p.n = bool(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

inline void sCPU::op_eor_w() {
  regs.a.w ^= rd.w;
  regs.p.n = bool(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

inline void sCPU::op_lda_b() {
  regs.a.l = rd.l;
  regs.p.n = bool(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

inline void sCPU::op_lda_w() {
  regs.a.w = rd.w;
  regs.p.n = bool(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

inline void sCPU::op_ldx_b() {
  regs.x.l = rd.l;
  regs.p.n = bool(regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

inline void sCPU::op_ldx_w() {
  regs.x.w = rd.w;
  regs.p.n = bool(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

inline void sCPU::op_ldy_b() {
  regs.y.l = rd.l;
  regs.p.n = bool(regs.y.l & 0x80);
  regs.p.z = (regs.y.l == 0);
}

inline void sCPU::op_ldy_w() {
  regs.y.w = rd.w;
  regs.p.n = bool(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

inline void sCPU::op_ora_b() {
  regs.a.l |= rd.l;
  regs.p.n = bool(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

inline void sCPU::op_ora_w() {
  regs.a.w |= rd.w;
  regs.p.n = bool(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

inline void sCPU::op_sbc_b() {
int32 r;
  if(regs.p.d) {
  uint8 n0 = (regs.a.l     ) & 15;
  uint8 n1 = (regs.a.l >> 4) & 15;
    n0 -= ((rd.l     ) & 15) + !regs.p.c;
    n1 -= ((rd.l >> 4) & 15);
    if(n0 > 9) {
      n0 += 10;
      n1--;
    }
    if(n1 > 9) {
      n1 += 10;
      regs.p.c = 0;
    } else {
      regs.p.c = 1;
    }
    r = (n1 << 4) | (n0);
  } else {
    r = regs.a.l - rd.l - !regs.p.c;
    regs.p.c = (r >= 0);
  }
  regs.p.n = bool(r & 0x80);
  regs.p.v = bool((regs.a.l ^ rd.l) & (regs.a.l ^ r) & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.a.l = r;
}

inline void sCPU::op_sbc_w() {
int32 r;
  if(regs.p.d) {
  uint8 n0 = (regs.a.w      ) & 15;
  uint8 n1 = (regs.a.w >>  4) & 15;
  uint8 n2 = (regs.a.w >>  8) & 15;
  uint8 n3 = (regs.a.w >> 12) & 15;
    n0 -= ((rd.w      ) & 15) + !regs.p.c;
    n1 -= ((rd.w >>  4) & 15);
    n2 -= ((rd.w >>  8) & 15);
    n3 -= ((rd.w >> 12) & 15);
    if(n0 > 9) {
      n0 += 10;
      n1--;
    }
    if(n1 > 9) {
      n1 += 10;
      n2--;
    }
    if(n2 > 9) {
      n2 += 10;
      n3--;
    }
    if(n3 > 9) {
      n3 += 10;
      regs.p.c = 0;
    } else {
      regs.p.c = 1;
    }
    r = (n3 << 12) | (n2 << 8) | (n1 << 4) | (n0);
  } else {
    r = regs.a.w - rd.w - !regs.p.c;
    regs.p.c = (r >= 0);
  }
  regs.p.n = bool(r & 0x8000);
  regs.p.v = bool((regs.a.w ^ rd.w) & (regs.a.w ^ r) & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.a.w = r;
}

//op_rmw
inline void sCPU::op_inc_b() {
  rd.l++;
  regs.p.n = bool(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

inline void sCPU::op_inc_w() {
  rd.w++;
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

inline void sCPU::op_dec_b() {
  rd.l--;
  regs.p.n = bool(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

inline void sCPU::op_dec_w() {
  rd.w--;
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

inline void sCPU::op_asl_b() {
  regs.p.c = bool(rd.l & 0x80);
  rd.l <<= 1;
  regs.p.n = bool(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

inline void sCPU::op_asl_w() {
  regs.p.c = bool(rd.w & 0x8000);
  rd.w <<= 1;
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

inline void sCPU::op_lsr_b() {
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  regs.p.n = bool(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

inline void sCPU::op_lsr_w() {
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

inline void sCPU::op_rol_b() {
uint16 c = regs.p.c;
  regs.p.c = bool(rd.l & 0x80);
  rd.l <<= 1;
  rd.l |= c;
  regs.p.n = bool(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

inline void sCPU::op_rol_w() {
uint16 c = regs.p.c;
  regs.p.c = bool(rd.w & 0x8000);
  rd.w <<= 1;
  rd.w |= c;
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

inline void sCPU::op_ror_b() {
uint16 c = (regs.p.c)?0x80:0;
  regs.p.c = rd.l & 1;
  rd.l >>= 1;
  rd.l |= c;
  regs.p.n = bool(rd.l & 0x80);
  regs.p.z = (rd.l == 0);
}

inline void sCPU::op_ror_w() {
uint16 c = (regs.p.c)?0x8000:0;
  regs.p.c = rd.w & 1;
  rd.w >>= 1;
  rd.w |= c;
  regs.p.n = bool(rd.w & 0x8000);
  regs.p.z = (rd.w == 0);
}

inline void sCPU::op_trb_b() {
  regs.p.z = ((rd.l & regs.a.l) == 0);
  rd.l &= ~regs.a.l;
}

inline void sCPU::op_trb_w() {
  regs.p.z = ((rd.w & regs.a.w) == 0);
  rd.w &= ~regs.a.w;
}

inline void sCPU::op_tsb_b() {
  regs.p.z = ((rd.l & regs.a.l) == 0);
  rd.l |= regs.a.l;
}

inline void sCPU::op_tsb_w() {
  regs.p.z = ((rd.w & regs.a.w) == 0);
  rd.w |= regs.a.w;
}
