#pragma once

//Zilog Z80

namespace Processor {

struct Z80 {
  struct Bus {
    virtual auto requested() -> bool { return _requested; }
    virtual auto granted() -> bool { return _granted; }

    virtual auto request(bool value) -> void { _requested = value; }
    virtual auto grant(bool value) -> void { _granted = value; }

    virtual auto read(uint16 addr) -> uint8 = 0;
    virtual auto write(uint16 addr, uint8 data) -> void = 0;

    virtual auto in(uint8 addr) -> uint8 = 0;
    virtual auto out(uint8 addr, uint8 data) -> void = 0;

    //serialization.cpp
    virtual auto serialize(serializer&) -> void;

  private:
    bool _requested;
    bool _granted;
  };

  virtual auto step(uint clocks) -> void = 0;
  virtual auto synchronizing() const -> bool = 0;

  //z80.cpp
  auto power() -> void;

  auto irq(bool maskable, uint16 vector = 0x0000, uint8 extbus = 0xff) -> bool;
  auto parity(uint8) const -> bool;

  //memory.cpp
  auto yield() -> void;
  auto wait(uint clocks = 1) -> void;
  auto opcode() -> uint8;
  auto operand() -> uint8;
  auto operands() -> uint16;
  auto push(uint16) -> void;
  auto pop() -> uint16;
  auto displace(uint16&) -> uint16;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto in(uint8 addr) -> uint8;
  auto out(uint8 addr, uint8 data) -> void;

  //instruction.cpp
  auto instruction() -> void;
  auto instruction(uint8 code) -> void;
  auto instructionCB(uint8 code) -> void;
  auto instructionCBd(uint16 addr, uint8 code) -> void;
  auto instructionED(uint8 code) -> void;

  //algorithms.cpp
  auto ADD(uint8, uint8, bool = false) -> uint8;
  auto AND(uint8, uint8) -> uint8;
  auto BIT(uint3, uint8) -> uint8;
  auto DEC(uint8) -> uint8;
  auto INC(uint8) -> uint8;
  auto OR (uint8, uint8) -> uint8;
  auto RES(uint3, uint8) -> uint8;
  auto RL (uint8) -> uint8;
  auto RLC(uint8) -> uint8;
  auto RR (uint8) -> uint8;
  auto RRC(uint8) -> uint8;
  auto SET(uint3, uint8) -> uint8;
  auto SLA(uint8) -> uint8;
  auto SLL(uint8) -> uint8;
  auto SRA(uint8) -> uint8;
  auto SRL(uint8) -> uint8;
  auto SUB(uint8, uint8, bool = false) -> uint8;
  auto XOR(uint8, uint8) -> uint8;

