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

auto HuC6280::TRB(uint8 i) -> uint8 {
  uint8 o = i & A;
  Z = o == 0;
  V = o.bit(6);
  N = o.bit(7);
  return i & ~A;
}

auto HuC6280::TSB(uint8 i) -> uint8 {
  uint8 o = i & A;
  Z = o == 0;
  V = o.bit(6);
  N = o.bit(7);
  return i | A;
}

//

auto HuC6280::TAI(uint16& source, uint16& target) -> void {
  source ^= 1;
  target++;
}

auto HuC6280::TDD(uint16& source, uint16& target) -> void {
  source--;
  target--;
}

auto HuC6280::TIA(uint16& source, uint16& target) -> void {
  source++;
  target ^= 1;
}

auto HuC6280::TII(uint16& source, uint16& target) -> void {
  source++;
  target++;
}

auto HuC6280::TIN(uint16& source, uint16& target) -> void {
  source++;
}

//

auto HuC6280::instruction_absoluteLoad(fp alu, uint8& data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L data = ALU(load(absolute + index));
}

auto HuC6280::instruction_absoluteModify(fp alu, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
  io();
  auto data = ALU(load(absolute + index));
L store(absolute + index, data);
}

auto HuC6280::instruction_absoluteStore(uint8 data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L store(absolute + index, data);
}

auto HuC6280::instruction_blockmove(bp alu) -> void {
  uint16 source = operand();
  source |= operand() << 8;
  uint16 target = operand();
  target |= operand() << 8;
  uint16 length = operand();
  length |= operand() << 8;
  do {
    auto data = load(source);
    store(target, data);
    ALU(source, target);
  } while(--length);
L io();
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
  data = 0;
L io();
}

auto HuC6280::instruction_clear(bool& flag) -> void {
  flag = 0;
L io();
}

auto HuC6280::instruction_immediate(fp alu, uint8& data) -> void {
  data = ALU(operand());
L io();
}

auto HuC6280::instruction_implied(fp alu, uint8& data) -> void {
  data = ALU(data);
L io();
}

auto HuC6280::instruction_indirectLoad(fp alu, uint8& data, uint8 index) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load(0x2000 + zeropage + index);
  absolute |= load(0x2001 + zeropage + index) << 8;
  io();
L data = ALU(load(absolute));
}

auto HuC6280::instruction_indirectStore(uint8 data, uint8 index) -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage + index);
  absolute |= load(0x2001 + zeropage + index) << 8;
L store(absolute, data);
}

auto HuC6280::instruction_indirectYLoad(fp alu, uint8& data) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load(0x2000 + zeropage);
  absolute |= load(0x2001 + zeropage) << 8;
  io();
L data = ALU(load(absolute + Y));
}

auto HuC6280::instruction_indirectYStore(uint8 data) -> void {
  auto zeropage = operand();
  io();
  auto absolute = load(0x2000 + zeropage);
  absolute |= load(0x2001 + zeropage) << 8;
L store(absolute + Y, data);
}

auto HuC6280::instruction_memory(fp alu) -> void {
  auto a = A;
  A = ALU(load(0x2000 + X));
L store(0x2000 + X, A);
  A = a;
}

auto HuC6280::instruction_pull(uint8& data) -> void {
  io();
  io();
L data = pull();
}

auto HuC6280::instruction_pullP() -> void {
  io();
  io();
L P = pull();
}

auto HuC6280::instruction_push(uint8 data) -> void {
  io();
L push(data);
}

auto HuC6280::instruction_set(bool& flag) -> void {
  flag = 1;
L io();
}

auto HuC6280::instruction_swap(uint8& lhs, uint8& rhs) -> void {
  swap(lhs, rhs);
  io();
L io();
}

auto HuC6280::instruction_transfer(uint8& source, uint8& target) -> void {
  target = source;
L io();
  Z = target == 0;
  N = target.bit(7);
}

auto HuC6280::instruction_zeropageLoad(fp alu, uint8& data, uint8 index) -> void {
  auto zeropage = operand();
  io();
L data = ALU(load(0x2000 + zeropage + index));
}

auto HuC6280::instruction_zeropageModify(fp alu, uint8 index) -> void {
  auto zeropage = operand();
  io();
  io();
  auto data = ALU(load(0x2000 + zeropage + index));
L store(0x2000 + zeropage + index, data);
}

auto HuC6280::instruction_zeropageStore(uint8 data, uint8 index) -> void {
  auto zeropage = operand();
  io();
L store(0x2000 + zeropage + index, data);
}

//

auto HuC6280::instruction_BBR(uint3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  io();
  io();
L io();
  if(zeropage.bit(index) == 0) {
    PC += (int8)displacement;
  }
}

auto HuC6280::instruction_BBS(uint3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  io();
  io();
L io();
  if(zeropage.bit(index) == 1) {
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
  I = 1;
  D = 0;
  PC.byte(0) = load(0xfff6);
L PC.byte(1) = load(0xfff7);
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
  r.cs = 12;
L io();
}

auto HuC6280::instruction_CSH() -> void {
  r.cs = 3;
L io();
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
  PC.byte(0) = load(address + index + 0);
L PC.byte(1) = load(address + index + 1);
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

auto HuC6280::instruction_RMB(uint3 index) -> void {
  auto zeropage = operand();
  io();
  io();
  io();
  auto data = load(0x2000 + zeropage);
  data.bit(index) = 0;
L store(0x2000 + zeropage, data);
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
  auto data = load(0x2000 + zeropage);
  data.bit(index) = 1;
L store(0x2000 + zeropage, data);
}

auto HuC6280::instruction_ST(uint2 index) -> void {
  auto data = operand();
  io();
L io();
  st(index, data);
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
L uint8 data = load(absolute + index) & mask;
  Z = data == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instruction_TST_zeropage(uint8 index) -> void {
  auto mask = operand();
  auto zeropage = operand();
  io();
  io();
  io();
L uint8 data = load(0x2000 + zeropage + index) & mask;
  Z = data == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instruction_TXS() -> void {
  S = X;
L io();
}
