auto HuC6280::ADC(uint8 i) -> uint8 {
  uint9 o = A + i + C;
  C = o.bit(8);
  Z = uint8(o) == 0;
  V = ~(A ^ i) & (A ^ o) & 0x80;
  N = o.bit(7);
  return o;
}

auto HuC6280::AND(uint8 i) -> uint8 {
  uint8 o = A & i;
  Z = o == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::ASL(uint8 i) -> uint8 {
  C = i.bit(7);
  i <<= 1;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::BIT(uint8 i) -> uint8 {
  Z = i == 0;
  V = i.bit(6);
  N = i.bit(7);
  return i;
}

auto HuC6280::CMP(uint8 i) -> uint8 {
  uint9 o = A - i;
  C = o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return i;
}

auto HuC6280::CPX(uint8 i) -> uint8 {
  uint9 o = X - i;
  C = o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return i;
}

auto HuC6280::CPY(uint8 i) -> uint8 {
  uint9 o = Y - i;
  C = o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return i;
}

auto HuC6280::DEC(uint8 i) -> uint8 {
  i--;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::EOR(uint8 i) -> uint8 {
  uint8 o = A ^ i;
  Z = o == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::INC(uint8 i) -> uint8 {
  i++;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::LD(uint8 i) -> uint8 {
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::LSR(uint8 i) -> uint8 {
  C = i.bit(0);
  i >>= 1;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::ORA(uint8 i) -> uint8 {
  uint8 o = A | i;
  Z = o == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::ROL(uint8 i) -> uint8 {
  bool c = C;
  C = i.bit(7);
  i = i << 1 | c;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::ROR(uint8 i) -> uint8 {
  bool c = C;
  C = i.bit(0);
  i = c << 7 | i >> 1;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::SBC(uint8 i) -> uint8 {
  return ADC(~i);
}

//

auto HuC6280::instruction_alu_absolute(fp alu, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L A = ALU(load(absolute + index));
}

auto HuC6280::instruction_alu_immediate(fp alu) -> void {
  A = ALU(operand());
L io();
}

auto HuC6280::instruction_alu_implied(fp alu, uint8& b) -> void {
  b = ALU(b);
L io();
}

auto HuC6280::instruction_alu_indirect(fp alu, uint8 index) -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage + index);
  absolute |= load(0x2001 + zeropage + index) << 8;
L A = ALU(load(absolute));
}

auto HuC6280::instruction_alu_indirectY(fp alu) -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage);
  absolute |= load(0x2001 + zeropage) << 8;
L A = ALU(load(absolute + Y));
}

auto HuC6280::instruction_alu_memory(fp alu) -> void {
  auto a = A;
  A = load(0x2000 + X);
  instruction_alu_immediate(alu);
  store(0x2000 + X, A);
  A = a;
}

auto HuC6280::instruction_alu_zeropage(fp alu, uint8 index) -> void {
  auto zeropage = operand();
  io();
L A = ALU(load(0x2000 + zeropage + index));
}

auto HuC6280::instruction_bra(bool c) -> void {
  if(!c) {
  L operand();
  } else {
    auto displacement = operand();
    io();
  L io();
    PC = PC + (int8)displacement;
  }
}

auto HuC6280::instruction_CLb(uint8& b) -> void {
  b = 0;
L io();
}

auto HuC6280::instruction_CLf(bool& f) -> void {
  f = 0;
L io();
}

auto HuC6280::instruction_CSL() -> void {
  r.cs = 12;
L io();
}

auto HuC6280::instruction_CSH() -> void {
  r.cs = 3;
L io();
}

auto HuC6280::instruction_LDA_indirect(uint8 index) -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage + index);
  absolute |= load(0x2001 + zeropage + index) << 8;
L A = LD(load(absolute));
}

auto HuC6280::instruction_LDA_indirectY() -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage);
  absolute |= load(0x2001 + zeropage) << 8;
L A = LD(load(absolute + Y));
}

auto HuC6280::instruction_LDb_absolute(uint8& b, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L b = LD(load(absolute + index));
}

auto HuC6280::instruction_LDb_immediate(uint8& b) -> void {
  b = LD(operand());
L io();
}

auto HuC6280::instruction_LDb_zeropage(uint8& b, uint8 index) -> void {
  auto zeropage = operand();
  io();
L b = LD(load(0x2000 + zeropage + index));
}

auto HuC6280::instruction_NOP() -> void {
L io();
}

auto HuC6280::instruction_PHb(uint8& b) -> void {
  io();
L push(b);
}

auto HuC6280::instruction_PHP() -> void {
  io();
L push(P);
}

auto HuC6280::instruction_PLb(uint8& b) -> void {
  io();
  io();
L b = pull();
}

auto HuC6280::instruction_PLP() -> void {
  io();
  io();
L P = pull();
}

auto HuC6280::instruction_SEf(bool& f) -> void {
  f = 1;
L io();
}

auto HuC6280::instruction_STA_indirect(uint8 index) -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage + index);
  absolute |= load(0x2001 + zeropage + index) << 8;
L store(absolute, A);
}

auto HuC6280::instruction_STA_indirectY() -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage);
  absolute |= load(0x2001 + zeropage) << 8;
L store(absolute + Y, A);
}

auto HuC6280::instruction_STb_absolute(uint8 b, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L store(absolute + index, b);
}

auto HuC6280::instruction_STb_zeropage(uint8 b, uint8 index) -> void {
  auto zeropage = operand();
  io();
L store(0x2000 + zeropage + index, b);
}

auto HuC6280::instruction_Sbb(uint8& lhs, uint8& rhs) -> void {
  swap(lhs, rhs);
  io();
L io();
}

auto HuC6280::instruction_TAM() -> void {
  auto mask = operand();
  io();
  io();
L io();
  for(uint index : range(8)) {
    if(mask.bit(index)) r.mpr[index] = A;
  }
}

auto HuC6280::instruction_Tbb(uint8& source, uint8& target) -> void {
  target = source;
L io();
  Z = target == 0;
  N = target.bit(7);
}

auto HuC6280::instruction_TMA() -> void {
  auto mask = operand();
  io();
L io();
  for(uint index : range(8)) {
    if(mask.bit(index)) { A = r.mpr[index]; break; }
  }
}

auto HuC6280::instruction_TXS() -> void {
  S = X;
L io();
}
