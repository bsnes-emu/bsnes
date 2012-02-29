#ifdef ARMDSP_CPP

bool ArmDSP::condition() {
  switch((uint4)(instruction >> 28)) {
  case  0: return cpsr.z == 1;                      //EQ (equal)
  case  1: return cpsr.z == 0;                      //NE (not equal)
  case  2: return cpsr.c == 1;                      //CS (carry set)
  case  3: return cpsr.c == 0;                      //CC (carry clear)
  case  4: return cpsr.n == 1;                      //MI (negative)
  case  5: return cpsr.n == 0;                      //PL (positive)
  case  6: return cpsr.v == 1;                      //VS (overflow)
  case  7: return cpsr.v == 0;                      //VC (no overflow)
  case  8: return cpsr.c == 1 && cpsr.z == 0;       //HI (unsigned higher)
  case  9: return cpsr.c == 0 || cpsr.z == 1;       //LS (unsigned lower or same)
  case 10: return cpsr.n == cpsr.v;                 //GE (signed greater than or equal)
  case 11: return cpsr.n != cpsr.v;                 //LT (signed less than)
  case 12: return cpsr.z == 0 && cpsr.n == cpsr.v;  //GT (signed greater than)
  case 13: return cpsr.z == 1 || cpsr.n != cpsr.v;  //LE (signed less than or equal)
  case 14: return true;                             //AL (always)
  case 15: return false;                            //NV (never)
  }
}

//rd = target
//rn = source
//rm = modifier
//ri = original target
//ro = modified target
void ArmDSP::opcode(uint32 rm) {
  uint4 opcode = instruction >> 21;
  uint1 s = instruction >> 20;
  auto &rn = r[(uint4)(instruction >> 16)];
  auto &rd = r[(uint4)(instruction >> 12)];
  uint32 ri = rd, ro;

  //comparison opcodes always update flags
  if(opcode >= 8 && opcode <= 11) assert(s == 1);

  static auto bit = [&](uint32 ro) {
    if(!s) return;
    cpsr.n = ro & 0x80000000;
    cpsr.z = ro == 0;
  };

  static auto add = [&](uint32 ro) {
    if(!s) return;
    cpsr.n = ro & 0x80000000;
    cpsr.z = ro == 0;
    cpsr.c = ro < ri;
    cpsr.v = ~(ri ^ rm) & (ri ^ ro) & 0x80000000;
  };

  static auto sub = [&](uint32 ro) {
    if(!s) return;
    cpsr.n = ro & 0x80000000;
    cpsr.z = ro == 0;
    cpsr.c = ro > ri;
    cpsr.v =  (ri ^ rm) & (ri ^ ro) & 0x80000000;
  };

  switch(opcode) {
  case  0: rd = rn & rm;          bit(rd); break;  //AND (logical and)
  case  1: rd = rn ^ rm;          bit(rd); break;  //EOR (logical exclusive or)
  case  2: rd = rn - rm;          sub(rd); break;  //SUB (subtract)
  case  3: rd = rm - rn;          sub(rd); break;  //RSB (reverse subtract)
  case  4: rd = rn + rm;          add(rd); break;  //ADD (add)
  case  5: rd = rn + rm + cpsr.c; add(rd); break;  //ADC (add with carry)
  case  6: rd = rn - rm -!cpsr.c; sub(rd); break;  //SBC (subtract with carry)
  case  7: rd = rm - rn -!cpsr.c; sub(rd); break;  //RSC (reverse subtract with carry)
  case  8: ro = rn & rm;          bit(ro); break;  //TST (test)
  case  9: ro = rn ^ rm;          bit(ro); break;  //TEQ (test equivalence)
  case 10: ro = rn - rm;          sub(ro); break;  //CMP (compare)
  case 11: ro = rn + rm;          add(ro); break;  //CMN (compare negated)
  case 12: rd = rn | rm;          bit(rd); break;  //ORR (logical inclusive or)
  case 13: rd = rm;               bit(rd); break;  //MOV (move)
  case 14: rd = rn &~rm;          bit(rd); break;  //BIC (bit clear)
  case 15: rd =~rm;               bit(rd); break;  //MVN (move not)
  }
}

//(mul,mla){condition}{s}
//cccc 0000 00as dddd nnnn ssss 1001 mmmm
//c = condition
//a = accumulate
//s = save flags
//d = rd
//n = rn
//s = rs
//n = rm
void ArmDSP::op_multiply() {
  if(!condition()) return;

  uint1 accumulate = instruction >> 21;
  uint1 save = instruction >> 20;
  uint4 d = instruction >> 16;
  uint4 n = instruction >> 12;
  uint4 s = instruction >> 8;
  uint4 m = instruction >> 0;

  r[d] = r[m] * r[s];
  if(accumulate) r[d] += r[n];
  if(save) {
    cpsr.n = r[d] & 0x80000000;
    cpsr.z = r[d] == 0;
  }
}

