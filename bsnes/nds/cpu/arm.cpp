
void ARMCore::armWritePsr(uint1 opcode, uint4 mask, uint32 rm) {
  r[15] += 4;
  
  if(opcode == 0) return writeCpsr(rm, mask);
  if(opcode == 1) {
    if(mode == USR || mode == SYS) return;
    
    uint32 update = 0xff000000*(mask>>3) | 0xff*(mask&1);
    spsr() ^= (spsr() ^ rm) & update;
  }
}

void ARMCore::armReadPsr(uint1 opcode, uint4 ird) {
  r[15] += 4;
  
  if(opcode == 0) r[ird] = readCpsr();
  if(opcode == 1) {
    if(mode == USR || mode == SYS) return;
    r[ird] = spsr();
  }
  if(ird == 15) branch(0, r[15]);
}

void ARMCore::armBranch(uint1 link, uint1 exch, int26 offset) {
  if(link) r[14] = r[15] - 4;
  branch(exch, r[15] + offset);
}

void ARMCore::armBranchEx(uint1 link, uint4 irm) {
  if(link) r[14] = r[15] - 4;
  branch(r[irm] & 1, r[irm]);
}

void ARMCore::armClz(uint4 ird, uint4 irm) {
  auto &rd = r[ird], rm = r[irm];
  r[15] += 4;
  
  if(rm == 0) { rd = 32; return; }
  
  rd = 0;
  while(~rm & 1<<31)
    rm <<= 1, rd++;
}

void ARMCore::armDspAdd(uint2 opcode, uint4 ird, uint4 irn, uint4 irm) {
  auto &rd = r[ird], rn = r[irn], rm = r[irm];
  r[15] += 4;
  
  if(opcode & 2) {  // qdadd/qdsub
    rd = rm, rm += rm;
    if(oflow(rm,rd,rd) & 1<<31)
      Qf = -1, rm = 0x80000000 - (rm>>31);
  }
  if(opcode & 1) rd = rn - rm, (rm = ~rm);  // qsub
  else           rd = rn + rm;              // qadd
  
  if(oflow(rd,rn,rm) & 1<<31)
    Qf = -1, rd = 0x80000000 - (rd>>31);
  
  if(ird == 15) branch(0, r[15]);
}

void ARMCore::armDspMul(uint2 opcode, uint2 xy, uint4 ird, uint4 irn, uint4 irm, uint4 irs) {
  auto rn = r[irn];
  r[15] += 4;
  
  if(opcode == 1) {  // smulwy, smlawy
    int32  rm = r[irm];
    int16  rs = r[irs] >> 16*(xy>>1);
    uint32 rd = (int64) rm * rs >> 16;
    
    if(xy & 1) {
      uint32 ra = rd + rn;
      Qf |= oflow(ra,rd,rn);
      rd = ra;
    }
    r[ird] = rd;
  }
  else {  // smulxy(3), smlaxy(0), smlalxy(2)
    int16 rm = r[irm] >> 16*(xy & 1);
    int16 rs = r[irs] >> 16*(xy>>1);
    int64 rd = (int64) rm * rs;
    
    if(opcode == 0) Qf |= oflow(rd+rn, rd, rn);  // smlaxy
    if(opcode != 3 /*accumulate*/) rd += rn;  // smlaxy, smlalxy
    if(opcode == 2 /*long*/) {
      r[irn] = rd; rd >>= 32;
      rd += r[ird];
      if(irn == 15) branch(0, r[15]);
    }
    r[ird] = rd;
  }
  if(ird == 15) branch(0, r[15]);
}

void ARMCore::armMultiply(uint4 opcode, uint4 ird, uint4 irn, uint4 irm, uint4 irs) {
  bool long_mul   = opcode & 8,   accumulate = opcode & 2;
  bool signed_mul = opcode & 4,   setf       = opcode & 1;
  
  auto rm = r[irm], rs = r[irs], rn = r[irn];
  int64 rd = (uint64) rm * rs;
  r[15] += 4;
  
  if(setf) Zf = 0;
  if(accumulate) rd += rn;
  if(long_mul) {
    r[irn] = rd; rd >>= 32;
    
    if(setf) Zf |= r[irn];
    if(accumulate) rd += r[ird];
    if(signed_mul) {
      if(rm & 1<<31) rd -= rs;
      if(rs & 1<<31) rd -= rm;
    }
  }
  r[ird] = rd;
  if(setf) Nf = r[ird], Zf |= r[ird];
  if(ird == 15) branch(0, r[15]);
}

void ARMCore::armData(uint5 opcode, uint4 ird, uint4 irn, SOut rm) {
  auto rn = r[irn];
  r[15] += 4;
  
  alu(opcode, r[ird], rn, rm);
  
  if(ird == 15 && (opcode & 1)) writeCpsr(spsr(), 0xf);
  if(ird == 15) branch(Tf, r[15]);  // use Tf here because MOVS/SUBS changes it
}

