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
  if(opcode >= 8 && opcode <= 11) s = 1;

  static auto nz = [&](uint32 ro) {
    if(!s) return;
    cpsr.n = ro & 0x80000000;
    cpsr.z = ro == 0;
  };

  static auto nzcv = [&](uint32 ro) {
    if(!s) return;
    cpsr.n = ro & 0x80000000;
    cpsr.z = ro == 0;
    cpsr.c = ro < ri;
    cpsr.v = ~(ri ^ rm) & (ri ^ ro) & 0x80000000;
  };

  switch(opcode) {
  case  0: rd = rn & rm;            nz(rd); break;  //AND (logical and)
  case  1: rd = rn ^ rm;            nz(rd); break;  //EOR (logical exclusive or)
  case  2: rd = rn +~rm + 1;      nzcv(rd); break;  //SUB (subtract)
  case  3: rd = rm +~rn + 1;      nzcv(rd); break;  //RSB (reverse subtract)
  case  4: rd = rn + rm;          nzcv(rd); break;  //ADD (add)
  case  5: rd = rn + rm + cpsr.c; nzcv(rd); break;  //ADC (add with carry)
  case  6: rd = rn +~rm + cpsr.c; nzcv(rd); break;  //SBC (subtract with carry)
  case  7: rd = rm +~rn + cpsr.c; nzcv(rd); break;  //RSC (reverse subtract with carry)
  case  8: ro = rn & rm;            nz(ro); break;  //TST (test)
  case  9: ro = rn ^ rm;            nz(ro); break;  //TEQ (test equivalence)
  case 10: ro = rn +~rm + 1;      nzcv(ro); break;  //CMP (compare)
  case 11: ro = rn + rm;          nzcv(ro); break;  //CMN (compare negated)
  case 12: rd = rn | rm;            nz(rd); break;  //ORR (logical inclusive or)
  case 13: rd = rm;                 nz(rd); break;  //MOV (move)
  case 14: rd = rn &~rm;            nz(rd); break;  //BIC (bit clear)
  case 15: rd =~rm;                 nz(rd); break;  //MVN (move not)
  }
}

//CCCC 101L DDDD DDDD DDDD DDDD DDDD DDDD
//C = condition
//L = link
//D = displacement (24-bit signed)
void ArmDSP::op_branch() {
  if(!condition()) return;

  uint1 l = instruction >> 24;
  int24 displacement = instruction;

  if(l) r[14] = r[15] - 4;
  r[15] += displacement * 4;
}

//{opcode}{condition}{s} rd,#immediate
//{opcode}{condition} rn,#immediate
//{opcode}{condition}{s} rd,rn,#immediate
//CCCC 001O OOOS NNNN DDDD RRRR IIII IIII
//C = condition
//O = opcode
//S = update flags
//N = Rn
//D = Rd
//R = rotate
//I = immediate
void ArmDSP::op_data_immediate() {
  if(!condition()) return;

  uint4 shift = instruction >> 8;
  uint8 immediate = instruction;

  uint32 rs = shift << 1;
  uint32 rm = (immediate >> rs) | (immediate << (32 - rs));

  opcode(rm);
}

//{opcode}{condition}{s} rd,rm {shift} #immediate
//{opcode}{condition} rn,rm {shift} #immediate
//{opcode}{condition}{s} rd,rn,rm {shift} #immediate
//CCCC 000O OOOS NNNN DDDD LLLL LSS0 MMMM
//C = condition
//O = opcode
//S = update flags
//N = Rn
//D = Rd
//L = shift amount
//S = shift
//M = Rm
void ArmDSP::op_data_immediate_shift() {
  if(!condition()) return;

  uint5 shift = instruction >> 7;
  uint2 mode = instruction >> 5;
  uint4 m = instruction;

  uint32 rs = shift;
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

  uint4 s = instruction >> 8;
  uint2 mode = instruction >> 5;
  uint4 m = instruction >> 0;

  uint32 rs = r[s];
  uint32 rm = r[m];

  switch(mode) {
  case 0: rm = rm << rs;                       break;  //LSL
  case 1: rm = rm >> rs;                       break;  //LSR
  case 2: rm = (int32)rm >> rs;                break;  //ASR
  case 3: rm = (rm >> rs) + (rm << (32 - rs)); break;  //ROR
  }

  opcode(rm);
}

//(ldr,str){condition}{b} rd,[rn{+offset}]
//CCCC 010P UBWL NNNN DDDD IIII IIII IIII
//C = condition
//P = pre (0 = post-indexed addressing)
//U = up (add/sub offset to base)
//B = byte (1 = 32-bit)
//W = writeback
//L = load (0 = save)
//N = Rn
//D = Rd
//I = immediate
void ArmDSP::op_move_immediate_offset() {
  if(!condition()) return;

  uint1 p = instruction >> 24;
  uint1 u = instruction >> 23;
  uint1 b = instruction >> 22;
  uint1 w = instruction >> 21;
  uint1 l = instruction >> 20;
  uint4 n = instruction >> 16;
  auto &rd = r[(uint4)(instruction >> 12)];
  uint12 immediate = instruction;

  uint32 rn = r[n];

  if(l) {
    if(p == 1) rn = u ? rn + immediate : rn - immediate;
    if(b) rd = bus_read<1>(rn);
    else  rd = bus_read<4>(rn);
    if(p == 0) rn = u ? rn + immediate : rn - immediate;
  } else {
    if(p == 1) rn = u ? rn + immediate : rn - immediate;
    if(b) bus_write<1>(rn, rd);
    else  bus_write<4>(rn, rd);
    if(p == 0) rn = u ? rn + immediate : rn - immediate;
  }

  if(p == 1 && w == 1) r[n] = rn;
}

//CCCC 0001 0R10 FFFF ++++ ---- 0000 MMMM
//C = condition
//R = SPSR (0 = CPSR)
//F = field mask
//M = Rm
void ArmDSP::op_move_status_register_to_register() {
  if(!condition()) return;

  uint1 source = instruction >> 22;
  uint4 field = instruction >> 16;

  auto &rm = r[(uint4)instruction];
  PSR &psr = source ? spsr : cpsr;

  if(field & 1) psr.setc(rm);
  if(field & 2) psr.setx(rm);
  if(field & 4) psr.sets(rm);
  if(field & 8) psr.setf(rm);
}

//CCCC 100P USWL NNNN LLLL LLLL LLLL LLLL
//C = condition
//P = pre (0 = post-indexed addressing)
//U = up (add/sub offset to base)
//S = ???
//W = writeback
//L = load (0 = save)
//N = Rn
//L = register list
void ArmDSP::op_move_multiple() {
  if(!condition()) return;

  uint1 p = instruction >> 24;
  uint1 u = instruction >> 23;
  uint1 s = instruction >> 22;
  uint1 w = instruction >> 21;
  uint1 l = instruction >> 20;
  uint4 n = instruction >> 16;
  uint16 list = instruction;

  uint32 rn;
  if(p == 0 && u == 1) rn = r[n] + 0;  //IA
  if(p == 1 && u == 1) rn = r[n] + 4;  //IB
  if(p == 1 && u == 0) rn = r[n] - bit::count(list) * 4 + 0;  //DB
  if(p == 0 && u == 0) rn = r[n] - bit::count(list) * 4 + 4;  //DA

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

//if(l)exception=true;
}

#endif
