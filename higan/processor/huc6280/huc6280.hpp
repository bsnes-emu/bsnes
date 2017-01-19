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
  inline auto load8(uint8) -> uint8;
  inline auto load16(uint16) -> uint8;
  inline auto store8(uint8, uint8) -> void;
  inline auto store16(uint16, uint8) -> void;
  inline auto store21(uint21, uint8) -> void;

  auto io() -> uint8;
  auto opcode() -> uint8;
  auto operand() -> uint8;

  auto push(uint8) -> void;
  auto pull() -> uint8;

  //instruction.cpp
  auto interrupt(uint16 vector) -> void;
  auto instruction() -> void;

  //instructions.cpp
  using fp = auto (HuC6280::*)(uint8) -> uint8;
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
  auto TRB(uint8) -> uint8;
  auto TSB(uint8) -> uint8;

  using bp = auto (HuC6280::*)(uint16&, uint16&, bool) -> void;
  auto TAI(uint16&, uint16&, bool) -> void;
  auto TDD(uint16&, uint16&, bool) -> void;
  auto TIA(uint16&, uint16&, bool) -> void;
  auto TII(uint16&, uint16&, bool) -> void;
  auto TIN(uint16&, uint16&, bool) -> void;

  auto instruction_absoluteLoad(fp, uint8&, uint8 = 0) -> void;
  auto instruction_absoluteModify(fp, uint8 = 0) -> void;
  auto instruction_absoluteStore(uint8, uint8 = 0) -> void;
  auto instruction_blockmove(bp) -> void;
  auto instruction_branch(bool) -> void;
  auto instruction_clear(uint8&) -> void;
  auto instruction_clear(bool&) -> void;
  auto instruction_immediate(fp, uint8&) -> void;
  auto instruction_implied(fp, uint8&) -> void;
  auto instruction_indirectLoad(fp, uint8&, uint8 = 0) -> void;
  auto instruction_indirectStore(uint8, uint8 = 0) -> void;
  auto instruction_indirectYLoad(fp, uint8&) -> void;
  auto instruction_indirectYStore(uint8) -> void;
  auto instruction_memory(fp) -> void;
  auto instruction_pull(uint8&) -> void;
  auto instruction_pullP() -> void;
  auto instruction_push(uint8) -> void;
  auto instruction_set(bool&) -> void;
  auto instruction_swap(uint8&, uint8&) -> void;
  auto instruction_transfer(uint8&, uint8&) -> void;
  auto instruction_zeropageLoad(fp, uint8&, uint8 = 0) -> void;
  auto instruction_zeropageModify(fp, uint8 = 0) -> void;
  auto instruction_zeropageStore(uint8, uint8 = 0) -> void;

  auto instruction_BBR(uint3) -> void;
  auto instruction_BBS(uint3) -> void;
  auto instruction_BRK() -> void;
  auto instruction_BSR() -> void;
  auto instruction_CSL() -> void;
  auto instruction_CSH() -> void;
  auto instruction_JMP_absolute() -> void;
  auto instruction_JMP_indirect(uint8 = 0) -> void;
  auto instruction_JSR() -> void;
  auto instruction_NOP() -> void;
  auto instruction_PHP() -> void;
  auto instruction_RMB(uint3) -> void;
  auto instruction_RTI() -> void;
  auto instruction_RTS() -> void;
  auto instruction_SMB(uint3) -> void;
  auto instruction_ST(uint2) -> void;
  auto instruction_TAM() -> void;
  auto instruction_TMA() -> void;
  auto instruction_TST_absolute(uint8 = 0) -> void;
  auto instruction_TST_zeropage(uint8 = 0) -> void;
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
