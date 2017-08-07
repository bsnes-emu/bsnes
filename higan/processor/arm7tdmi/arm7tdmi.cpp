#include <processor/processor.hpp>
#include "arm7tdmi.hpp"

namespace Processor {

#include "registers.cpp"
#include "memory.cpp"
#include "algorithms.cpp"
#include "instruction.cpp"
#include "instructions-arm.cpp"
#include "instructions-thumb.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

ARM7TDMI::ARM7TDMI() {
  armInitialize();
  thumbInitialize();
}

auto ARM7TDMI::power() -> void {
  processor = {};
  interrupt(PSR::SVC, 0x00);
  processor.r15.modify = [&] { pipeline.reload = true; };
  pipeline = {};
  carry = 0;
  irq = 0;
}

struct CPU : ARM7TDMI {
  auto step(uint) -> void {}
  auto sleep() -> void {}
  auto get(uint, uint32) -> uint32 {}
  auto set(uint, uint32, uint32) -> void {}

  CPU() {
  /*
    uint32 opcode = 0x00337e92;
    uint12 id = (opcode & 0x0ff00000) >> 16 | (opcode & 0x000000f0) >> 4;
    print("!!", hex(id), "\n");
    armInstruction[id](opcode);
    print(armDisassemble[id](opcode), "\n");
  */
  }
} cpu;

}
