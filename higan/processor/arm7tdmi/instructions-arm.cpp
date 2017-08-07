auto ARM7TDMI::armALU(uint4 mode, uint4 target, uint4 source, uint32 data) -> void {
  switch(mode) {
  case  0: r(target) = BIT(r(source) & data);          break;  //AND
  case  1: r(target) = BIT(r(source) ^ data);          break;  //EOR
  case  2: r(target) = SUB(r(source), data, 1);        break;  //SUB
  case  3: r(target) = SUB(data, r(source), 1);        break;  //RSB
  case  4: r(target) = ADD(r(source), data, 0);        break;  //ADD
  case  5: r(target) = ADD(r(source), data, cpsr().c); break;  //ADC
  case  6: r(target) = SUB(r(source), data, cpsr().c); break;  //SBC
  case  7: r(target) = SUB(data, r(source), cpsr().c); break;  //RSC
  case  8:             BIT(r(source) & data);          break;  //TST
  case  9:             BIT(r(source) ^ data);          break;  //TEQ
  case 10:             SUB(r(source), data, 1);        break;  //CMP
  case 11:             ADD(r(source), data, 0);        break;  //CMN
  case 12: r(target) = BIT(r(source) | data);          break;  //ORR
  case 13: r(target) = BIT(data);                      break;  //MOV
  case 14: r(target) = BIT(r(source) & ~data);         break;  //BIC
  case 15: r(target) = BIT(~data);                     break;  //MVN
  }

  if(exception() && target == 15 && opcode.bit(20)) {
    cpsr() = spsr();
  }
}

auto ARM7TDMI::armMoveToStatus(uint4 field, uint1 mode, uint32 data) -> void {
  if(mode && (cpsr().m == PSR::USR || cpsr().m == PSR::SYS)) return;
  PSR& psr = mode ? spsr() : cpsr();

  if(field.bit(0)) {
    if(mode || privileged()) {
      psr.m = 0x10 | data.bits(0,4);
      psr.t = data.bit (5);
      psr.f = data.bit (6);
      psr.i = data.bit (7);
    }
  }

  if(field.bit(3)) {
    psr.v = data.bit(28);
    psr.c = data.bit(29);
    psr.z = data.bit(30);
    psr.n = data.bit(31);
  }
}

//

auto ARM7TDMI::armInstructionBranchExchangeRegister
(uint4 m) -> void {
  uint32 address = r(m);
  cpsr().t = address.bit(0);
  r(15) = address;
}

auto ARM7TDMI::armInstructionLoadImmediate
(uint8 immediate, uint1 half, uint4 d, uint4 n, uint1 writeback, uint1 up, uint1 pre) -> void {
  uint32 rn = r(n);
  uint32 rd = r(d);

  if(pre == 1) rn = up ? rn + immediate : rn - immediate;
  rd = load((half ? Half : Byte) | Nonsequential | Signed, rn);
  if(pre == 0) rn = up ? rn + immediate : rn - immediate;

  if(pre == 0 || writeback) r(n) = rn;
  r(d) = rd;
}

auto ARM7TDMI::armInstructionLoadRegister
(uint4 m, uint1 half, uint4 d, uint4 n, uint1 writeback, uint1 up, uint1 pre) -> void {
  uint32 rn = r(n);
  uint32 rm = r(m);
  uint32 rd = r(d);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  rd = load((half ? Half : Byte) | Nonsequential | Signed, rn);
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback) r(n) = rn;
  r(d) = rd;
}

auto ARM7TDMI::armInstructionMemorySwap
(uint4 m, uint4 d, uint4 n, uint1 byte) -> void {
  uint32 word = load((byte ? Byte : Word) | Nonsequential, r(n));
  store((byte ? Byte : Word) | Nonsequential, r(n), r(m));
  r(d) = word;
}

auto ARM7TDMI::armInstructionMoveHalfImmediate
(uint8 immediate, uint4 d, uint4 n, uint1 mode, uint1 writeback, uint1 up, uint1 pre) -> void {
  uint32 rn = r(n);
  uint32 rd = r(d);

  if(pre == 1) rn = up ? rn + immediate : rn - immediate;
  if(mode == 1) rd = load(Half | Nonsequential, rn);
  if(mode == 0) store(Half | Nonsequential, rn, rd);
  if(pre == 0) rn = up ? rn + immediate : rn - immediate;

  if(pre == 0 || writeback) r(n) = rn;
  if(mode == 1) r(d) = rd;
}

auto ARM7TDMI::armInstructionMoveHalfRegister
(uint4 m, uint4 d, uint4 n, uint1 mode, uint1 writeback, uint1 up, uint1 pre) -> void {
  uint32 rn = r(n);
  uint32 rm = r(m);
  uint32 rd = r(d);

  if(pre == 1) rn = up ? rn + rm : rn - rm;
  if(mode == 1) rd = load(Half | Nonsequential, rn);
  if(mode == 0) store(Half | Nonsequential, rn, rd);
  if(pre == 0) rn = up ? rn + rm : rn - rm;

  if(pre == 0 || writeback) r(n) = rn;
  if(mode == 1) r(d) = rd;
}

auto ARM7TDMI::armInstructionMoveToRegisterFromStatus
(uint4 d, uint1 mode) -> void {
  if(mode && (cpsr().m == PSR::USR || cpsr().m == PSR::SYS)) return;
  r(d) = mode ? spsr() : cpsr();
}

auto ARM7TDMI::armInstructionMoveToStatusFromImmediate
(uint8 immediate, uint4 rotate, uint4 field, uint1 mode) -> void {
  uint32 data = immediate;
  if(rotate) data = ROR(data, rotate << 1);
  armMoveToStatus(field, mode, data);
}

auto ARM7TDMI::armInstructionMoveToStatusFromRegister
(uint4 m, uint4 field, uint1 mode) -> void {
  armMoveToStatus(field, mode, r(m));
}

auto ARM7TDMI::armInstructionMultiply
(uint4 m, uint4 s, uint4 n, uint4 d, uint1 save, uint1 accumulate) -> void {
  if(accumulate) idle();
  r(d) = MUL(accumulate ? r(n) : 0, r(m), r(s));
}

auto ARM7TDMI::armInstructionMultiplyLong
(uint4 m, uint4 s, uint4 l, uint4 h, uint1 save, uint1 accumulate, uint1 sign) -> void {
  uint64 rm = r(m);
  uint64 rs = r(s);

  idle();
  idle();
  if(accumulate) idle();

  if(sign) {
    if(rs >>  8 && rs >>  8 != 0xffffff) idle();
    if(rs >> 16 && rs >> 16 !=   0xffff) idle();
    if(rs >> 24 && rs >> 24 !=     0xff) idle();
    rm = (int32)rm;
    rs = (int32)rs;
  } else {
    if(rs >>  8) idle();
    if(rs >> 16) idle();
    if(rs >> 24) idle();
  }

  uint64 rd = rm * rs;
  if(accumulate) rd += (uint64)r(h) << 32 | (uint64)r(l) << 0;

  r(h) = rd >> 32;
  r(l) = rd >>  0;

  if(save) {
    cpsr().z = rd == 0;
    cpsr().n = rd.bit(63);
  }
}
