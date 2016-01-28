#include <processor/processor.hpp>
#include "v30mz.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
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

  uint8 opcode = readByte();
  wait(1);

  switch(opcode) {
  case 0x90: return opNoOperation();
  case 0xb0: return opMoveRegisterImmediateByte(r.al);
  case 0xb1: return opMoveRegisterImmediateByte(r.cl);
  case 0xb2: return opMoveRegisterImmediateByte(r.dl);
  case 0xb3: return opMoveRegisterImmediateByte(r.bl);
  case 0xb4: return opMoveRegisterImmediateByte(r.ah);
  case 0xb5: return opMoveRegisterImmediateByte(r.ch);
  case 0xb6: return opMoveRegisterImmediateByte(r.dh);
  case 0xb7: return opMoveRegisterImmediateByte(r.bh);
  case 0xb8: return opMoveRegisterImmediateWord(r.ax);
  case 0xb9: return opMoveRegisterImmediateWord(r.cx);
  case 0xba: return opMoveRegisterImmediateWord(r.dx);
  case 0xbb: return opMoveRegisterImmediateWord(r.bx);
  case 0xbc: return opMoveRegisterImmediateWord(r.sp);
  case 0xbd: return opMoveRegisterImmediateWord(r.bp);
  case 0xbe: return opMoveRegisterImmediateWord(r.si);
  case 0xbf: return opMoveRegisterImmediateWord(r.di);
  case 0xe4: return opInByte();
  case 0xe5: return opInWord();
  case 0xe6: return opOutByte();
  case 0xe7: return opOutWord();
  case 0xea: return opJumpFar();
  case 0xec: return opInDXByte();
  case 0xed: return opInDXWord();
  case 0xee: return opOutDXByte();
  case 0xef: return opOutDXWord();
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
