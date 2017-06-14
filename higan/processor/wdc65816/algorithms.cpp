auto WDC65816::algorithmADC8(uint8 data) -> uint8 {
  int result;

  if(!DF) {
    result = lo(A) + data + CF;
  } else {
    result = (lo(A) & 0x0f) + (data & 0x0f) + (CF << 0);
    if(result > 0x09) result += 0x06;
    CF = result > 0x0f;
    result = (lo(A) & 0xf0) + (data & 0xf0) + (CF << 4) + (result & 0x0f);
  }

  VF = ~(lo(A) ^ data) & (lo(A) ^ result) & 0x80;
  if(DF && result > 0x9f) result += 0x60;
  CF = result > 0xff;
  ZF = (uint8)result == 0;
  NF = result & 0x80;

  return lo(A) = result;
}

auto WDC65816::algorithmADC16(uint16 data) -> uint16 {
  int result;

  if(!DF) {
    result = A + data + CF;
  } else {
    result = (A & 0x000f) + (data & 0x000f) + (CF <<  0);
    if(result > 0x0009) result += 0x0006;
    CF = result > 0x000f;
    result = (A & 0x00f0) + (data & 0x00f0) + (CF <<  4) + (result & 0x000f);
    if(result > 0x009f) result += 0x0060;
    CF = result > 0x00ff;
    result = (A & 0x0f00) + (data & 0x0f00) + (CF <<  8) + (result & 0x00ff);
    if(result > 0x09ff) result += 0x0600;
    CF = result > 0x0fff;
    result = (A & 0xf000) + (data & 0xf000) + (CF << 12) + (result & 0x0fff);
  }

  VF = ~(A ^ data) & (A ^ result) & 0x8000;
  if(D && result > 0x9fff) result += 0x6000;
  CF = result > 0xffff;
  ZF = (uint16)result == 0;
  NF = result & 0x8000;

  return A = result;
}

auto WDC65816::algorithmAND8(uint8 data) -> uint8 {
  lo(A) &= data;
  ZF = A == 0;
  NF = A & 0x80;
  return data;
}

auto WDC65816::algorithmAND16(uint16 data) -> uint16 {
  A &= data;
  ZF = A == 0;
  NF = A & 0x8000;
  return data;
}

