#ifdef ARMDSP_CPP

bool ArmDSP::condition() {
  switch((uint4)(instruction >> 28)) { default:
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

void ArmDSP::opcode(uint32 &rd, uint32 &rn, uint32 shifter) {
  switch((uint4)(instruction >> 21)) {
  case  0: rd = rn & shifter; break;            //AND (logical and)
  case  1: rd = rn & shifter; break;            //EOR (logical exclusive or)
  case  2: rd = rn - shifter; break;            //SUB (subtract)
  case  3: rd = shifter - rn; break;            //RSB (reverse subtract)
  case  4: rd = rn + shifter; break;            //ADD (add)
  case  5: rd = rn + shifter + cpsr.c; break;   //ADC (add with carry)
  case  6: rd = rn - shifter - !cpsr.c; break;  //SBC (subtract with carry)
  case  7: rd = shifter - rn - !cpsr.c; break;  //RSC (reverse subtract with carry)
  case  8: flags(rn & shifter); break;          //TST (test)
  case  9: flags(rn ^ shifter); break;          //TEQ (test equivalence)
  case 10: flags(rn - shifter); break;          //CMP (compare)
  case 11: flags(rn + shifter); break;          //CMN (compare negated)
  case 12: rd = rn | shifter; break;            //ORR (logical inclusive or)
  case 13: rd = shifter; break;                 //MOV (move)
  case 14: rd = rn & ~shifter; break;           //BIC (bit clear)
  case 15: rd = ~shifter;                       //MVN (move not)
  }
}

void ArmDSP::flags(uint32 rd) {
  cpsr.n = rd & 0x80000000;
  cpsr.z = rd == 0;
}

//CCCC 101L DDDD DDDD DDDD DDDD DDDD DDDD
//C = condition
//L = link
//D = displacement (24-bit signed)
void ArmDSP::op_branch() {
  if(!condition()) return;

  int24 displacement = (int24)instruction;
  r[15] += displacement * 4;
}

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

  bool s = (uint1)(instruction >> 20);
  uint4 n = (uint4)(instruction >> 16);
  uint4 d = (uint4)(instruction >> 12);
  uint5 rotate = 2 * (uint4)(instruction >> 8);
  uint32 immediate = (uint8)instruction;
  immediate = (immediate >> rotate) | (immediate << (32 - rotate));

  opcode(r[d], r[n], immediate);
}

//CCCC 010P UBWL NNNN DDDD IIII IIII
//C = condition
//P = pre/post-indexed addressing
//U = add/sub offset to base
//B = byte/word access
//W = ...
//L = load/save
//N = Rn
//D = Rd
//I = immediate
void ArmDSP::op_move_immediate_offset() {
  if(!condition()) return;
}

#endif
