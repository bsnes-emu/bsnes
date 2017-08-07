static uint32 _pc;
static string _c;
static string _r[16] = {
  "r0", "r1",  "r2",  "r3",  "r4", "r5", "r6", "r7",
  "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"
};
#define _s save ? "s" : ""

auto ARM7TDMI::disassemble(uint32 pc) -> string {
  return "";
}

//

auto ARM7TDMI::armDisassembleBranchExchangeRegister
(uint4 m) -> string {
  return {"bx", _c, " ", _r[m]};
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

//

auto ARM7TDMI::thumbDisassembleAdjustRegister
(uint3 d, uint3 n, uint3 m, uint1 mode) -> string {
  return {!mode ? "add" : "sub", " ", _r[d], ",", _r[n], ",", _r[m]};
}
