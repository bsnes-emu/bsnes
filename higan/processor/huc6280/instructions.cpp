auto HuC6280::ADC(uint8 i) -> uint8 {
  int16 o;
  if(!D) {
    o = A + i + C;
    V = ~(A ^ i) & (A ^ o) & 0x80;
  } else {
    o = (A & 0x0f) + (i & 0x0f) + (C << 0);
    if(o > 0x09) o += 0x06;
    C = o > 0x0f;
    o = (A & 0xf0) + (i & 0xf0) + (C << 4) + (o & 0x0f);
    V = ~(A ^ i) & (A ^ o) & 0x80;
    if(o > 0x9f) o += 0x60;
  }
  C = o.bit(8);
  Z = uint8(o) == 0;
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
  Z = (A & i) == 0;
  V = i.bit(6);
  N = i.bit(7);
  return A;
}

auto HuC6280::CMP(uint8 i) -> uint8 {
  uint9 o = A - i;
  C = !o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return A;
}

auto HuC6280::CPX(uint8 i) -> uint8 {
  uint9 o = X - i;
  C = !o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return X;
}

auto HuC6280::CPY(uint8 i) -> uint8 {
  uint9 o = Y - i;
  C = !o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return Y;
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
  i ^= 0xff;
  int16 o;
  if(!D) {
    o = A + i + C;
    V = ~(A ^ i) & (A ^ o) & 0x80;
  } else {
    o = (A & 0x0f) + (i & 0x0f) + (C << 0);
    if(o <= 0x0f) o -= 0x06;
    C = o > 0x0f;
    o = (A & 0xf0) + (i & 0xf0) + (C << 4) + (o & 0x0f);
    V = ~(A ^ i) & (A ^ o) & 0x80;
    if(o <= 0xff) o -= 0x60;
  }
  C = o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::TRB(uint8 i) -> uint8 {
  Z = (A & i) == 0;
  V = i.bit(6);
  N = i.bit(7);
  return ~A & i;
}

auto HuC6280::TSB(uint8 i) -> uint8 {
  Z = (A & i) == 0;
  V = i.bit(6);
  N = i.bit(7);
  return A | i;
}

//

auto HuC6280::TAI(uint16& source, uint16& target, bool alternate) -> void {
  !alternate ? source++ : source--;
  target++;
}

auto HuC6280::TDD(uint16& source, uint16& target, bool) -> void {
  source--;
  target--;
}

auto HuC6280::TIA(uint16& source, uint16& target, bool alternate) -> void {
  source++;
  !alternate ? target++ : target--;
}

auto HuC6280::TII(uint16& source, uint16& target, bool) -> void {
  source++;
  target++;
}

auto HuC6280::TIN(uint16& source, uint16& target, bool) -> void {
  source++;
}

//

auto HuC6280::instruction_absoluteLoad(fp alu, uint8& data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L data = ALU(load16(absolute + index));
}

auto HuC6280::instruction_absoluteModify(fp alu, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
  io();
  auto data = ALU(load16(absolute + index));
L store16(absolute + index, data);
}

auto HuC6280::instruction_absoluteStore(uint8 data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L store16(absolute + index, data);
}

auto HuC6280::instruction_blockmove(bp alu) -> void {
  uint16 source = operand();
  source |= operand() << 8;
  uint16 target = operand();
  target |= operand() << 8;
  uint16 length = operand();
  length |= operand() << 8;
  push(Y);
  push(A);
  push(X);
  io();
  io();
  io();
  io();
  io();
  bool alternate = 0;
  do {
    auto data = load16(source);
    store16(target, data);
    ALU(source, target, alternate);
    alternate ^= 1;
    io();
    io();
    io();
    io();
  } while(--length);
  X = pull();
  A = pull();
L Y = pull();
}

auto HuC6280::instruction_branch(bool take) -> void {
  if(!take) {
  L operand();
  } else {
    auto displacement = operand();
    io();
  L io();
    PC += (int8)displacement;
  }
}

auto HuC6280::instruction_clear(uint8& data) -> void {
L io();
  data = 0;
}

auto HuC6280::instruction_clear(bool& flag) -> void {
L io();
  flag = 0;
}

auto HuC6280::instruction_immediate(fp alu, uint8& data) -> void {
L data = ALU(operand());
}

auto HuC6280::instruction_implied(fp alu, uint8& data) -> void {
L io();
  data = ALU(data);
}

auto HuC6280::instruction_indirectLoad(fp alu, uint8& data, uint8 index) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
  io();
L data = ALU(load16(absolute));
}

auto HuC6280::instruction_indirectStore(uint8 data, uint8 index) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
L store16(absolute, data);
}

auto HuC6280::instruction_indirectYLoad(fp alu, uint8& data) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
  io();
L data = ALU(load16(absolute + Y));
}

auto HuC6280::instruction_indirectYStore(uint8 data) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
L store16(absolute + Y, data);
}

