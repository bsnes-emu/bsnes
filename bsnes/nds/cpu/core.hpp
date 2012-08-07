
struct ARMCore {
  // Model specific implementation
  virtual void power();
  virtual void step(unsigned n) = 0;
  
  // Instruction and data access
  virtual uint32 fetch(uint32 addr, uint32 size, bool s) = 0;
  virtual uint32 read(uint32 addr, uint32 size, bool s) = 0;
  virtual void write(uint32 addr, uint32 size, bool s, uint32 data) = 0;
  
  // CDP, MCR, MRC
  virtual void dataCop(uint4 cpno, uint4 op1, uint4 ird, uint4 irn, uint4 irm, uint4 op2) = 0;
  
  // Vectors
  void reset()     { vector(0x000, SVC); Ff = 1;                               } // THUMB:
  void fiq()       { vector(0x01c, FIQ); Ff = 1; if(spsr() & 1<<5) r[14] += 4; } //   LR = insn+4
  void irq()       { vector(0x018, IRQ);         if(spsr() & 1<<5) r[14] += 4; }
  void swi()       { vector(0x008, SVC);         if(spsr() & 1<<5) r[14] += 2; } //   LR = insn+2
  void undefined() { vector(0x004, UND);         if(spsr() & 1<<5) r[14] += 2; }
  void pfabort()   { vector(0x00c, ABT);         if(spsr() & 1<<5) r[14] += 2; }
  void abort()     { vector(0x010, ABT);         if(spsr() & 1<<5) r[14] += 8; } //   LR = insn+8
  
  // PSRs and mode switching
  uint32& spsr();
  uint32 readCpsr();
  void writeCpsr(uint32 value, uint32 mask);
  void vector(uint32 offset, unsigned tomode);
  void swapBank(unsigned mode);
  
  alwaysinline bool evalCond(unsigned cond);
  
  void branch(bool tf, uint32 target);
  
  // LDR rotation, STR mirroring
  uint32 load(uint32 addr, uint32 size);
  void store(uint32 addr, uint32 size, uint32 data);
  
  // Shifts, arithmetic
  struct SOut {
    // Shifter output
    uint32 rm;
    int32  carry;
    
    SOut(uint32 rm) : rm(rm) {}
    SOut(uint32 rm, int32 carry) : rm(rm), carry(carry) {}
    operator uint32() { return rm; }
  };
  alwaysinline SOut lsl(uint32 rm, uint8 rs);
  alwaysinline SOut lsr(uint32 rm, uint8 rs);
  alwaysinline SOut asr(uint32 rm, uint8 rs);
  alwaysinline SOut ror(uint32 rm, uint8 rs);
  alwaysinline SOut rrx(uint32 rm);
  
  alwaysinline SOut shiftImm(uint4 irm, uint2 opcode, uint5 rs);
  alwaysinline void alu(unsigned opcode, uint32& rd, uint32 rn, SOut rm);
  
  // Flags
  alwaysinline uint32 oflow(uint32 rd, uint32 rn, uint32 rm);
  alwaysinline void bitf(bool s, uint32 rd, SOut rm);
  alwaysinline void sumf(bool s, uint32 rd, uint32 rn, uint32 rm);
  
  // ARM argument Rm
  alwaysinline uint32 armRm(uint4 irm)                { return r[irm]; }
  alwaysinline SOut   armImmed(uint8 value, uint4 rs) { return ror(value, 2*rs); }
  alwaysinline uint32 armOffset8(uint4 lo, uint4 hi)  { return lo | hi<<4; }
  alwaysinline uint32 armOffset12(uint12 value)       { return value; }
  
  // ARM handlers
  void armWritePsr(uint1 opcode, uint4 mask, uint32 rm);
  void armReadPsr(uint1 opcode, uint4 ird);
  void armBranch(uint1 link, uint1 exch, int26 offset);
  void armBranchEx(uint1 link, uint4 irm);
  void armClz(uint4 ird, uint4 irm);
  void armDspAdd(uint2 opcode, uint4 ird, uint4 irn, uint4 irm);
  void armDspMul(uint2 opcode, uint2 xy, uint4 ird, uint4 irn, uint4 irm, uint4 irs);
  void armMultiply(uint4 opcode, uint4 ird, uint4 irn, uint4 irm, uint4 irs);
  void armDataRs(uint5 opcode, uint4 ird, uint4 irn, uint4 irm, uint2 sh, uint4 irs);
  void armData(uint5 opcode, uint4 ird, uint4 irn, SOut rm);
  void armMemSwap(uint1 opcode, uint4 ird, uint4 irn, uint4 irm);
  void armMem(uint5 opcode, uint4 ird, uint4 irn, uint32 rm);
  void armMem_v4(uint5 opcode, uint2 sh, uint4 ird, uint4 irn, uint32 rm);
  void armMem_v5(uint5 opcode, uint2 sh, uint4 ird, uint4 irn, uint32 rm);
  void armBlock(uint5 opcode, uint4 irn, uint16 rlist);
  
  // THUMB handlers
  void thumbDataLo(uint4 opcode, uint3 ird, uint3 irm);
  void thumbDataHi(uint2 opcode, uint4 ird, uint4 irm);
  void thumbDataImm(uint2 opcode, uint3 ird, uint8 rm);
  void thumbShiftImm(uint2 opcode, uint3 ird, uint3 irm, uint5 rs);
  void thumbAddSub(uint2 opcode, uint3 ird, uint3 irn, uint3 irm);
  void thumbMemImm(uint5 opcode, uint3 ird, uint3 irn, uint5 rm);
  void thumbMemReg(uint3 opcode, uint3 ird, uint3 irn, uint3 irm);
  void thumbRelative(uint5 opcode, uint3 ird, uint8 rm);
  void thumbAddSP(uint1 opcode, uint7 rm);
  void thumbBlock(uint4 opcode, uint3 irn, uint8 rlist);
  void thumbCond(uint4 opcode, int8 offset);
  void thumbBranch(int11 offset);
  void thumbBh(int11 offset);
  void thumbBlx(uint1 link, uint11 offset);
  
  // CPSR
  //   NZCVQ are stored in a lazy format:
  //   - Z is only true when 0.
  //   - NCVQ are true when < 0 (bit 31 set).
  int32  Nf, Zf, Cf, Vf, Qf;
  uint8  If, Ff, Tf, mode;
  
  // Output from shifter
  int32  carryout;        // same format as Cf
  
  // Pipeline data
  bool   branched;
  uint32 ifetch, idecode, iexecute;
  
  // Configuration
  uint32 vectorBase;      // Address of vector table
  bool   bxWithLoadedPC;  // Does LDR/LDM PC behave as MOV or BX Rm?
  
  // Register banks
  // - r_fiq[n] etc. hold registers in other banks.
  // - r[n] holds registers for the active mode
  // - This means that in FIQ mode, r_fiq[n] holds shadowed _user_ registers.
  // - SPSR is not swapped, use spsr() to access it.
  uint32 r[16],   spsr_none,  r_abt[2], spsr_abt;
  uint32 r_fiq[7], spsr_fiq,  r_irq[2], spsr_irq;
  uint32 r_svc[2], spsr_svc,  r_und[2], spsr_und;
  
  // Exception modes
  enum {
    USR = 0x10,
    FIQ = 0x11, IRQ = 0x12, SVC = 0x13,
    UND = 0x1b, ABT = 0x17, SYS = 0x1f,
  };
};
