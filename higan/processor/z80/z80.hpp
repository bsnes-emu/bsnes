#pragma once

//Zilog Z80

namespace Processor {

struct Z80 {
  struct Bus {
    virtual auto read(uint16 addr) -> uint8 = 0;
    virtual auto write(uint16 addr, uint8 data) -> void = 0;
    virtual auto in(uint8 addr) -> uint8 = 0;
    virtual auto out(uint8 addr, uint8 data) -> void = 0;
  };

  virtual auto step(uint clocks) -> void = 0;

  //z80.cpp
  auto power() -> void;
  auto reset() -> void;

  auto parity(uint8) const -> bool;

  //memory.cpp
  auto wait(uint clocks = 1) -> void;
  auto opcode() -> uint8;
  auto operand() -> uint8;
  auto operands() -> uint16;
  auto displace(uint16&) -> uint16;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto in(uint8 addr) -> uint8;
  auto out(uint8 addr, uint8 data) -> void;

  //instruction.cpp
  auto trap(uint8 prefix, uint8 code) -> void;
  auto instruction() -> void;
  auto instruction__(uint8 code) -> void;
  auto instructionCB(uint8 code) -> void;
  auto instructionED(uint8 code) -> void;

  //instructions.cpp
  auto ADD(uint8, uint8, bool = false) -> uint8;
  auto AND(uint8, uint8) -> uint8;
  auto DEC(uint8) -> uint8;
  auto INC(uint8) -> uint8;
  auto OR (uint8, uint8) -> uint8;
  auto SUB(uint8, uint8, bool = false) -> uint8;
  auto XOR(uint8, uint8) -> uint8;

  auto instructionADC_a_irr(uint16&) -> void;
  auto instructionADC_a_r(uint8&) -> void;
  auto instructionADD_a_irr(uint16&) -> void;
  auto instructionADD_a_r(uint8&) -> void;
  auto instructionADD_rr_rr(uint16&, uint16&) -> void;
  auto instructionAND_a_irr(uint16&) -> void;
  auto instructionAND_a_r(uint8&) -> void;
  auto instructionCCF() -> void;
  auto instructionCP_a_irr(uint16& x) -> void;
  auto instructionCP_a_n() -> void;
  auto instructionCP_a_r(uint8& x) -> void;
  auto instructionCPL() -> void;
  auto instructionDEC_irr(uint16&) -> void;
  auto instructionDEC_r(uint8&) -> void;
  auto instructionDEC_rr(uint16&) -> void;
  auto instructionDI() -> void;
  auto instructionEI() -> void;
  auto instructionEX_rr_rr(uint16&, uint16&) -> void;
  auto instructionHALT() -> void;
  auto instructionIM_o(uint2) -> void;
  auto instructionIN_a_in() -> void;
  auto instructionINC_irr(uint16&) -> void;
  auto instructionINC_r(uint8&) -> void;
  auto instructionINC_rr(uint16&) -> void;
  auto instructionJP_c_nn(bool) -> void;
  auto instructionJR_c_e(bool) -> void;
  auto instructionLD_a_inn() -> void;
  auto instructionLD_a_irr(uint16& x) -> void;
  auto instructionLD_inn_a() -> void;
  auto instructionLD_inn_rr(uint16&) -> void;
  auto instructionLD_irr_a(uint16&) -> void;
  auto instructionLD_irr_n(uint16&) -> void;
  auto instructionLD_irr_r(uint16&, uint8&) -> void;
  auto instructionLD_r_n(uint8&) -> void;
  auto instructionLD_r_irr(uint8&, uint16&) -> void;
  auto instructionLD_r_r(uint8&, uint8&) -> void;
  auto instructionLD_rr_inn(uint16&) -> void;
  auto instructionLD_rr_nn(uint16&) -> void;
  auto instructionNOP() -> void;
  auto instructionOR_a_irr(uint16&) -> void;
  auto instructionOR_a_r(uint8&) -> void;
  auto instructionRLA() -> void;
  auto instructionRLCA() -> void;
  auto instructionRRA() -> void;
  auto instructionRRCA() -> void;
  auto instructionSBC_a_irr(uint16&) -> void;
  auto instructionSBC_a_r(uint8&) -> void;
  auto instructionSCF() -> void;
  auto instructionSUB_a_irr(uint16&) -> void;
  auto instructionSUB_a_r(uint8&) -> void;
  auto instructionXOR_a_irr(uint16&) -> void;
  auto instructionXOR_a_r(uint8&) -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;
  auto disassemble__(uint16 pc, uint8 prefix, uint8 code) -> string;
  auto disassembleCB(uint16 pc, uint8 prefix, uint8 code) -> string;
  auto disassembleED(uint16 pc, uint8 prefix, uint8 code) -> string;

  struct Registers {
    union Pair {
      Pair() : word(0) {}
      uint16 word;
      struct Byte { uint8 order_msb2(hi, lo); } byte;
    };

    Pair af, af_;
    Pair bc, bc_;
    Pair de, de_;
    Pair hl, hl_;
    Pair ix;
    Pair iy;
    Pair ir;
    uint16 sp;
    uint16 pc;

    boolean halt;  //HALT instruction executed
    boolean iff1;  //interrupt flip-flop 1
    boolean iff2;  //interrupt flip-flop 2
    uint2 im;      //interrupt mode (0-2)

    Pair* hlp = nullptr;
  } r;

  Bus* bus = nullptr;

private:
  uint64 instructionsExecuted = 0;
};

}