void ARMCore::armDataRs(uint5 opcode, uint4 ird, uint4 irn, uint4 irm, uint2 sh, uint4 irs) {
  auto rs = r[irs];
  r[15] += 4;
  
  if(sh == 0) alu(opcode, r[ird], r[irn], lsl(r[irm], rs));
  if(sh == 1) alu(opcode, r[ird], r[irn], lsr(r[irm], rs));
  if(sh == 2) alu(opcode, r[ird], r[irn], asr(r[irm], rs));
  if(sh == 3) alu(opcode, r[ird], r[irn], ror(r[irm], rs));
  
  if(ird == 15 && (opcode & 1)) writeCpsr(spsr(), 0xf);
  if(ird == 15) branch(Tf, r[15]);
}

void ARMCore::armMemSwap(uint1 opcode, uint4 ird, uint4 irn, uint4 irm) {
  auto rn = r[irn];
  r[15] += 4;
  
  uint32 rd = load(rn, opcode? Byte : Word);
  store(rn, opcode? Byte : Word, r[irm]);
  r[ird] = rd;
  if(ird == 15) branch(r[15] & 1, r[15]);
}

void ARMCore::armMem(uint5 opcode, uint4 ird, uint4 irn, uint32 rm) {
  auto &rd = r[ird], &rn = r[irn];
  uint32 update = opcode & 0x08? rn+rm : rn-rm;
  uint32 addr   = opcode & 0x10? update : rn;
  r[15] += 4;
  
  if((opcode & 2) || !(opcode & 0x10)) rn = update;
  
  if(opcode & 1) rd = load(addr, opcode & 4? Byte : Word);  // ldr, ldrb
  else           store(addr, opcode & 4? Byte : Word, rd);  // str, strb
  
  if(ird == 15) branch(r[15] & 1, r[15]);
}

void ARMCore::armMem_v4(uint5 opcode, uint2 sh, uint4 ird, uint4 irn, uint32 rm) {
  auto &rd = r[ird], &rn = r[irn];
  uint32 update = opcode & 0x08? rn+rm : rn-rm;
  uint32 addr   = opcode & 0x10? update : rn;
  r[15] += 4;
  
  if((opcode & 2) || !(opcode & 0x10)) rn = update;
  
  if(opcode & 1) {
    if(sh == 1) rd =         load(addr, Half);  // ldrh
    if(sh == 3) rd = (int16) load(addr, Half);  // ldrsh
    if(sh == 2) rd =  (int8) load(addr, Byte);  // ldrsb
    if(ird == 15) branch(r[15] & 1, r[15]);
  } else {
    if(sh == 1)   store(addr, Half, rd);  // strh
  }
}

void ARMCore::armMem_v5(uint5 opcode, uint2 sh, uint4 ird, uint4 irn, uint32 rm) {
  auto &rd = r[ird], &rn = r[irn];
  uint32 update = opcode & 0x08? rn+rm : rn-rm;
  uint32 addr   = opcode & 0x10? update : rn;
  r[15] += 4;
  
  if((opcode & 2) || !(opcode & 0x10)) rn = update;
  
  if(~opcode & 1) {
    if(sh == 3) { store(addr, Word, r[ird&~1]); store(addr+4, Word, r[ird|1]); } // strd
    if(sh == 2) { r[ird&~1] = load(addr, Word); r[ird|1] = load(addr+4, Word);   // ldrd
                  if(ird >= 14) branch(r[15] & 1, r[15]); }
  }
}

void ARMCore::armBlock(uint5 opcode, uint4 irn, uint16 rlist) {
  unsigned index = opcode & 0x18,  up = opcode & 0x08;
  bool writeback = opcode & 0x02,  ld = opcode & 0x01;
  bool user      = (opcode & 4) && !(ld && (rlist & 1<<15));
  auto &rn = r[irn];
  uint32 addr = rn, base = rn, size = 4*bit::count(rlist);
  r[15] += 4;
  
  if(index == 0x00) addr += 4 - size;  // da
  if(index == 0x10) addr += 0 - size;  // db
  if(index == 0x18) addr += 4;         // ib
  
  if(user) swapBank(mode);
  
  for(unsigned b = 0, s = 0; b < 16; b++) {
    if(~rlist & 1<<b) continue;
    
    if(ld==0) { write(addr, Word, s++, r[b]); }
    if(ld==1) { r[b] = read(addr, Word, s++); }
    
    if(writeback) {
      writeback = false;
      rn = up? base + size : base - size;
    }
    addr += 4;
  }
  if(user) swapBank(mode);
  if(ld && (rlist & 1<<15)) branch(r[15] & 1, r[15]);
}
