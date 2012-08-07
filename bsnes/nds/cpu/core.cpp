
void ARMCore::power() {
  for(int n = 0; n < 16; n++) r[n] = 0;
  for(int n = 0; n < 7; n++)  r_fiq[n] = 0;
  for(int n = 0; n < 2; n++)  r_irq[n] = r_svc[n] = r_abt[n] = r_und[n] = 0;
  
  spsr_none = spsr_fiq = spsr_irq = spsr_svc = spsr_abt = spsr_und = 0;
  carryout = 0;
  Nf = Cf = Vf = Qf = 0;
  Zf = -1;
  
  reset();
}



uint32& ARMCore::spsr() {
  return mode == FIQ? spsr_fiq : mode == IRQ? spsr_irq
       : mode == SVC? spsr_svc : mode == UND? spsr_und
       : mode == ABT? spsr_abt : spsr_none;
}

uint32 ARMCore::readCpsr() {
  uint32 n = Nf & 1<<31,  c = Cf & 1<<31;
  uint32 v = Vf & 1<<31,  q = Qf & 1<<31;
  uint32 z = !Zf   <<31;
  
  return n>>0 | z>>1 | c>>2 | v>>3 | q>>4 | If<<7 | Ff<<6 | Tf<<5 | mode<<0;
}

void ARMCore::writeCpsr(uint32 value, unsigned mask) {
  if(mask & 8) {
    Nf = value<<0 & 1<<31; Zf = !(value<<1 & 1<<31);
    Cf = value<<2 & 1<<31; Vf =   value<<3 & 1<<31;
    Qf = value<<4 & 1<<31;
  }
  if(mode == USR)
    return;  // cpsr_c is privileged
  
  if(mask & 1) {
    swapBank(mode);
    If = value>>7 & 1; Ff   = value>>6 & 1;
    Tf = value>>5 & 1; mode = value & 0x1f;
    
    mode |= 0x10;  // enforce 32-bit addressing
    if(mode & 0x0c)
      mode |= 0x03;  // enforce valid mode bits
    swapBank(mode);
  }
}

void ARMCore::vector(uint32 offset, unsigned tomode) {
  uint32 psr = readCpsr();
  
  swapBank(mode);
  swapBank(mode = tomode);
  
  // vector() is called between the instruction fetch and PC increment..
  // therefore in ARM mode, r15 == PC+8. THUMB requires an adjustment
  // (done in the caller).
  r[14] = r[15] - 4;
  r[15] = vectorBase + offset;
  If    = 1;
  Tf    = 0;
  spsr() = psr;
  branched = true;
}

void ARMCore::swapBank(unsigned mode) {
  uint32 *bank = nullptr, *user = &r[15];
  unsigned count = 0;
  
  if(mode == FIQ) bank = &r_fiq[count = 7];
  if(mode == IRQ) bank = &r_irq[count = 2];
  if(mode == SVC) bank = &r_svc[count = 2];
  if(mode == ABT) bank = &r_abt[count = 2];
  if(mode == UND) bank = &r_und[count = 2];
  
  while(count--) std::swap(*--bank, *--user);
}



bool ARMCore::evalCond(unsigned cond) {
  if(cond == 14) return true;
  if(cond ==  0) return  (Zf == 0);            // eq
  if(cond ==  1) return !(Zf == 0);            //    ne
  if(cond ==  2) return  (Cf < 0);             // hs    / cs
  if(cond ==  3) return !(Cf < 0);             //    lo /    cc
  if(cond ==  8) return  (Cf < 0 && Zf);       // hi   
  if(cond ==  9) return !(Cf < 0 && Zf);       //    ls
  if(cond == 10) return  ((Nf^Vf) >= 0);       // ge   
  if(cond == 11) return !((Nf^Vf) >= 0);       //    lt
  if(cond == 12) return  ((Nf^Vf) >= 0 && Zf); // gt   
  if(cond == 13) return !((Nf^Vf) >= 0 && Zf); //    le
  if(cond ==  4) return  (Nf < 0);             // mi   
  if(cond ==  5) return !(Nf < 0);             //    pl
  if(cond ==  6) return  (Vf < 0);             // vs   
  if(cond ==  7) return !(Vf < 0);             //    vc
  return false;
}

void ARMCore::branch(bool tf, uint32 target) {
  r[15] = target;
  Tf = tf;
  branched = true;
}



uint32 ARMCore::load(uint32 addr, uint32 size) {
  uint32 data = read(addr, size, false);
  data = ror(data, 8*(addr & 3));
  step(1);
  
  if(size == Half) data &= 0xffff;
  if(size == Byte) data &= 0xff;
  return data;
}

void ARMCore::store(uint32 addr, uint32 size, uint32 data) {
  if(size == Half) data &= 0xffff, data *= 0x00010001;
  if(size == Byte) data &= 0xff,   data *= 0x01010101;
  
  write(addr, size, false, data);
}



