static uint32 _pc;
static string _c;
static const string _r[] = {
  "r0", "r1",  "r2",  "r3",  "r4", "r5", "r6", "r7",
  "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"
};
#define _s save ? "s" : ""

#define isMove(mode) (mode == 13 || mode == 15)
#define isComp(mode) (mode >=  8 && mode <= 11)
#define isMath(mode) (mode <=  7 || mode == 12 || mode == 14)

auto ARM7TDMI::disassemble(uint32 pc) -> string {
  return "";
}

//

auto ARM7TDMI::armDisassembleBranch
(int24 displacement, uint1 link) -> string {
  return {"b", link ? "l" : "", _c, " 0x", hex(_pc + 8 + displacement * 4, 8L)};
}

auto ARM7TDMI::armDisassembleBranchExchangeRegister
(uint4 m) -> string {
  return {"bx", _c, " ", _r[m]};
}

auto ARM7TDMI::armDisassembleDataImmediate
(uint8 immediate, uint4 shift, uint4 d, uint4 n, uint1 save, uint4 mode) -> string {
  static const string opcode[] = {
    "and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc",
    "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn",
  };
  uint32 data = immediate >> (shift << 1) | immediate << 32 - (shift << 1);
  return {opcode[mode], _c,
    isMove(mode) ? string{_s, " ", _r[d]} : string{},
    isComp(mode) ? string{" ", _r[n]} : string{},
    isMath(mode) ? string{_s, " ", _r[d], ",", _r[n]} : string{},
    ",#0x", hex(data, 8L)};
}

auto ARM7TDMI::armDisassembleDataImmediateShift
(uint4 m, uint2 type, uint5 shift, uint4 d, uint4 n, uint1 save, uint4 mode) -> string {
  static const string opcode[] = {
    "and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc",
    "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn",
  };
  return {opcode[mode], _c,
    isMove(mode) ? string{_s, " ", _r[d]} : string{},
    isComp(mode) ? string{" ", _r[n]} : string{},
    isMath(mode) ? string{_s, " ", _r[d], ",", _r[n]} : string{},
    ",", _r[m],
    type == 0 && shift ? string{" lsl #", shift} : string{},
    type == 1 ? string{" lsr #", shift ? (uint)shift : 32} : string{},
    type == 2 ? string{" asr #", shift ? (uint)shift : 32} : string{},
    type == 3 && shift ? string{" ror #", shift} : string{},
    type == 3 && !shift ? " rrx" : ""};
}

auto ARM7TDMI::armDisassembleDataRegisterShift
(uint4 m, uint2 type, uint4 s, uint4 d, uint4 n, uint1 save, uint4 mode) -> string {
  static const string opcode[] = {
    "and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc",
    "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn",
  };
  return {opcode[mode], _c,
    isMove(mode) ? string{_s, " ", _r[d]} : string{},
    isComp(mode) ? string{" ", _r[n]} : string{},
    isMath(mode) ? string{_s, " ", _r[d], ",", _r[n]} : string{},
    ",", _r[m], " ",
    type == 0 ? "lsl" : "",
    type == 1 ? "lsr" : "",
    type == 2 ? "asr" : "",
    type == 3 ? "ror" : "",
    " ", _r[s]};
}

auto ARM7TDMI::armDisassembleLoadImmediate
(uint8 immediate, uint1 half, uint4 d, uint4 n, uint1 writeback, uint1 up, uint1 pre) -> string {
  string data;
  if(n == 15) data = {" =0x", hex(read((half ? Half : Byte) | Nonsequential,
    _pc + 8 + (up ? +immediate : -immediate)), half ? 4L : 2L)};

  return {"ldr", _c, half ? "sh" : "sb", " ",
    _r[d], ",[", _r[n],
    pre == 0 ? "]" : "",
    immediate ? string{",", up ? "+" : "-", "0x", hex(immediate, 2L)} : string{},
    pre == 1 ? "]" : "",
    pre == 0 || writeback ? "!" : "", data};
}