//mrs{condition} rd,(c,s)psr
//cccc 0001 0r00 ++++ dddd ---- 0000 ----
//c = condition
//r = SPSR (0 = CPSR)
//d = rd
void ArmDSP::op_move_to_register_from_status_register() {
  if(!condition()) return;

  uint1 source = instruction >> 22;
  uint4 d = instruction >> 12;

  r[d] = source ? spsr : cpsr;
}

//msr{condition} (c,s)psr:{fields},rm
//cccc 0001 0r10 ffff ++++ ---- 0000 mmmm
//c = condition
//r = SPSR (0 = CPSR)
//f = field mask
//m = rm
void ArmDSP::op_move_to_status_register_from_register() {
  if(!condition()) return;

  uint1 source = instruction >> 22;
  uint4 field = instruction >> 16;
  uint4 m = instruction;

  PSR &psr = source ? spsr : cpsr;
  if(field & 1) psr.setc(r[m]);
  if(field & 2) psr.setx(r[m]);
  if(field & 4) psr.sets(r[m]);
  if(field & 8) psr.setf(r[m]);
}

//{opcode}{condition}{s} rd,rm {shift} #immediate
//{opcode}{condition} rn,rm {shift} #immediate
//{opcode}{condition}{s} rd,rn,rm {shift} #immediate
//cccc 000o ooos nnnn dddd llll lss0 mmmm
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//l = shift immmediate
//s = shift
//m = rm
void ArmDSP::op_data_immediate_shift() {
  if(!condition()) return;

  uint1 save = instruction >> 20;
  uint5 shift = instruction >> 7;
  uint2 mode = instruction >> 5;
  uint4 m = instruction;

  bool carry = cpsr.c;
  uint32 rs = shift;
  uint32 rm = r[m];

  switch(mode) {
  case 0:  //LSL
    rm = rm << rs;
    if(rs) carry = rm & (1 << (32 - rs));
    break;

  case 1:  //LSR
    if(rs == 0) rs = 32;
    rm = rm >> rs;
    carry = rm & (1 << (rs - 1));
    break;

  case 2:  //ASR
    if(rs == 0) rs = 32;
    rm = (int32)rm >> rs;
    break;

  case 3:  //ROR + RRX
    if(rs == 0) rm = (cpsr.c << 31) | (rm >> 1);      //RRX
    if(rs != 0) rm = (rm >> rs) + (rm << (32 - rs));  //ROR
    break;
  }

  if(save) cpsr.c = carry;
  opcode(rm);
}

//cccc 000o ooos nnnn dddd ssss 0ss1 mmmm
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//s = rs
//s = shift
//m = rm
void ArmDSP::op_data_register_shift() {
  if(!condition()) return;

  uint1 save = instruction >> 20;
  uint4 s = instruction >> 8;
  uint2 mode = instruction >> 5;
  uint4 m = instruction >> 0;

  bool carry = cpsr.c;
  uint32 rs = (uint8)r[s];
  uint32 rm = r[m];

  switch(mode) {
  case 0:  //LSL
    rm = rm << rs;
    if(rs) carry = rm & (1 << (32 - rs));
    break;

  case 1:  //LSR
    if(rs == 0) break;
    rm = rm >> rs;
    carry = rm & (1 << (rs - 1));
    break;

  case 2:  //ASR
    rm = (int32)rm >> rs;
    break;

  case 3:  //ROR
    rm = (rm >> rs) + (rm << (32 - rs));
    break;
  }

  if(save) cpsr.c = carry;
  opcode(rm);
}

//{opcode}{condition}{s} rd,#immediate
//{opcode}{condition} rn,#immediate
//{opcode}{condition}{s} rd,rn,#immediate
//cccc 001o ooos nnnn dddd llll iiii iiii
//c = condition
//o = opcode
//s = save flags
//n = rn
//d = rd
//l = shift immediate
//i = immediate
void ArmDSP::op_data_immediate() {
  if(!condition()) return;

  uint1 save = instruction >> 20;
  uint4 shift = instruction >> 8;
  uint8 immediate = instruction;

  bool carry = cpsr.c;
  uint32 rs = shift << 1;
  uint32 rm = (immediate >> rs) | (immediate << (32 - rs));
  if(rs) carry = immediate & 0x80000000;

  if(save) cpsr.c = carry;
  opcode(rm);
}