auto WDC65816::algorithmASL8(uint8 data) -> uint8 {
  CF = data & 0x80;
  data <<= 1;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmASL16(uint16 data) -> uint16 {
  CF = data & 0x8000;
  data <<= 1;
  ZF = data == 0;
  NF = data & 0x8000;
  return data;
}

auto WDC65816::algorithmBIT8(uint8 data) -> uint8 {
  ZF = (data & lo(A)) == 0;
  VF = data & 0x40;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmBIT16(uint16 data) -> uint16 {
  ZF = (data & A) == 0;
  VF = data & 0x4000;
  NF = data & 0x8000;
  return data;
}

auto WDC65816::algorithmCMP8(uint8 data) -> uint8 {
  int result = lo(A) - data;
  CF = result >= 0;
  ZF = (uint8)result == 0;
  NF = result & 0x80;
  return result;
}

auto WDC65816::algorithmCMP16(uint16 data) -> uint16 {
  int result = A - data;
  CF = result >= 0;
  ZF = (uint16)result == 0;
  NF = result & 0x8000;
  return result;
}

auto WDC65816::algorithmCPX8(uint8 data) -> uint8 {
  int result = lo(X) - data;
  CF = result >= 0;
  ZF = (uint8)result == 0;
  NF = result & 0x80;
  return result;
}

auto WDC65816::algorithmCPX16(uint16 data) -> uint16 {
  int result = X - data;
  CF = result >= 0;
  ZF = (uint16)result == 0;
  NF = result & 0x8000;
  return result;
}

auto WDC65816::algorithmCPY8(uint8 data) -> uint8 {
  int result = lo(Y) - data;
  CF = result >= 0;
  ZF = (uint8)result == 0;
  NF = result & 0x80;
  return result;
}

auto WDC65816::algorithmCPY16(uint16 data) -> uint16 {
  int result = Y - data;
  CF = result >= 0;
  ZF = (uint16)result == 0;
  NF = result & 0x8000;
  return result;
}

auto WDC65816::algorithmDEC8(uint8 data) -> uint8 {
  data--;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmDEC16(uint16 data) -> uint16 {
  data--;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmEOR8(uint8 data) -> uint8 {
  lo(A) ^= data;
  ZF = A == 0;
  NF = A & 0x80;
  return lo(A);
}

auto WDC65816::algorithmEOR16(uint16 data) -> uint16 {
  A ^= data;
  ZF = A == 0;
  NF = A & 0x8000;
  return data;
}

auto WDC65816::algorithmINC8(uint8 data) -> uint8 {
  data++;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmINC16(uint16 data) -> uint16 {
  data++;
  ZF = data == 0;
  NF = data & 0x8000;
  return data;
}

auto WDC65816::algorithmLDA8(uint8 data) -> uint8 {
  lo(A) = data;
  ZF = lo(A) == 0;
  NF = A & 0x80;
  return data;
}

auto WDC65816::algorithmLDA16(uint16 data) -> uint16 {
  A = data;
  ZF = A == 0;
  NF = A & 0x8000;
  return data;
}

auto WDC65816::algorithmLDX8(uint8 data) -> uint8 {
  lo(X) = data;
  ZF = lo(X) == 0;
  NF = X & 0x80;
  return data;
}

auto WDC65816::algorithmLDX16(uint16 data) -> uint16 {
  X = data;
  ZF = X == 0;
  NF = X & 0x8000;
  return data;
}

auto WDC65816::algorithmLDY8(uint8 data) -> uint8 {
  lo(Y) = data;
  ZF = lo(Y) == 0;
  NF = Y & 0x80;
  return data;
}

auto WDC65816::algorithmLDY16(uint16 data) -> uint16 {
  Y = data;
  ZF = Y == 0;
  NF = Y & 0x8000;
  return data;
}

auto WDC65816::algorithmLSR8(uint8 data) -> uint8 {
  CF = data & 1;
  data >>= 1;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmLSR16(uint16 data) -> uint16 {
  CF = data & 1;
  data >>= 1;
  ZF = data == 0;
  NF = data & 0x8000;
  return data;
}

auto WDC65816::algorithmORA8(uint8 data) -> uint8 {
  lo(A) |= data;
  ZF = lo(A) == 0;
  NF = A & 0x80;
  return lo(A);
}

auto WDC65816::algorithmORA16(uint16 data) -> uint16 {
  A |= data;
  ZF = A == 0;
  NF = A & 0x8000;
  return A;
}

auto WDC65816::algorithmROL8(uint8 data) -> uint8 {
  bool carry = CF;
  CF = data & 0x80;
  data = data << 1 | carry;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmROL16(uint16 data) -> uint16 {
  bool carry = CF;
  CF = data & 0x8000;
  data = data << 1 | carry;
  ZF = data == 0;
  NF = data & 0x8000;
  return data;
}

auto WDC65816::algorithmROR8(uint8 data) -> uint8 {
  bool carry = CF;
  CF = data & 1;
  data = carry << 7 | data >> 1;
  ZF = data == 0;
  NF = data & 0x80;
  return data;
}

auto WDC65816::algorithmROR16(uint16 data) -> uint16 {
  bool carry = CF;
  CF = data & 1;
  data = carry << 15 | data >> 1;
  ZF = data == 0;
  NF = data & 0x8000;
  return data;
}

auto WDC65816::algorithmSBC8(uint8 data) -> uint8 {
  int result;
  data = ~data;

  if(!DF) {
    result = lo(A) + data + CF;
  } else {
    result = (lo(A) & 0x0f) + (data & 0x0f) + (CF << 0);
    if(result <= 0x0f) result -= 0x06;
    CF = result > 0x0f;
    result = (lo(A) & 0xf0) + (data & 0xf0) + (CF << 4) + (result & 0x0f);
  }

  VF = ~(lo(A) ^ data) & (lo(A) ^ result) & 0x80;
  if(DF && result <= 0xff) result -= 0x60;
  CF = result > 0xff;
  ZF = (uint8)result == 0;
  NF = result & 0x80;

  return lo(A) = result;
}

auto WDC65816::algorithmSBC16(uint16 data) -> uint16 {
  int result;
  data = ~data;

  if(!DF) {
    result = A + data + CF;
  } else {
    result = (A & 0x000f) + (data & 0x000f) + (CF <<  0);
    if(result <= 0x000f) result -= 0x0006;
    CF = result > 0x000f;
    result = (A & 0x00f0) + (data & 0x00f0) + (CF <<  4) + (result & 0x000f);
    if(result <= 0x00ff) result -= 0x0060;
    CF = result > 0x00ff;
    result = (A & 0x0f00) + (data & 0x0f00) + (CF <<  8) + (result & 0x00ff);
    if(result <= 0x0fff) result -= 0x0600;
    CF = result > 0x0fff;
    result = (A & 0xf000) + (data & 0xf000) + (CF << 12) + (result & 0x0fff);
  }

  VF = ~(A ^ data) & (A ^ result) & 0x8000;
  if(DF && result <= 0xffff) result -= 0x6000;
  CF = result > 0xffff;
  ZF = (uint16)result == 0;
  NF = result & 0x8000;

  return A = result;
}

auto WDC65816::algorithmTRB8(uint8 data) -> uint8 {
  ZF = (data & lo(A)) == 0;
  data &= ~lo(A);
  return data;
}

auto WDC65816::algorithmTRB16(uint16 data) -> uint16 {
  ZF = (data & A) == 0;
  data &= ~A;
  return data;
}

auto WDC65816::algorithmTSB8(uint8 data) -> uint8 {
  ZF = (data & lo(A)) == 0;
  data |= lo(A);
  return data;
}

auto WDC65816::algorithmTSB16(uint16 data) -> uint16 {
  ZF = (data & A) == 0;
  data |= A;
  return data;
}