ARMCore::SOut ARMCore::lsl(uint32 rm, uint8 rs) {
  if(rs == 0) return {rm, Cf};
  else        return {rs>31? 0 : rm << rs,
                      rs>32? 0 : rm << rs-1};
}
ARMCore::SOut ARMCore::lsr(uint32 rm, uint8 rs) {
  if(rs == 0) return {rm, Cf};
  else        return {rs>31? 0 : rm >>    rs,
                      rs>32? 0 : rm << 32-rs};
}
ARMCore::SOut ARMCore::asr(uint32 rm, uint8 rs) {
  if(rs == 0) return {rm, Cf};
  else        return {rs>31? (int32)rm>>31 : (int32)rm >>    rs,
                      rs>32?        rm     :        rm << 32-rs};
}
ARMCore::SOut ARMCore::ror(uint32 rm, uint8 rs) {
  if(rs == 0)     return {rm, Cf};
  if(!(rs &= 31)) return {rm, rm};  // rs == multiple of 32
  else            return {rm << 32-rs | rm >> rs,  rm << 32-rs};
}
ARMCore::SOut ARMCore::rrx(uint32 rm) {
  return {(Cf & 1<<31) | rm >> 1,  rm << 31};
}

ARMCore::SOut ARMCore::shiftImm(uint4 irm, uint2 opcode, uint5 rs) {
  if(opcode == 0) return lsl(r[irm], rs);
  if(opcode == 1) return lsr(r[irm], rs? (uint8)rs : 32);
  if(opcode == 2) return asr(r[irm], rs? (uint8)rs : 32);
  if(rs != 0)     return ror(r[irm], rs);
  if(rs == 0)     return rrx(r[irm]);
}

void ARMCore::alu(unsigned opcode, uint32& rd, uint32 rn, SOut rm) {
  if(opcode == 13*2+0) return bitf(0, rd =      rm, rm);                  // mov
  if(opcode ==  2*2+0) return sumf(0, rd = rn - rm,             rn,~rm);  // sub
  if(opcode ==  4*2+0) return sumf(0, rd = rn + rm,             rn, rm);  // add
  if(opcode ==  0*2+0) return bitf(0, rd = rn & rm, rm);                  // and
  if(opcode == 12*2+0) return bitf(0, rd = rn | rm, rm);                  // orr
  if(opcode == 14*2+0) return bitf(0, rd = rn &~rm, rm);                  // bic
  if(opcode ==  1*2+0) return bitf(0, rd = rn ^ rm, rm);                  // eor
  
  if(opcode == 13*2+1) return bitf(1, rd =      rm, rm);                  // movs
  if(opcode == 10*2+1) return sumf(1,      rn - rm,             rn,~rm);  // cmps
  if(opcode == 11*2+1) return sumf(1,      rn + rm,             rn, rm);  // adds
  
  if(opcode ==  2*2+1) return sumf(1, rd = rn - rm,             rn,~rm);  // subs
  if(opcode ==  4*2+1) return sumf(1, rd = rn + rm,             rn, rm);  // adds
  if(opcode ==  0*2+1) return bitf(1, rd = rn & rm, rm);                  // ands
  if(opcode == 12*2+1) return bitf(1, rd = rn | rm, rm);                  // orrs
  if(opcode == 14*2+1) return bitf(1, rd = rn &~rm, rm);                  // bics
  if(opcode ==  1*2+1) return bitf(1, rd = rn ^ rm, rm);                  // eors
  if(opcode ==  8*2+1) return bitf(1,      rn & rm, rm);                  // tsts
  if(opcode ==  9*2+1) return bitf(1,      rn ^ rm, rm);                  // teqs

  if(opcode ==  3*2+0) return sumf(0, rd = rm - rn,            ~rn, rm);  // rsb
  if(opcode ==  5*2+0) return sumf(0, rd = rn + rm +!!(Cf>>31), rn, rm);  // adc
  if(opcode ==  6*2+0) return sumf(0, rd = rn - rm - !(Cf>>31), rn,~rm);  // sbc
  if(opcode ==  7*2+0) return sumf(0, rd = rm - rn - !(Cf>>31),~rn, rm);  // rsc
  if(opcode == 15*2+0) return bitf(0, rd =     ~rm, rm);                  // mvn
  
  if(opcode ==  3*2+1) return sumf(1, rd = rm - rn,            ~rn, rm);  // rsbs
  if(opcode ==  5*2+1) return sumf(1, rd = rn + rm +!!(Cf>>31), rn, rm);  // adcs
  if(opcode ==  6*2+1) return sumf(1, rd = rn - rm - !(Cf>>31), rn,~rm);  // sbcs
  if(opcode ==  7*2+1) return sumf(1, rd = rm - rn - !(Cf>>31),~rn, rm);  // rscs
  if(opcode == 15*2+1) return bitf(1, rd =     ~rm, rm);                  // mvns
}



uint32 ARMCore::oflow(uint32 rd, uint32 rn, uint32 rm) {
  return ~(rn^rm) & (rn^rd);
}

void ARMCore::bitf(bool s, uint32 rd, SOut rm) {
  if(s) Cf = rm.carry, Nf = Zf = rd;
}

void ARMCore::sumf(bool s, uint32 rd, uint32 rn, uint32 rm) {
  if(s) Vf = oflow(rd,rn,rm), Cf = Vf ^ rd^rn^rm, Nf = Zf = rd;
}



#include "arm.cpp"
#include "thumb.cpp"