auto ARM7TDMI::armDisassembleLoadRegister
(uint4 m, uint1 half, uint4 d, uint4 n, uint1 writeback, uint1 up, uint1 pre) -> string {
  return {"ldr", _c, half ? "sh" : "sb", " ",
    _r[d], ",[", _r[n],
    pre == 0 ? "]" : "",
    ",", up ? "+" : "-", _r[m],
    pre == 1 ? "]" : "",
    pre == 0 || writeback ? "!" : ""};
}

auto ARM7TDMI::armDisassembleMemorySwap
(uint4 m, uint4 d, uint4 n, uint1 byte) -> string {
  return {"swp", _c, byte ? "b" : "", " ", _r[d], ",", _r[m], ",[", _r[n], "]"};
}

auto ARM7TDMI::armDisassembleMoveHalfImmediate
(uint8 immediate, uint4 d, uint4 n, uint1 mode, uint1 writeback, uint1 up, uint1 pre) -> string {
  string data;
  if(n == 15) data = {" =0x", hex(read(Half | Nonsequential, _pc + (up ? +immediate : -immediate)), 4L)};

  return {mode ? "ldr" : "str", _c, "h ",
    _r[d], ",[", _r[n],
    pre == 0 ? "]" : "",
    immediate ? string{",", up ? "+" : "-", "0x", hex(immediate, 2L)} : string{},
    pre == 1 ? "]" : "",
    pre == 0 || writeback ? "!" : "", data};
}

auto ARM7TDMI::armDisassembleMoveHalfRegister
(uint4 m, uint4 d, uint4 n, uint1 mode, uint1 writeback, uint1 up, uint1 pre) -> string {
  return {mode ? "ldr" : "str", _c, "h ",
    _r[d], ",[", _r[n],
    pre == 0 ? "]" : "",
    ",", up ? "+" : "-", _r[m],
    pre == 1 ? "]" : "",
    pre == 0 || writeback ? "!" : ""};
}

auto ARM7TDMI::armDisassembleMoveImmediateOffset
(uint12 immediate, uint4 d, uint4 n, uint1 mode, uint1 writeback, uint1 byte, uint1 up, uint1 pre) -> string {
  string data;
  if(n == 15) data = {" =0x", hex(read((byte ? Byte : Word) | Nonsequential,
    _pc + 8 + (up ? +immediate : -immediate)), byte ? 2L : 4L)};
  return {mode ? "ldr" : "str", _c, byte ? "b" : "", " ", _r[d], ",[", _r[n],
    pre == 0 ? "]" : "",
    immediate ? string{",", up ? "+" : "-", "0x", hex(immediate, 3L)} : string{},
    pre == 1 ? "]" : "",
    pre == 0 || writeback ? "!" : "", data};
}

auto ARM7TDMI::armDisassembleMoveMultiple
(uint16 list, uint4 n, uint1 mode, uint1 writeback, uint1 type, uint1 up, uint1 pre) -> string {
  string registers;
  for(auto index : range(16)) {
    if(list.bit(index)) registers.append(_r[index], ",");
  }
  registers.trimRight(",", 1L);
  return {mode ? "ldm" : "stm",
    up == 0 && pre == 0 ? "da" : "",
    up == 0 && pre == 1 ? "db" : "",
    up == 1 && pre == 0 ? "ia" : "",
    up == 1 && pre == 1 ? "ib" : "",
    " ", _r[n], writeback ? "!" : "",
    ",{", registers, "}", type ? "^" : ""};
}

auto ARM7TDMI::armDisassembleMoveRegisterOffset
(uint4 m, uint2 type, uint5 shift, uint4 d, uint4 n, uint1 mode, uint1 writeback, uint1 byte, uint1 up, uint1 pre) -> string {
  return {mode ? "ldr" : "str", _c, byte ? "b" : "", " ", _r[d], ",[", _r[n],
    pre == 0 ? "]" : "",
    ",", up ? "+" : "-", _r[m],
    type == 0 && shift ? string{" lsl #", shift} : string{},
    type == 1 ? string{" lsr #", shift ? (uint)shift : 32} : string{},
    type == 2 ? string{" asr #", shift ? (uint)shift : 32} : string{},
    type == 3 && shift ? string{" ror #", shift} : string{},
    type == 3 && !shift ? " rrx" : "",
    pre == 1 ? "]" : "",
    pre == 0 || writeback ? "!" : ""};
}

