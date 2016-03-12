#include <processor/processor.hpp>
#include "v30mz.hpp"

namespace Processor {

#include "registers.cpp"
#include "modrm.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instructions-adjust.cpp"
#include "instructions-alu.cpp"
#include "instructions-exec.cpp"
#include "instructions-flag.cpp"
#include "instructions-group.cpp"
#include "instructions-misc.cpp"
#include "instructions-move.cpp"
#include "instructions-string.cpp"
#include "disassembler.cpp"

auto V30MZ::debug(string text) -> void {
  print(text, "\n");
}

auto V30MZ::power() -> void {
  state.halt   = false;
  state.poll   = true;
  state.prefix = false;
  prefixes.reset();

  r.ax = 0x0000;
  r.cx = 0x0000;
  r.dx = 0x0000;
  r.bx = 0x0000;
  r.sp = 0x0000;
  r.bp = 0x0000;
  r.si = 0x0000;
  r.di = 0x0000;
  r.es = 0x0000;
  r.cs = 0xffff;
  r.ss = 0x0000;
  r.ds = 0x0000;
  r.ip = 0x0000;
  r.f  = 0x8000;
}

auto V30MZ::exec() -> void {
  state.poll = true;
  state.prefix = false;
  if(state.halt) return wait(1);

  instruction();
  if(!state.prefix) prefixes.reset();
}

auto V30MZ::interrupt(uint8 vector) -> void {
  state.halt = false;
  state.poll = true;
  state.prefix = false;

  //if an IRQ fires during a rep string instruction;
  //flush prefix queue and seek back to first prefix.
  //this allows the transfer to resume after the IRQ.
  if(prefixes) {
    r.ip -= prefixes.size();
    prefixes.reset();
  }

  auto ip = read(Word, 0x0000, vector * 4 + 0);
  auto cs = read(Word, 0x0000, vector * 4 + 2);

  push(r.f);
  push(r.cs);
  push(r.ip);

  r.f.m = true;
  r.f.i = false;
  r.f.b = false;

  r.ip = ip;
  r.cs = cs;
}

auto V30MZ::instruction() -> void {
  opcode = fetch();
  wait(1);

  switch(opcode) {
  case 0x00: return opAddMemReg(Byte);
  case 0x01: return opAddMemReg(Word);
  case 0x02: return opAddRegMem(Byte);
  case 0x03: return opAddRegMem(Word);
  case 0x04: return opAddAccImm(Byte);
  case 0x05: return opAddAccImm(Word);
  case 0x06: return opPushReg(r.es);
  case 0x07: return opPopReg(r.es);
  case 0x08: return opOrMemReg(Byte);
  case 0x09: return opOrMemReg(Word);
  case 0x0a: return opOrRegMem(Byte);
  case 0x0b: return opOrRegMem(Word);
  case 0x0c: return opOrAccImm(Byte);
  case 0x0d: return opOrAccImm(Word);
  case 0x0e: return opPushReg(r.cs);
  case 0x0f: return;  //pop cs
  case 0x10: return opAdcMemReg(Byte);
  case 0x11: return opAdcMemReg(Word);
  case 0x12: return opAdcRegMem(Byte);
  case 0x13: return opAdcRegMem(Word);
  case 0x14: return opAdcAccImm(Byte);
  case 0x15: return opAdcAccImm(Word);
  case 0x16: return opPushReg(r.ss);
  case 0x17: return opPopReg(r.ss);
  case 0x18: return opSbbMemReg(Byte);
  case 0x19: return opSbbMemReg(Word);
  case 0x1a: return opSbbRegMem(Byte);
  case 0x1b: return opSbbRegMem(Word);
  case 0x1c: return opSbbAccImm(Byte);
  case 0x1d: return opSbbAccImm(Word);
  case 0x1e: return opPushReg(r.ds);
  case 0x1f: return opPopReg(r.ds);
  case 0x20: return opAndMemReg(Byte);
  case 0x21: return opAndMemReg(Word);
  case 0x22: return opAndRegMem(Byte);
  case 0x23: return opAndRegMem(Word);
  case 0x24: return opAndAccImm(Byte);
  case 0x25: return opAndAccImm(Word);
  case 0x26: return opSegment(r.es);
  case 0x27: return opDecimalAdjust(0);  //daa
  case 0x28: return opSubMemReg(Byte);
  case 0x29: return opSubMemReg(Word);
  case 0x2a: return opSubRegMem(Byte);
  case 0x2b: return opSubRegMem(Word);
  case 0x2c: return opSubAccImm(Byte);
  case 0x2d: return opSubAccImm(Word);
  case 0x2e: return opSegment(r.cs);
  case 0x2f: return opDecimalAdjust(1);  //das
  case 0x30: return opXorMemReg(Byte);
  case 0x31: return opXorMemReg(Word);
  case 0x32: return opXorRegMem(Byte);
  case 0x33: return opXorRegMem(Word);
  case 0x34: return opXorAccImm(Byte);
  case 0x35: return opXorAccImm(Word);
  case 0x36: return opSegment(r.ss);
  case 0x37: return opAsciiAdjust(0);  //aaa
  case 0x38: return opCmpMemReg(Byte);
  case 0x39: return opCmpMemReg(Word);
  case 0x3a: return opCmpRegMem(Byte);
  case 0x3b: return opCmpRegMem(Word);
  case 0x3c: return opCmpAccImm(Byte);
  case 0x3d: return opCmpAccImm(Word);
  case 0x3e: return opSegment(r.ds);
  case 0x3f: return opAsciiAdjust(1);  //aas
  case 0x40: return opIncReg(r.ax);
  case 0x41: return opIncReg(r.cx);
  case 0x42: return opIncReg(r.dx);
  case 0x43: return opIncReg(r.bx);
  case 0x44: return opIncReg(r.sp);
  case 0x45: return opIncReg(r.bp);
  case 0x46: return opIncReg(r.si);
  case 0x47: return opIncReg(r.di);
  case 0x48: return opDecReg(r.ax);
  case 0x49: return opDecReg(r.cx);
  case 0x4a: return opDecReg(r.dx);
  case 0x4b: return opDecReg(r.bx);
  case 0x4c: return opDecReg(r.sp);
  case 0x4d: return opDecReg(r.bp);
  case 0x4e: return opDecReg(r.si);
  case 0x4f: return opDecReg(r.di);
  case 0x50: return opPushReg(r.ax);
  case 0x51: return opPushReg(r.cx);
  case 0x52: return opPushReg(r.dx);
  case 0x53: return opPushReg(r.bx);
  case 0x54: return opPushReg(r.sp);
  case 0x55: return opPushReg(r.bp);
  case 0x56: return opPushReg(r.si);
  case 0x57: return opPushReg(r.di);
  case 0x58: return opPopReg(r.ax);
  case 0x59: return opPopReg(r.cx);
  case 0x5a: return opPopReg(r.dx);
  case 0x5b: return opPopReg(r.bx);
  case 0x5c: return opPopReg(r.sp);
  case 0x5d: return opPopReg(r.bp);
  case 0x5e: return opPopReg(r.si);
  case 0x5f: return opPopReg(r.di);
  case 0x60: return opPushAll();
  case 0x61: return opPopAll();
  case 0x62: return opBound();
  case 0x63: return;
  case 0x64: return;
  case 0x65: return;
  case 0x66: return;
  case 0x67: return;
  case 0x68: return opPushImm(Word);
  case 0x69: return opMultiplySignedRegMemImm(Word);
  case 0x6a: return opPushImm(Byte);
  case 0x6b: return opMultiplySignedRegMemImm(Byte);
  case 0x6c: return opInString(Byte);
  case 0x6d: return opInString(Word);
  case 0x6e: return opOutString(Byte);
  case 0x6f: return opOutString(Word);
  case 0x70: return opJumpIf(r.f.v == 1);
  case 0x71: return opJumpIf(r.f.v == 0);
  case 0x72: return opJumpIf(r.f.c == 1);
  case 0x73: return opJumpIf(r.f.c == 0);
  case 0x74: return opJumpIf(r.f.z == 1);
  case 0x75: return opJumpIf(r.f.z == 0);
  case 0x76: return opJumpIf(r.f.z == 1 || r.f.c == 1);
  case 0x77: return opJumpIf(r.f.z != 1 && r.f.c != 1);
  case 0x78: return opJumpIf(r.f.s == 1);
  case 0x79: return opJumpIf(r.f.s == 0);
  case 0x7a: return opJumpIf(r.f.p == 1);
  case 0x7b: return opJumpIf(r.f.p == 0);
  case 0x7c: return opJumpIf(r.f.s != r.f.v && r.f.z == 0);
  case 0x7d: return opJumpIf(r.f.s == r.f.v || r.f.z == 1);
  case 0x7e: return opJumpIf(r.f.s != r.f.v || r.f.z == 1);
  case 0x7f: return opJumpIf(r.f.s == r.f.v && r.f.z == 0);
  case 0x80: return opGroup1MemImm(Byte, 0);
  case 0x81: return opGroup1MemImm(Word, 0);
  case 0x82: return opGroup1MemImm(Byte, 1);
  case 0x83: return opGroup1MemImm(Word, 1);
  case 0x84: return opTestMemReg(Byte);
  case 0x85: return opTestMemReg(Word);
  case 0x86: return opExchangeMemReg(Byte);
  case 0x87: return opExchangeMemReg(Word);
  case 0x88: return opMoveMemReg(Byte);
  case 0x89: return opMoveMemReg(Word);
  case 0x8a: return opMoveRegMem(Byte);
  case 0x8b: return opMoveRegMem(Word);
  case 0x8c: return opMoveMemSeg();
  case 0x8d: return opLoadEffectiveAddressRegMem();
  case 0x8e: return opMoveSegMem();
  case 0x8f: return opPopMem();
  case 0x90: return opNop();
  case 0x91: return opExchange(r.ax, r.cx);
  case 0x92: return opExchange(r.ax, r.dx);
  case 0x93: return opExchange(r.ax, r.bx);
  case 0x94: return opExchange(r.ax, r.sp);
  case 0x95: return opExchange(r.ax, r.bp);
  case 0x96: return opExchange(r.ax, r.si);
  case 0x97: return opExchange(r.ax, r.di);
  case 0x98: return opSignExtendByte();
  case 0x99: return opSignExtendWord();
  case 0x9a: return opCallFar();
  case 0x9b: return opWait();
  case 0x9c: return opPushFlags();
  case 0x9d: return opPopFlags();
  case 0x9e: return opStoreFlagsAcc();
  case 0x9f: return opLoadAccFlags();
  case 0xa0: return opMoveAccMem(Byte);
  case 0xa1: return opMoveAccMem(Word);
  case 0xa2: return opMoveMemAcc(Byte);
  case 0xa3: return opMoveMemAcc(Word);
  case 0xa4: return opMoveString(Byte);
  case 0xa5: return opMoveString(Word);
  case 0xa6: return opCompareString(Byte);
  case 0xa7: return opCompareString(Word);
  case 0xa8: return opTestAcc(Byte);
  case 0xa9: return opTestAcc(Word);
  case 0xaa: return opStoreString(Byte);
  case 0xab: return opStoreString(Word);
  case 0xac: return opLoadString(Byte);
  case 0xad: return opLoadString(Word);
  case 0xae: return opScanString(Byte);
  case 0xaf: return opScanString(Word);
  case 0xb0: return opMoveRegImm(r.al);
  case 0xb1: return opMoveRegImm(r.cl);
  case 0xb2: return opMoveRegImm(r.dl);
  case 0xb3: return opMoveRegImm(r.bl);
  case 0xb4: return opMoveRegImm(r.ah);
  case 0xb5: return opMoveRegImm(r.ch);
  case 0xb6: return opMoveRegImm(r.dh);
  case 0xb7: return opMoveRegImm(r.bh);
  case 0xb8: return opMoveRegImm(r.ax);
  case 0xb9: return opMoveRegImm(r.cx);
  case 0xba: return opMoveRegImm(r.dx);
  case 0xbb: return opMoveRegImm(r.bx);
  case 0xbc: return opMoveRegImm(r.sp);
  case 0xbd: return opMoveRegImm(r.bp);
  case 0xbe: return opMoveRegImm(r.si);
  case 0xbf: return opMoveRegImm(r.di);
  case 0xc0: return opGroup2MemImm(Byte);
  case 0xc1: return opGroup2MemImm(Word);
  case 0xc2: return opReturnImm();
  case 0xc3: return opReturn();
  case 0xc4: return opLoadSegmentMem(r.es);
  case 0xc5: return opLoadSegmentMem(r.ds);
  case 0xc6: return opMoveMemImm(Byte);
  case 0xc7: return opMoveMemImm(Word);
  case 0xc8: return opEnter();
  case 0xc9: return opLeave();
  case 0xca: return opReturnFarImm();
  case 0xcb: return opReturnFar();
  case 0xcc: return opInt3();
  case 0xcd: return opIntImm();
  case 0xce: return opInto();
  case 0xcf: return opReturnInt();
  case 0xd0: return opGroup2MemImm(Byte, (uint8)1);
  case 0xd1: return opGroup2MemImm(Word, (uint8)1);
  case 0xd2: return opGroup2MemImm(Byte, (uint8)r.cl);
  case 0xd3: return opGroup2MemImm(Word, (uint8)r.cl);
  case 0xd4: return opAdjustAfterMultiply();
  case 0xd5: return opAdjustAfterDivide();
  case 0xd6: return;
  case 0xd7: return opTranslate();
  case 0xd8: return;  //fpo1
  case 0xd9: return;  //fpo1
  case 0xda: return;  //fpo1
  case 0xdb: return;  //fpo1
  case 0xdc: return;  //fpo1
  case 0xdd: return;  //fpo1
  case 0xde: return;  //fpo1
  case 0xdf: return;  //fpo1
  case 0xe0: return opLoopWhile(0);  //loopnz
  case 0xe1: return opLoopWhile(1);  //loopz
  case 0xe2: return opLoop();
  case 0xe3: return opJumpIf(r.cx == 0);
  case 0xe4: return opIn(Byte);
  case 0xe5: return opIn(Word);
  case 0xe6: return opOut(Byte);
  case 0xe7: return opOut(Word);
  case 0xe8: return opCallNear();
  case 0xe9: return opJumpNear();
  case 0xea: return opJumpFar();
  case 0xeb: return opJumpShort();
  case 0xec: return opInDX(Byte);
  case 0xed: return opInDX(Word);
  case 0xee: return opOutDX(Byte);
  case 0xef: return opOutDX(Word);
  case 0xf0: return opLock();
  case 0xf1: return;
  case 0xf2: return opRepeat(0);  //repnz
  case 0xf3: return opRepeat(1);  //repz
  case 0xf4: return opHalt();
  case 0xf5: return opComplementCarry();
  case 0xf6: return opGroup3MemImm(Byte);
  case 0xf7: return opGroup3MemImm(Word);
  case 0xf8: return opClearFlag(r.f.c);
  case 0xf9: return opSetFlag(r.f.c);
  case 0xfa: return opClearFlag(r.f.i);
  case 0xfb: return opSetFlag(r.f.i);
  case 0xfc: return opClearFlag(r.f.d);
  case 0xfd: return opSetFlag(r.f.d);
  case 0xfe: return opGroup4MemImm(Byte);
  case 0xff: return opGroup4MemImm(Word);
  }
}

}
