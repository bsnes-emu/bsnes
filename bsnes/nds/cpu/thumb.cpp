
void ARMCore::thumbShiftImm(uint2 opcode, uint3 ird, uint3 irm, uint5 rs) {
  auto &rd = r[ird];
  r[15] += 2;
  
  SOut r = shiftImm(irm, opcode, rs);
  bitf(true, rd = r, r);  // lsl, lsr, asr
}

void ARMCore::thumbAddSub(uint2 opcode, uint3 ird, uint3 irn, uint3 irm) {
  auto& rd = r[ird], rn = r[irn], rm = opcode & 2? (uint32)irm : r[irm];
  r[15] += 2;
  
  if(opcode & 1) sumf(true, rd = rn-rm, rn, ~rm);  // subs
  else           sumf(true, rd = rn+rm, rn,  rm);  // adds
}

void ARMCore::thumbDataImm(uint2 opcode, uint3 ird, uint8 rm) {
  auto &rd = r[ird], rn = rd;
  r[15] += 2;
  
  if(opcode == 0) bitf(true, rd = rm, {rm, Cf});     // movs
  if(opcode == 1) sumf(true,      rn-rm, rn, ~rm);   // cmps
  if(opcode == 2) sumf(true, rd = rn+rm, rn,  rm);   // adds
  if(opcode == 3) sumf(true, rd = rn-rm, rn, ~rm);   // subs
}

void ARMCore::thumbDataLo(uint4 opcode, uint3 ird, uint3 irm) {
  auto &rd = r[ird], rm = r[irm];
  r[15] += 2;
  
       if(opcode ==  2) { SOut r = lsl(rd, rm); bitf(true, rd = r, r); }  // lsls
  else if(opcode ==  3) { SOut r = lsr(rd, rm); bitf(true, rd = r, r); }  // lsrs
  else if(opcode ==  4) { SOut r = asr(rd, rm); bitf(true, rd = r, r); }  // asrs
  else if(opcode ==  7) { SOut r = ror(rd, rm); bitf(true, rd = r, r); }  // rors
  else if(opcode ==  9) sumf(true, rd = -rm, 0, ~rm);                     // negs
  else if(opcode == 13) bitf(true, rd = rm * rd, {rm*rd, Cf});            // muls
  else alu(2*opcode+1, rd, rd, {rm,Cf});                // others are same as ARM
}

void ARMCore::thumbDataHi(uint2 opcode, uint4 ird, uint4 irm) {
  auto &rd = r[ird], rn = rd, rm = r[irm];
  r[15] += 2;
  
  if(opcode == 0) rd = rn + rm;                // add
  if(opcode == 1) sumf(true, rn-rm, rn, ~rm);  // cmps
  if(opcode == 2) rd = rm;                     // mov
  if(opcode == 3) {                            // bx, blx
    if(ird & 8) r[14] = r[15] - 4 + 1;
    return branch(rm & 1, rm);
  }
  if(ird == 15) branch(1, r[15]);
}

void ARMCore::thumbMemImm(uint5 opcode, uint3 ird, uint3 irn, uint5 rm) {
  auto &rd = r[ird], addr = r[irn];
  r[15] += 2;
  
  if(opcode == 13) rd = load(addr + 4*rm, Word);  // ldr
  if(opcode == 15) rd = load(addr + 1*rm, Byte);  // ldrb
  if(opcode == 17) rd = load(addr + 2*rm, Half);  // ldrh
  if(opcode == 12) store(addr + 4*rm, Word, rd);  // str
  if(opcode == 14) store(addr + 1*rm, Byte, rd);  // strb
  if(opcode == 16) store(addr + 2*rm, Half, rd);  // strh
}

void ARMCore::thumbMemReg(uint3 opcode, uint3 ird, uint3 irn, uint3 irm) {
  bool ld = opcode & 1;
  auto &rd = r[ird], addr = r[irn] + r[irm];
  r[15] += 2;
  
  if(opcode == 0)         store(addr, Word, rd);  // str
  if(opcode == 1)         store(addr, Half, rd);  // strh
  if(opcode == 2)         store(addr, Byte, rd);  // strb
  if(opcode == 4) rd =         load(addr, Word);  // ldr
  if(opcode == 5) rd =         load(addr, Half);  // ldrh
  if(opcode == 6) rd =         load(addr, Byte);  // ldrb
  if(opcode == 7) rd = (int16) load(addr, Half);  // ldrsh
  if(opcode == 3) rd =  (int8) load(addr, Byte);  // ldrsb
}

void ARMCore::thumbRelative(uint5 opcode, uint3 ird, uint8 rm) {
  auto &rd = r[ird], pc = r[15] & ~3;
  r[15] += 2;
  
  if(opcode == 9) rd = load(pc    + 4*rm, Word);      // ldr [pc,#n]
  if(opcode ==18)     store(r[13] + 4*rm, Word, rd);  // str [sp,#n]
  if(opcode ==19) rd = load(r[13] + 4*rm, Word);      // ldr [sp,#n]
  if(opcode ==20) rd = pc    + 4*rm;                  // adr rd,pc,..
  if(opcode ==21) rd = r[13] + 4*rm;                  // adr rd,sp,..
}

void ARMCore::thumbAddSP(uint1 opcode, uint7 rm) {
  r[15] += 2;
  if(opcode == 0) r[13] += 4*rm;  // add sp,#n
  if(opcode == 1) r[13] -= 4*rm;  // sub sp,#n
}

void ARMCore::thumbBlock(uint4 opcode, uint3 irn, uint8 rlist) {
  auto   &rn = opcode < 8? r[13] : r[irn];
  bool   ld = opcode & 1;
  bool   up = true;
  bool   writeback = true;
  uint32 regs = rlist;
  
  if(opcode < 8) {
    up = ld;
    if(irn & 1) regs |= (1 << 14+ld);
  }
  
  uint32 addr = rn, base = rn, size = 4*bit::count(regs);
  if(!up) addr -= size;
  r[15] += 2;
  
  for(unsigned b = 0, s = 0; b < 16; b++) {
    if(~regs & 1<<b) continue;
    
    if(ld) r[b] = read(addr, Word, s++);
    else   write(addr, Word, s++, r[b]);
    
    if(writeback) {
      writeback = false;
      rn = up? base + size : base - size;
    }
    addr += 4;
  }
  if(regs & 1<<15) branch(r[15] & 1, r[15]);
}

void ARMCore::thumbCond(uint4 opcode, int8 offset) {
  if(opcode == 14) { return undefined(); }
  if(opcode == 15) { return swi(); }
  if(evalCond(opcode)) return branch(1, r[15] + 2*offset);
  r[15] += 2;
}

void ARMCore::thumbBranch(int11 offset) {
  branch(1, r[15] + 2*offset);
}

void ARMCore::thumbBh(int11 offset) {
  r[14] = r[15] + 2*(offset << 11);
  r[15] += 2;
}

void ARMCore::thumbBlx(uint1 tf, uint11 offset) {
  uint32 base = r[14];
  r[14] = r[15]-2 + 1;
  branch(tf, base + 2*offset);
}