auto ARM7TDMI::armDisassembleMoveToRegisterFromStatus
(uint4 d, uint1 mode) -> string {
  return {"mrs", _c, " ", _r[d], ",", mode ? "spsr" : "cpsr"};
}

auto ARM7TDMI::armDisassembleMoveToStatusFromImmediate
(uint8 immediate, uint4 rotate, uint4 field, uint1 mode) -> string {
  uint32 data = immediate >> (rotate << 1) | immediate << 32 - (rotate << 1);
  return {"msr", _c, " ", mode ? "spsr:" : "cpsr:",
    field.bit(0) ? "c" : "",
    field.bit(1) ? "x" : "",
    field.bit(2) ? "s" : "",
    field.bit(3) ? "f" : "",
    ",#0x", hex(data, 8L)};
}

auto ARM7TDMI::armDisassembleMoveToStatusFromRegister
(uint4 m, uint4 field, uint1 mode) -> string {
  return {"msr", _c, " ", mode ? "spsr:" : "cpsr:",
    field.bit(0) ? "c" : "",
    field.bit(1) ? "x" : "",
    field.bit(2) ? "s" : "",
    field.bit(3) ? "f" : "",
    ",", _r[m]};
}

auto ARM7TDMI::armDisassembleMultiply
(uint4 m, uint4 s, uint4 n, uint4 d, uint1 save, uint1 accumulate) -> string {
  if(accumulate) {
    return {"mla", _c, _s, " ", _r[d], ",", _r[m], ",", _r[s], ",", _r[n]};
  } else {
    return {"mul", _c, _s, " ", _r[d], ",", _r[m], ",", _r[s]};
  }
}

auto ARM7TDMI::armDisassembleMultiplyLong
(uint4 m, uint4 s, uint4 l, uint4 h, uint1 save, uint1 accumulate, uint1 sign) -> string {
  return {sign ? "s" : "u", accumulate ? "mlal" : "mull", _c, _s, " ",
    _r[l], ",", _r[h], ",", _r[m], ",", _r[s]};
}

auto ARM7TDMI::armDisassembleSoftwareInterrupt
(uint24 immediate) -> string {
  return {"swi #0x", hex(immediate, 6L)};
}

//

auto ARM7TDMI::thumbDisassembleALU
(uint3 d, uint3 m, uint4 mode) -> string {
  static const string opcode[] = {
    "and", "eor", "lsl", "lsr", "asr", "adc", "sbc", "ror",
    "tst", "neg", "cmp", "cmn", "orr", "mul", "bic", "mvn",
  };
  return {opcode[mode], " ", _r[d], ",", _r[m]};
}

auto ARM7TDMI::thumbDisassembleAdjustImmediate
(uint3 d, uint3 n, uint3 immediate, uint1 mode) -> string {
  return {!mode ? "add" : "sub", " ", _r[d], ",", _r[n], ",#", immediate};
}

auto ARM7TDMI::thumbDisassembleAdjustRegister
(uint3 d, uint3 n, uint3 m, uint1 mode) -> string {
  return {!mode ? "add" : "sub", " ", _r[d], ",", _r[n], ",", _r[m]};
}

auto ARM7TDMI::thumbDisassembleBranchExchange
(uint4 m) -> string {
  return {"bx ", _r[m]};
}

auto ARM7TDMI::thumbDisassembleImmediate
(uint8 immediate, uint3 d, uint2 mode) -> string {
  static const string opcode[] = {"mov", "cmp", "add", "sub"};
  return {opcode[mode], " ", _r[d], ",#0x", hex(immediate, 2L)};
}

auto ARM7TDMI::thumbDisassembleShiftImmediate
(uint3 d, uint3 m, uint5 immediate, uint2 mode) -> string {
  static const string opcode[] = {"lsl", "lsr", "asr"};
  return {opcode[mode], " ", _r[d], ",", _r[m], ",#", immediate};
}
