  //instructions.cpp
  auto instructionADC_a_irr(uint16&) -> void;
  auto instructionADC_a_n() -> void;
  auto instructionADC_a_r(uint8&) -> void;
  auto instructionADC_hl_rr(uint16&) -> void;
  auto instructionADD_a_irr(uint16&) -> void;
  auto instructionADD_a_n() -> void;
  auto instructionADD_a_r(uint8&) -> void;
  auto instructionADD_hl_rr(uint16&) -> void;
  auto instructionAND_a_irr(uint16&) -> void;
  auto instructionAND_a_n() -> void;
  auto instructionAND_a_r(uint8&) -> void;
  auto instructionBIT_o_irr(uint3, uint16&) -> void;
  auto instructionBIT_o_irr_r(uint3, uint16&, uint8&) -> void;
  auto instructionBIT_o_r(uint3, uint8&) -> void;
  auto instructionCALL_c_nn(bool c) -> void;
  auto instructionCALL_nn() -> void;
  auto instructionCCF() -> void;
  auto instructionCP_a_irr(uint16& x) -> void;
  auto instructionCP_a_n() -> void;
  auto instructionCP_a_r(uint8& x) -> void;
  auto instructionCPD() -> void;
  auto instructionCPDR() -> void;
  auto instructionCPI() -> void;
  auto instructionCPIR() -> void;
  auto instructionCPL() -> void;
  auto instructionDAA() -> void;
  auto instructionDEC_irr(uint16&) -> void;
  auto instructionDEC_r(uint8&) -> void;
  auto instructionDEC_rr(uint16&) -> void;
  auto instructionDI() -> void;
  auto instructionDJNZ_e() -> void;
  auto instructionEI() -> void;
  auto instructionEX_irr_rr(uint16&, uint16&) -> void;
  auto instructionEX_rr_rr(uint16&, uint16&) -> void;
  auto instructionEXX() -> void;
  auto instructionHALT() -> void;
  auto instructionIM_o(uint2) -> void;
  auto instructionIN_a_in() -> void;
  auto instructionIN_r_ic(uint8&) -> void;
  auto instructionINC_irr(uint16&) -> void;
  auto instructionINC_r(uint8&) -> void;
  auto instructionINC_rr(uint16&) -> void;
  auto instructionIND() -> void;
  auto instructionINDR() -> void;
  auto instructionINI() -> void;
  auto instructionINIR() -> void;
  auto instructionJP_c_nn(bool) -> void;
  auto instructionJP_rr(uint16&) -> void;
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
  auto instructionLD_r_r1(uint8&, uint8&) -> void;
  auto instructionLD_rr_inn(uint16&) -> void;
  auto instructionLD_rr_nn(uint16&) -> void;
  auto instructionLD_sp_rr(uint16&) -> void;
  auto instructionLDD() -> void;
  auto instructionLDDR() -> void;
  auto instructionLDI() -> void;
  auto instructionLDIR() -> void;
  auto instructionNEG() -> void;
  auto instructionNOP() -> void;
  auto instructionOR_a_irr(uint16&) -> void;
  auto instructionOR_a_n() -> void;
  auto instructionOR_a_r(uint8&) -> void;
  auto instructionOTDR() -> void;
  auto instructionOTIR() -> void;
  auto instructionOUT_ic_r(uint8&) -> void;
  auto instructionOUT_n_a() -> void;
  auto instructionOUTD() -> void;
  auto instructionOUTI() -> void;
  auto instructionPOP_rr(uint16&) -> void;
  auto instructionPUSH_rr(uint16&) -> void;
  auto instructionRES_o_irr(uint3, uint16&) -> void;
  auto instructionRES_o_irr_r(uint3, uint16&, uint8&) -> void;
  auto instructionRES_o_r(uint3, uint8&) -> void;
  auto instructionRET() -> void;
  auto instructionRET_c(bool c) -> void;
  auto instructionRETI() -> void;
  auto instructionRETN() -> void;
  auto instructionRL_irr(uint16&) -> void;
  auto instructionRL_irr_r(uint16&, uint8&) -> void;
  auto instructionRL_r(uint8&) -> void;
  auto instructionRLA() -> void;
  auto instructionRLC_irr(uint16&) -> void;
  auto instructionRLC_irr_r(uint16&, uint8&) -> void;
  auto instructionRLC_r(uint8&) -> void;
  auto instructionRLCA() -> void;
  auto instructionRLD() -> void;
  auto instructionRR_irr(uint16&) -> void;
  auto instructionRR_irr_r(uint16&, uint8&) -> void;
  auto instructionRR_r(uint8&) -> void;
  auto instructionRRA() -> void;
  auto instructionRRC_irr(uint16&) -> void;
  auto instructionRRC_irr_r(uint16&, uint8&) -> void;
  auto instructionRRC_r(uint8&) -> void;
  auto instructionRRCA() -> void;
  auto instructionRRD() -> void;
  auto instructionRST_o(uint3) -> void;
  auto instructionSBC_a_irr(uint16&) -> void;
  auto instructionSBC_a_n() -> void;
  auto instructionSBC_a_r(uint8&) -> void;
  auto instructionSBC_hl_rr(uint16&) -> void;
  auto instructionSCF() -> void;
  auto instructionSET_o_irr(uint3, uint16&) -> void;
  auto instructionSET_o_irr_r(uint3, uint16&, uint8&) -> void;
  auto instructionSET_o_r(uint3, uint8&) -> void;
  auto instructionSLA_irr(uint16&) -> void;
  auto instructionSLA_irr_r(uint16&, uint8&) -> void;
  auto instructionSLA_r(uint8&) -> void;
  auto instructionSLL_irr(uint16&) -> void;
  auto instructionSLL_irr_r(uint16&, uint8&) -> void;
  auto instructionSLL_r(uint8&) -> void;
  auto instructionSRA_irr(uint16&) -> void;
  auto instructionSRA_irr_r(uint16&, uint8&) -> void;
  auto instructionSRA_r(uint8&) -> void;
  auto instructionSRL_irr(uint16&) -> void;
  auto instructionSRL_irr_r(uint16&, uint8&) -> void;
  auto instructionSRL_r(uint8&) -> void;
  auto instructionSUB_a_irr(uint16&) -> void;
  auto instructionSUB_a_n() -> void;
  auto instructionSUB_a_r(uint8&) -> void;
  auto instructionXOR_a_irr(uint16&) -> void;
  auto instructionXOR_a_n() -> void;
  auto instructionXOR_a_r(uint8&) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;
  auto disassemble(uint16 pc, uint8 prefix, uint8 code) -> string;
  auto disassembleCB(uint16 pc, uint8 prefix, uint8 code) -> string;
  auto disassembleCBd(uint16 pc, uint8 prefix, int8 d, uint8 code) -> string;
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

    bool ei;    //EI instruction executed
    bool halt;  //HALT instruction executed
    bool iff1;  //interrupt flip-flop 1
    bool iff2;  //interrupt flip-flop 2
    uint2 im;   //interrupt mode (0-2)
  } r;

  enum class Prefix : uint { hl, ix, iy } prefix = Prefix::hl;

  Bus* bus = nullptr;
};

}
