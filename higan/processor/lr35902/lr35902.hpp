//Sharp LR35902

#pragma once

namespace Processor {

struct LR35902 {
  virtual auto idle() -> void = 0;
  virtual auto read(uint16 address) -> uint8 = 0;
  virtual auto write(uint16 address, uint8 data) -> void = 0;
  virtual auto stop() -> bool = 0;

  //lr35902.cpp
  auto power() -> void;

  //instruction.cpp
  auto interrupt(uint16 vector) -> void;
  auto instruction() -> void;
  auto instructionCB() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  virtual auto readDebugger(uint16 address) -> uint8 { return 0; }
  auto disassemble() -> string { return ""; }

  //memory.cpp
  auto operand() -> uint8;
  auto operands() -> uint16;
  auto load(uint16 address) -> uint16;
  auto store(uint16 address, uint16 data) -> void;
  auto pop() -> uint16;
  auto push(uint16 data) -> void;

  //algorithms.cpp
  auto ADC(uint8, uint8) -> uint8;
  auto ADD(uint8, uint8) -> uint8;
  auto AND(uint8, uint8) -> uint8;
  auto CP(uint8, uint8) -> void;
  auto OR(uint8, uint8) -> uint8;
  auto SBC(uint8, uint8) -> uint8;
  auto SUB(uint8, uint8) -> uint8;
  auto XOR(uint8, uint8) -> uint8;

  //instructions.cpp
  auto instructionADC_Direct_Data(uint8&) -> void;
  auto instructionADC_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionADC_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionADD_Direct_Data(uint8&) -> void;
  auto instructionADD_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionADD_Direct_Direct(uint16&, uint16&) -> void;
  auto instructionADD_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionAND_Direct_Data(uint8&) -> void;
  auto instructionAND_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionAND_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionCCF() -> void;
  auto instructionCP_Direct_Data(uint8&) -> void;
  auto instructionCP_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionCP_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionCPL() -> void;
  auto instructionDAA() -> void;
  auto instructionDEC_Direct(uint8&) -> void;
  auto instructionDEC_Direct(uint16&) -> void;
  auto instructionDEC_Indirect(uint16&) -> void;
  auto instructionDI() -> void;
  auto instructionEI() -> void;
  auto instructionHALT() -> void;
  auto instructionINC_Direct(uint8&) -> void;
  auto instructionINC_Direct(uint16&) -> void;
  auto instructionINC_Indirect(uint16&) -> void;
  auto instructionJR_Condition_Relative(bool) -> void;
  auto instructionLD_Address_Direct(uint16&) -> void;
  auto instructionLD_Direct_Data(uint8&) -> void;
  auto instructionLD_Direct_Data(uint16&) -> void;
  auto instructionLD_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionLD_Direct_Indirect(uint8&, uint8&) -> void;
  auto instructionLD_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionLD_Direct_IndirectDecrement(uint8&, uint16&) -> void;
  auto instructionLD_Direct_IndirectIncrement(uint8&, uint16&) -> void;
  auto instructionLD_Indirect_Data(uint16&) -> void;
  auto instructionLD_Indirect_Direct(uint8&, uint8&) -> void;
  auto instructionLD_Indirect_Direct(uint16&, uint8&) -> void;
  auto instructionLD_IndirectDecrement_Direct(uint16&, uint8&) -> void;
  auto instructionLD_IndirectIncrement_Direct(uint16&, uint8&) -> void;
  auto instructionNOP() -> void;
  auto instructionOR_Direct_Data(uint8&) -> void;
  auto instructionOR_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionOR_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionRET_Condition(bool) -> void;
  auto instructionRL_Direct(uint8&) -> void;
  auto instructionRLC_Direct(uint8&) -> void;
  auto instructionRR_Direct(uint8&) -> void;
  auto instructionRRC_Direct(uint8&) -> void;
  auto instructionSBC_Direct_Data(uint8&) -> void;
  auto instructionSBC_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionSBC_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionSCF() -> void;
  auto instructionSUB_Direct_Data(uint8&) -> void;
  auto instructionSUB_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionSUB_Direct_Indirect(uint8&, uint16&) -> void;
  auto instructionSTOP() -> void;
  auto instructionXOR_Direct_Data(uint8&) -> void;
  auto instructionXOR_Direct_Direct(uint8&, uint8&) -> void;
  auto instructionXOR_Direct_Indirect(uint8&, uint16&) -> void;

  struct Registers {
    union Pair {
      Pair() : word(0) {}
      uint16 word;
      struct Byte { uint8 order_msb2(hi, lo); } byte;
    };

    Pair af;
    Pair bc;
    Pair de;
    Pair hl;
    Pair sp;
    Pair pc;

    uint1 ei;
    uint1 halt;
    uint1 stop;
    uint1 ime;
  } r;
};

}