//(ldr,str){condition}{b} rd,[rn{+offset}]
//cccc 010p ubwl nnnn dddd iiii iiii iiii
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up (add/sub offset to base)
//b = byte (1 = 32-bit)
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//i = immediate
void ArmDSP::op_move_immediate_offset() {
  if(!condition()) return;

  uint1 p = instruction >> 24;
  uint1 u = instruction >> 23;
  uint1 b = instruction >> 22;
  uint1 w = instruction >> 21;
  uint1 l = instruction >> 20;
  uint4 n = instruction >> 16;
  uint4 d = instruction >> 12;
  uint12 rm = instruction;

  uint32 rn = r[n];
  auto &rd = r[d];

  if(l) {
    if(p == 1) rn = u ? rn + rm : rn - rm;
    if(b) rd = bus_read<1>(rn);
    else  rd = bus_read<4>(rn);
    if(p == 0) rn = u ? rn + rm : rn - rm;
  } else {
    if(p == 1) rn = u ? rn + rm : rn - rm;
    if(b) bus_write<1>(rn, rd);
    else  bus_write<4>(rn, rd);
    if(p == 0) rn = u ? rn + rm : rn - rm;
  }

  if(p == 0 || w == 1) r[n] = rn;
}

//(ldr)(str){condition}{b} rd,rn,rm {mode} #immediate
//cccc 011p ubwl nnnn dddd llll lss0 mmmm
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up
//b = byte (1 = 32-bit)
//w = writeback
//l = load (0 = save)
//n = rn
//d = rd
//l = shift immediate
//s = shift mode
//m = rm
void ArmDSP::op_move_register_offset() {
  if(!condition()) return;

  uint1 p = instruction >> 24;
  uint1 u = instruction >> 23;
  uint1 b = instruction >> 22;
  uint1 w = instruction >> 21;
  uint1 l = instruction >> 20;
  uint4 n = instruction >> 16;
  uint4 d = instruction >> 12;
  uint5 immediate = instruction >> 7;
  uint2 mode = instruction >> 5;
  uint4 m = instruction;

  uint32 rn = r[n];
  auto &rd = r[d];
  uint32 rs = immediate;
  uint32 rm = r[m];

  switch(mode) {
  case 0:  //LSL
    rm = rm << rs;
    break;

  case 1:  //LSR
    if(rs == 0) rs = 32;
    rm = rm >> rs;
    break;

  case 2:  //ASR
    if(rs == 0) rs = 32;
    rm = (int32)rm >> rs;
    break;

  case 3:  //ROR + RRX
    if(rs == 0) rm = (cpsr.c << 31) | (rm >> 1);      //RRX
    if(rs != 0) rm = (rm >> rs) + (rm << (32 - rs));  //ROR
    break;
  }

  if(l) {
    if(p == 1) rn = u ? rn + rm : rn - rm;
    if(b) rd = bus_read<1>(rn);
    else  rd = bus_read<4>(rn);
    if(p == 0) rn = u ? rn + rm : rn - rm;
  } else {
    if(p == 1) rn = u ? rn + rm : rn - rm;
    if(b) bus_write<1>(rn, rd);
    else  bus_write<4>(rn, rd);
    if(p == 0) rn = u ? rn + rm : rn - rm;
  }

  if(p == 0 || w == 1) r[n] = rn;
}

//cccc 100p uswl nnnn llll llll llll llll
//c = condition
//p = pre (0 = post-indexed addressing)
//u = up (add/sub offset to base)
//s = ???
//w = writeback
//l = load (0 = save)
//n = rn
//l = register list
void ArmDSP::op_move_multiple() {
  if(!condition()) return;

  uint1 p = instruction >> 24;
  uint1 u = instruction >> 23;
  uint1 s = instruction >> 22;
  uint1 w = instruction >> 21;
  uint1 l = instruction >> 20;
  uint4 n = instruction >> 16;
  uint16 list = instruction;

  uint32 rn = r[n];
  if(p == 0 && u == 1) rn = rn + 0;  //IA
  if(p == 1 && u == 1) rn = rn + 4;  //IB
  if(p == 1 && u == 0) rn = rn - bit::count(list) * 4 + 0;  //DB
  if(p == 0 && u == 0) rn = rn - bit::count(list) * 4 + 4;  //DA

  if(l) {
    for(unsigned n = 0; n < 16; n++) {
      if(list & (1 << n)) {
        r[n] = bus_read<4>(rn);
        rn += 4;
      }
    }
  } else {
    for(unsigned n = 0; n < 16; n++) {
      if(list & (1 << n)) {
        bus_write<4>(rn, r[n]);
        rn += 4;
      }
    }
  }

  if(w) {
    if(u == 1) r[n] = r[n] + bit::count(list) * 4;  //IA, IB
    if(u == 0) r[n] = r[n] - bit::count(list) * 4;  //DA, DB
  }
}

//b{l}{condition} address
//cccc 101l dddd dddd dddd dddd dddd dddd
//c = condition
//l = link
//d = displacement (24-bit signed)
void ArmDSP::op_branch() {
  if(!condition()) return;

  uint1 l = instruction >> 24;
  int24 displacement = instruction;

  if(l) r[14] = r[15] - 4;
  r[15] += displacement * 4;
}

#endif
