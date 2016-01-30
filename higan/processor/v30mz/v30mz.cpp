#include <processor/processor.hpp>
#include "v30mz.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"

auto V30MZ::exec() -> void {
  if(halt) return wait(1);

  #if 1
  print(disassemble(r.cs, r.ip), "\n");
  #endif

  execOpcode();
}

auto V30MZ::execOpcode() -> void {
  executed++;

  uint8 opcode = readbIP();
  wait(1);

  switch(opcode) {
  case 0x32: return opbXorRegisterModRM();
  case 0x33: return opwXorRegisterModRM();
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
  case 0x8a: return opbMoveRegisterModRM();
  case 0x8b: return opwMoveRegisterModRM();
  case 0x8e: return opMoveSegmentRegisterModRM();
  case 0x90: return opNoOperation();
  case 0xa8: return opTestAL();
  case 0xa9: return opTestAX();
  case 0xb0: return opbMoveRegisterImmediate(r.al);
  case 0xb1: return opbMoveRegisterImmediate(r.cl);
  case 0xb2: return opbMoveRegisterImmediate(r.dl);
  case 0xb3: return opbMoveRegisterImmediate(r.bl);
  case 0xb4: return opbMoveRegisterImmediate(r.ah);
  case 0xb5: return opbMoveRegisterImmediate(r.ch);
  case 0xb6: return opbMoveRegisterImmediate(r.dh);
  case 0xb7: return opbMoveRegisterImmediate(r.bh);
  case 0xb8: return opwMoveRegisterImmediate(r.ax);
  case 0xb9: return opwMoveRegisterImmediate(r.cx);
  case 0xba: return opwMoveRegisterImmediate(r.dx);
  case 0xbb: return opwMoveRegisterImmediate(r.bx);
  case 0xbc: return opwMoveRegisterImmediate(r.sp);
  case 0xbd: return opwMoveRegisterImmediate(r.bp);
  case 0xbe: return opwMoveRegisterImmediate(r.si);
  case 0xbf: return opwMoveRegisterImmediate(r.di);
  case 0xc3: return opReturn();
  case 0xe4: return opbIn();
  case 0xe5: return opwIn();
  case 0xe6: return opbOut();
  case 0xe7: return opwOut();
  case 0xe8: return opCallNear();
  case 0xea: return opJumpFar();
  case 0xec: return opbInDX();
  case 0xed: return opwInDX();
  case 0xee: return opbOutDX();
  case 0xef: return opwOutDX();
  case 0xf8: return opClearFlag(r.f.c);
  case 0xf9: return opSetFlag(r.f.c);
  case 0xfa: return opClearFlag(r.f.i);
  case 0xfb: return opSetFlag(r.f.i);
  case 0xfc: return opClearFlag(r.f.d);
  case 0xfd: return opSetFlag(r.f.d);
  }

  print("error: unknown opcode: ", hex(opcode, 2L), "\n");
  print("executed: ", --executed, "\n");
  halt = true;
}

auto V30MZ::power() -> void {
  halt = false;
  executed = 0;

  r.ip = 0x0000;
  r.ax = 0x0000;
  r.bx = 0x0000;
  r.cx = 0x0000;
  r.dx = 0x0000;
  r.si = 0x0000;
  r.di = 0x0000;
  r.bp = 0x0000;
  r.sp = 0x0000;
  r.cs = 0xffff;
  r.ds = 0x0000;
  r.es = 0x0000;
  r.ss = 0x0000;
  r.f  = 0x8000;
}

}
