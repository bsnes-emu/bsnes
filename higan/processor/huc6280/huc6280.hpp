//Hudson Soft HuC6280

#pragma once

namespace Processor {

struct HuC6280 {
  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(uint21 addr) -> uint8 = 0;
  virtual auto write(uint21 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;

  auto power() -> void;

  //memory.cpp
  inline auto mmu(uint16) const -> uint21;
  inline auto load(uint16) -> uint8;
  inline auto store(uint16, uint8) -> void;

  auto io() -> uint8;
  auto opcode() -> uint8;
  auto operand() -> uint8;

  auto push(uint8) -> void;
  auto pull() -> uint8;

  //instruction.cpp
  auto instruction() -> void;

  //instructions.cpp
  auto ADC(uint8) -> uint8;
  auto AND(uint8) -> uint8;
  auto ASL(uint8) -> uint8;
  auto BIT(uint8) -> uint8;
  auto CMP(uint8) -> uint8;
  auto CPX(uint8) -> uint8;
  auto CPY(uint8) -> uint8;
  auto DEC(uint8) -> uint8;
  auto EOR(uint8) -> uint8;
  auto INC(uint8) -> uint8;
  auto LD (uint8) -> uint8;
  auto LSR(uint8) -> uint8;
  auto ORA(uint8) -> uint8;
  auto ROL(uint8) -> uint8;
  auto ROR(uint8) -> uint8;
  auto SBC(uint8) -> uint8;

  using fp = auto (HuC6280::*)(uint8) -> uint8;
  auto instruction_alu_absolute(fp, uint8 = 0) -> void;
  auto instruction_alu_immediate(fp) -> void;
  auto instruction_alu_implied(fp, uint8&) -> void;
  auto instruction_alu_indirect(fp, uint8 = 0) -> void;
  auto instruction_alu_indirectY(fp) -> void;
  auto instruction_alu_memory(fp) -> void;
  auto instruction_alu_zeropage(fp, uint8 = 0) -> void;
  auto instruction_bra(bool) -> void;
  auto instruction_CLb(uint8&) -> void;
  auto instruction_CLf(bool&) -> void;
  auto instruction_CSL() -> void;
  auto instruction_CSH() -> void;
  auto instruction_LDA_indirect(uint8 index = 0) -> void;
  auto instruction_LDA_indirectY() -> void;
  auto instruction_LDb_absolute(uint8&, uint8 = 0) -> void;
  auto instruction_LDb_immediate(uint8&) -> void;
  auto instruction_LDb_zeropage(uint8&, uint8 = 0) -> void;
  auto instruction_NOP() -> void;
  auto instruction_PHb(uint8&) -> void;
  auto instruction_PHP() -> void;
  auto instruction_PLb(uint8&) -> void;
  auto instruction_PLP() -> void;
  auto instruction_SEf(bool&) -> void;
  auto instruction_STA_indirect(uint8 index = 0) -> void;
  auto instruction_STA_indirectY() -> void;
  auto instruction_STb_absolute(uint8, uint8 = 0) -> void;
  auto instruction_STb_zeropage(uint8, uint8 = 0) -> void;
  auto instruction_Sbb(uint8&, uint8&) -> void;
  auto instruction_TAM() -> void;
  auto instruction_Tbb(uint8&, uint8&) -> void;
  auto instruction_TMA() -> void;
  auto instruction_TXS() -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;

  struct Flags {
    bool c;  //carry
    bool z;  //zero
    bool i;  //interrupt disable
    bool d;  //decimal mode
    bool b;  //break
    bool t;  //memory operation
    bool v;  //overflow
    bool n;  //negative

    inline operator uint8() const {
      return c << 0 | z << 1 | i << 2 | d << 3 | b << 4 | t << 5 | v << 6 | n << 7;
    }

    inline auto& operator=(uint8 data) {
      c = data.bit(0);
      z = data.bit(1);
      i = data.bit(2);
      d = data.bit(3);
      b = data.bit(4);
      t = data.bit(5);
      v = data.bit(6);
      n = data.bit(7);
      return *this;
    }
  };

  struct Registers {
    uint8  a;
    uint8  x;
    uint8  y;
    uint8  s;
    uint16 pc;
    uint8  mpr[8];
    uint8  mdr;
    Flags  p;
    uint8  cs;  //code speed (3 = fast, 12 = slow)
  } r;
};

}
