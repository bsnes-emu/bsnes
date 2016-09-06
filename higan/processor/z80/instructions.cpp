//legend:
//  a   = r.a
//  c   = condition
//  e   = relative operand
//  in  = (operand)
//  inn = (operand-word)
//  irr = (register-word)
//  o   = opcode bits
//  n   = operand
//  nn  = operand-word
//  r   = register

auto Z80::ADD(uint8 x, uint8 y, bool c) -> uint8 {
  uint9 z = x + y + c;

  r.p.c = z.bit(8);
  r.p.n = 0;
  r.p.v = ~(x ^ y) & (x ^ z) & 0x80;
  r.p.x = z.bit(3);
  r.p.h = (x ^ y ^ z) & 0x10;
  r.p.y = z.bit(5);
  r.p.z = (uint8)z == 0;
  r.p.s = z.bit(7);

  return z;
}

auto Z80::AND(uint8 x, uint8 y) -> uint8 {
  uint8 z = x & y;

  r.p.c = 0;
  r.p.n = 0;
  r.p.v = z == 0;
  r.p.x = z.bit(3);
  r.p.h = 1;
  r.p.y = z.bit(5);
  r.p.z = z == 0;
  r.p.s = z.bit(7);

  return z;
}

auto Z80::OR(uint8 x, uint8 y) -> uint8 {
  uint8 z = x | y;

  r.p.c = 0;
  r.p.n = 0;
  r.p.v = z == 0;
  r.p.x = z.bit(3);
  r.p.h = 0;
  r.p.y = z.bit(5);
  r.p.z = z == 0;
  r.p.s = z.bit(7);

  return z;
}

auto Z80::SUB(uint8 x, uint8 y, bool c) -> uint8 {
  uint9 z = x - y - c;

  r.p.c = z.bit(8);
  r.p.n = 1;
  r.p.v = (x ^ y) & (x ^ z) & 0x80;
  r.p.x = z.bit(3);
  r.p.h = (x ^ y ^ z) & 0x10;
  r.p.y = z.bit(5);
  r.p.z = (uint8)z == 0;
  r.p.s = z.bit(7);

  return z;
}

auto Z80::XOR(uint8 x, uint8 y) -> uint8 {
  uint8 z = x ^ y;

  r.p.c = 0;
  r.p.n = 0;
  r.p.p = parity(z);
  r.p.x = z.bit(3);
  r.p.h = 0;
  r.p.y = z.bit(5);
  r.p.z = z == 0;
  r.p.s = z.bit(7);

  return z;
}

//

auto Z80::instructionADC_a_irr(uint16& x) -> void {
  r.a = ADD(r.a, read(displace(x)), r.p.c);
}

auto Z80::instructionADC_a_r(uint8& x) -> void {
  r.a = ADD(r.a, x, r.p.c);
}

auto Z80::instructionADD_a_irr(uint16& x) -> void {
  r.a = ADD(r.a, read(displace(x)));
}

auto Z80::instructionADD_a_r(uint8& x) -> void {
  r.a = ADD(r.a, x);
}

auto Z80::instructionAND_a_irr(uint16& x) -> void {
  r.a = AND(r.a, read(displace(x)));
}

auto Z80::instructionAND_a_r(uint8& x) -> void {
  r.a = AND(r.a, x);
}

auto Z80::instructionCP_a_irr(uint16& x) -> void {
  SUB(r.a, read(displace(x)));
}

auto Z80::instructionCP_a_n() -> void {
  SUB(r.a, operand());
}

auto Z80::instructionCP_a_r(uint8& x) -> void {
  SUB(r.a, x);
}

auto Z80::instructionDI() -> void {
  r.iff1 = 0;
  r.iff2 = 0;
}

auto Z80::instructionEI() -> void {
  r.iff1 = 1;
  r.iff2 = 1;
}

auto Z80::instructionHALT() -> void {
  r.halt = 1;
}

auto Z80::instructionIM_o(uint2 code) -> void {
  wait(4);
  r.im = code;
}

auto Z80::instructionIN_a_in() -> void {
  r.a = in(operand());
}

auto Z80::instructionJP_c_nn(bool c) -> void {
  auto pc = operands();
  if(c) r.pc = pc;
}

auto Z80::instructionJR_c_e(bool c) -> void {
  auto e = operand();
  if(c) wait(5), r.pc += (int8)e;
}

auto Z80::instructionLD_inn_a() -> void {
  write(operands(), r.a);
}

auto Z80::instructionLD_irr_n(uint16& x) -> void {
  auto addr = displace(x);
  write(addr, operand());
}

auto Z80::instructionLD_irr_r(uint16& x, uint8& y) -> void {
  write(displace(x), y);
}

auto Z80::instructionLD_r_n(uint8& x) -> void {
  x = operand();
}

auto Z80::instructionLD_r_irr(uint8& x, uint16& y) -> void {
  x = read(displace(y));
}

auto Z80::instructionLD_r_r(uint8& x, uint8& y) -> void {
  x = y;
}

auto Z80::instructionLD_rr_nn(uint16& x) -> void {
  x = operands();
}

auto Z80::instructionNOP() -> void {
}

auto Z80::instructionOR_a_irr(uint16& x) -> void {
  r.a = OR(r.a, read(displace(x)));
}

auto Z80::instructionOR_a_r(uint8& x) -> void {
  r.a = OR(r.a, x);
}

auto Z80::instructionSBC_a_irr(uint16& x) -> void {
  r.a = SUB(r.a, read(displace(x)), r.p.c);
}

auto Z80::instructionSBC_a_r(uint8& x) -> void {
  r.a = SUB(r.a, x, r.p.c);
}

auto Z80::instructionSUB_a_irr(uint16& x) -> void {
  r.a = SUB(r.a, read(displace(x)));
}

auto Z80::instructionSUB_a_r(uint8& x) -> void {
  r.a = SUB(r.a, x);
}

auto Z80::instructionXOR_a_irr(uint16& x) -> void {
  r.a = XOR(r.a, read(displace(x)));
}

auto Z80::instructionXOR_a_r(uint8& x) -> void {
  r.a = XOR(r.a, x);
}
