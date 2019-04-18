auto HuC6280::algorithmADC(uint8 i) -> uint8 {
  int16 o;
  if(!D) {
    o = A + i + C;
    V = ~(A ^ i) & (A ^ o) & 0x80;
  } else {
    io();
    o = (A & 0x0f) + (i & 0x0f) + (C << 0);
    if(o > 0x09) o += 0x06;
    C = o > 0x0f;
    o = (A & 0xf0) + (i & 0xf0) + (C << 4) + (o & 0x0f);
    if(o > 0x9f) o += 0x60;
  }
  C = o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::algorithmAND(uint8 i) -> uint8 {
  uint8 o = A & i;
  Z = o == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::algorithmASL(uint8 i) -> uint8 {
  C = i.bit(7);
  i <<= 1;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmBIT(uint8 i) -> uint8 {
  Z = (A & i) == 0;
  V = i.bit(6);
  N = i.bit(7);
  return A;
}

auto HuC6280::algorithmCMP(uint8 i) -> uint8 {
  uint9 o = A - i;
  C = !o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return A;
}

auto HuC6280::algorithmCPX(uint8 i) -> uint8 {
  uint9 o = X - i;
  C = !o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return X;
}

auto HuC6280::algorithmCPY(uint8 i) -> uint8 {
  uint9 o = Y - i;
  C = !o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return Y;
}

auto HuC6280::algorithmDEC(uint8 i) -> uint8 {
  i--;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmEOR(uint8 i) -> uint8 {
  uint8 o = A ^ i;
  Z = o == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::algorithmINC(uint8 i) -> uint8 {
  i++;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmLD(uint8 i) -> uint8 {
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmLSR(uint8 i) -> uint8 {
  C = i.bit(0);
  i >>= 1;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmORA(uint8 i) -> uint8 {
  uint8 o = A | i;
  Z = o == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::algorithmROL(uint8 i) -> uint8 {
  bool c = C;
  C = i.bit(7);
  i = i << 1 | c;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmROR(uint8 i) -> uint8 {
  bool c = C;
  C = i.bit(0);
  i = c << 7 | i >> 1;
  Z = i == 0;
  N = i.bit(7);
  return i;
}

auto HuC6280::algorithmSBC(uint8 i) -> uint8 {
  i ^= 0xff;
  int16 o;
  if(!D) {
    o = A + i + C;
    V = ~(A ^ i) & (A ^ o) & 0x80;
  } else {
    io();
    o = (A & 0x0f) + (i & 0x0f) + (C << 0);
    if(o <= 0x0f) o -= 0x06;
    C = o > 0x0f;
    o = (A & 0xf0) + (i & 0xf0) + (C << 4) + (o & 0x0f);
    if(o <= 0xff) o -= 0x60;
  }
  C = o.bit(8);
  Z = uint8(o) == 0;
  N = o.bit(7);
  return o;
}

auto HuC6280::algorithmTRB(uint8 i) -> uint8 {
  Z = (A & i) == 0;
  V = i.bit(6);
  N = i.bit(7);
  return ~A & i;
}

auto HuC6280::algorithmTSB(uint8 i) -> uint8 {
  Z = (A & i) == 0;
  V = i.bit(6);
  N = i.bit(7);
  return A | i;
}

//

auto HuC6280::algorithmTAI(uint16& source, uint16& target, bool alternate) -> void {
  !alternate ? source++ : source--;
  target++;
}

auto HuC6280::algorithmTDD(uint16& source, uint16& target, bool) -> void {
  source--;
  target--;
}

auto HuC6280::algorithmTIA(uint16& source, uint16& target, bool alternate) -> void {
  source++;
  !alternate ? target++ : target--;
}

auto HuC6280::algorithmTII(uint16& source, uint16& target, bool) -> void {
  source++;
  target++;
}

auto HuC6280::algorithmTIN(uint16& source, uint16& target, bool) -> void {
  source++;
}