auto HuC6280::instruction_memory(fp alu) -> void {
  auto a = A;
  A = ALU(load8(X));
L store8(X, A);
  A = a;
}

auto HuC6280::instruction_pull(uint8& data) -> void {
  io();
  io();
L data = pull();
  Z = data == 0;
  N = data.bit(7);
}

auto HuC6280::instruction_push(uint8 data) -> void {
  io();
L push(data);
}

auto HuC6280::instruction_set(bool& flag) -> void {
L io();
  flag = 1;
}

auto HuC6280::instruction_swap(uint8& lhs, uint8& rhs) -> void {
  io();
L io();
  swap(lhs, rhs);
}

auto HuC6280::instruction_transfer(uint8& source, uint8& target) -> void {
L io();
  target = source;
  Z = target == 0;
  N = target.bit(7);
}

auto HuC6280::instruction_zeropageLoad(fp alu, uint8& data, uint8 index) -> void {
  auto zeropage = operand();
  io();
L data = ALU(load8(zeropage + index));
}

auto HuC6280::instruction_zeropageModify(fp alu, uint8 index) -> void {
  auto zeropage = operand();
  io();
  io();
  auto data = ALU(load8(zeropage + index));
L store8(zeropage + index, data);
}

auto HuC6280::instruction_zeropageStore(uint8 data, uint8 index) -> void {
  auto zeropage = operand();
  io();
L store8(zeropage + index, data);
}

//

auto HuC6280::instruction_BBR(uint3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  io();
  io();
L auto data = load8(zeropage);
  if(data.bit(index) == 0) {
    PC += (int8)displacement;
  }
}

auto HuC6280::instruction_BBS(uint3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  io();
  io();
L auto data = load8(zeropage);
  if(data.bit(index) == 1) {
    PC += (int8)displacement;
  }
}

auto HuC6280::instruction_BRK() -> void {
  operand();
  io();
  push(PC >> 8);
  push(PC >> 0);
  uint8 p = P;
  push(p | 0x10);  //B flag set on push
  D = 0;
  I = 1;
  PC.byte(0) = load16(0xfff6);
L PC.byte(1) = load16(0xfff7);
}

auto HuC6280::instruction_BSR() -> void {
  auto displacement = operand();
  io();
  io();
  io();
  io();
  push((PC - 1) >> 8);
L push((PC - 1) >> 0);
  PC += (int8)displacement;
}

auto HuC6280::instruction_CSL() -> void {
L io();
  r.cs = 4;
}

auto HuC6280::instruction_CSH() -> void {
L io();
  r.cs = 1;
}

auto HuC6280::instruction_JMP_absolute() -> void {
  uint16 address = operand();
  address |= operand() << 8;
L io();
  PC = address;
}

auto HuC6280::instruction_JMP_indirect(uint8 index) -> void {
  uint16 address = operand();
  address |= operand() << 8;
  io();
  io();
  PC.byte(0) = load16(address + index + 0);
L PC.byte(1) = load16(address + index + 1);
}

auto HuC6280::instruction_JSR() -> void {
  uint16 address = operand();
  address |= operand() << 8;
  io();
  io();
  push((PC - 1) >> 8);
L push((PC - 1) >> 0);
  PC = address;
}

auto HuC6280::instruction_NOP() -> void {
L io();
}

auto HuC6280::instruction_PLP() -> void {
  io();
  io();
L P = pull();
}

auto HuC6280::instruction_RMB(uint3 index) -> void {
  auto zeropage = operand();
  io();
  io();
  io();
  auto data = load8(zeropage);
  data.bit(index) = 0;
L store8(zeropage, data);
}

auto HuC6280::instruction_RTI() -> void {
  io();
  io();
  io();
  P = pull();
  PC.byte(0) = pull();
L PC.byte(1) = pull();
}

auto HuC6280::instruction_RTS() -> void {
  io();
  io();
  io();
  PC.byte(0) = pull();
  PC.byte(1) = pull();
L io();
  PC++;
}

auto HuC6280::instruction_SMB(uint3 index) -> void {
  auto zeropage = operand();
  io();
  io();
  io();
  auto data = load8(zeropage);
  data.bit(index) = 1;
L store8(zeropage, data);
}

auto HuC6280::instruction_ST(uint2 index) -> void {
  auto data = operand();
  io();
L io();
  store(index, data);
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

auto HuC6280::instruction_TMA() -> void {
  auto mask = operand();
  io();
L io();
  for(uint index : range(8)) {
    if(mask.bit(index)) { A = r.mpr[index]; break; }
  }
}

auto HuC6280::instruction_TST_absolute(uint8 index) -> void {
  auto mask = operand();
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
  io();
  io();
L uint8 data = load16(absolute + index);
  Z = (data & mask) == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instruction_TST_zeropage(uint8 index) -> void {
  auto mask = operand();
  auto zeropage = operand();
  io();
  io();
  io();
L uint8 data = load8(zeropage + index);
  Z = (data & mask) == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instruction_TXS() -> void {
L io();
  S = X;
}
