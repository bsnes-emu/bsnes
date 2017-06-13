auto WDC65816::algorithmADC8() -> void {
  int result;

  if(!r.p.d) {
    result = r.a.l + rd.l + r.p.c;
  } else {
    result = (r.a.l & 0x0f) + (rd.l & 0x0f) + (r.p.c << 0);
    if(result > 0x09) result += 0x06;
    r.p.c = result > 0x0f;
    result = (r.a.l & 0xf0) + (rd.l & 0xf0) + (r.p.c << 4) + (result & 0x0f);
  }

  r.p.v = ~(r.a.l ^ rd.l) & (r.a.l ^ result) & 0x80;
  if(r.p.d && result > 0x9f) result += 0x60;
  r.p.c = result > 0xff;
  r.p.n = result & 0x80;
  r.p.z = (uint8)result == 0;

  r.a.l = result;
}

auto WDC65816::algorithmADC16() -> void {
  int result;

  if(!r.p.d) {
    result = r.a.w + rd.w + r.p.c;
  } else {
    result = (r.a.w & 0x000f) + (rd.w & 0x000f) + (r.p.c <<  0);
    if(result > 0x0009) result += 0x0006;
    r.p.c = result > 0x000f;
    result = (r.a.w & 0x00f0) + (rd.w & 0x00f0) + (r.p.c <<  4) + (result & 0x000f);
    if(result > 0x009f) result += 0x0060;
    r.p.c = result > 0x00ff;
    result = (r.a.w & 0x0f00) + (rd.w & 0x0f00) + (r.p.c <<  8) + (result & 0x00ff);
    if(result > 0x09ff) result += 0x0600;
    r.p.c = result > 0x0fff;
    result = (r.a.w & 0xf000) + (rd.w & 0xf000) + (r.p.c << 12) + (result & 0x0fff);
  }

  r.p.v = ~(r.a.w ^ rd.w) & (r.a.w ^ result) & 0x8000;
  if(r.p.d && result > 0x9fff) result += 0x6000;
  r.p.c = result > 0xffff;
  r.p.n = result & 0x8000;
  r.p.z = (uint16)result == 0;

  r.a.w = result;
}

auto WDC65816::algorithmAND8() -> void {
  r.a.l &= rd.l;
  r.p.n = r.a.l & 0x80;
  r.p.z = r.a.l == 0;
}

auto WDC65816::algorithmAND16() -> void {
  r.a.w &= rd.w;
  r.p.n = r.a.w & 0x8000;
  r.p.z = r.a.w == 0;
}

auto WDC65816::algorithmASL8() -> void {
  r.p.c = rd.l & 0x80;
  rd.l <<= 1;
  r.p.n = rd.l & 0x80;
  r.p.z = rd.l == 0;
}

auto WDC65816::algorithmASL16() -> void {
  r.p.c = rd.w & 0x8000;
  rd.w <<= 1;
  r.p.n = rd.w & 0x8000;
  r.p.z = rd.w == 0;
}

auto WDC65816::algorithmBIT8() -> void {
  r.p.n = rd.l & 0x80;
  r.p.v = rd.l & 0x40;
  r.p.z = (rd.l & r.a.l) == 0;
}

auto WDC65816::algorithmBIT16() -> void {
  r.p.n = rd.w & 0x8000;
  r.p.v = rd.w & 0x4000;
  r.p.z = (rd.w & r.a.w) == 0;
}

auto WDC65816::algorithmCMP8() -> void {
  int result = r.a.l - rd.l;
  r.p.n = result & 0x80;
  r.p.z = (uint8)result == 0;
  r.p.c = result >= 0;
}

auto WDC65816::algorithmCMP16() -> void {
  int result = r.a.w - rd.w;
  r.p.n = result & 0x8000;
  r.p.z = (uint16)result == 0;
  r.p.c = result >= 0;
}

auto WDC65816::algorithmCPX8() -> void {
  int result = r.x.l - rd.l;
  r.p.n = result & 0x80;
  r.p.z = (uint8)result == 0;
  r.p.c = result >= 0;
}

auto WDC65816::algorithmCPX16() -> void {
  int result = r.x.w - rd.w;
  r.p.n = result & 0x8000;
  r.p.z = (uint16)result == 0;
  r.p.c = result >= 0;
}

auto WDC65816::algorithmCPY8() -> void {
  int result = r.y.l - rd.l;
  r.p.n = result & 0x80;
  r.p.z = (uint8)result == 0;
  r.p.c = result >= 0;
}

auto WDC65816::algorithmCPY16() -> void {
  int result = r.y.w - rd.w;
  r.p.n = result & 0x8000;
  r.p.z = (uint16)result == 0;
  r.p.c = result >= 0;
}

auto WDC65816::algorithmDEC8() -> void {
  rd.l--;
  r.p.n = rd.l & 0x80;
  r.p.z = rd.l == 0;
}

auto WDC65816::algorithmDEC16() -> void {
  rd.w--;
  r.p.n = rd.w & 0x8000;
  r.p.z = rd.w == 0;
}

auto WDC65816::algorithmEOR8() -> void {
  r.a.l ^= rd.l;
  r.p.n = r.a.l & 0x80;
  r.p.z = r.a.l == 0;
}

auto WDC65816::algorithmEOR16() -> void {
  r.a.w ^= rd.w;
  r.p.n = r.a.w & 0x8000;
  r.p.z = r.a.w == 0;
}

auto WDC65816::algorithmINC8() -> void {
  rd.l++;
  r.p.n = rd.l & 0x80;
  r.p.z = rd.l == 0;
}

auto WDC65816::algorithmINC16() -> void {
  rd.w++;
  r.p.n = rd.w & 0x8000;
  r.p.z = rd.w == 0;
}

auto WDC65816::algorithmLDA8() -> void {
  r.a.l = rd.l;
  r.p.n = r.a.l & 0x80;
  r.p.z = r.a.l == 0;
}

auto WDC65816::algorithmLDA16() -> void {
  r.a.w = rd.w;
  r.p.n = r.a.w & 0x8000;
  r.p.z = r.a.w == 0;
}

auto WDC65816::algorithmLDX8() -> void {
  r.x.l = rd.l;
  r.p.n = r.x.l & 0x80;
  r.p.z = r.x.l == 0;
}

auto WDC65816::algorithmLDX16() -> void {
  r.x.w = rd.w;
  r.p.n = r.x.w & 0x8000;
  r.p.z = r.x.w == 0;
}

auto WDC65816::algorithmLDY8() -> void {
  r.y.l = rd.l;
  r.p.n = r.y.l & 0x80;
  r.p.z = r.y.l == 0;
}

auto WDC65816::algorithmLDY16() -> void {
  r.y.w = rd.w;
  r.p.n = r.y.w & 0x8000;
  r.p.z = r.y.w == 0;
}

auto WDC65816::algorithmLSR8() -> void {
  r.p.c = rd.l & 1;
  rd.l >>= 1;
  r.p.n = rd.l & 0x80;
  r.p.z = rd.l == 0;
}

auto WDC65816::algorithmLSR16() -> void {
  r.p.c = rd.w & 1;
  rd.w >>= 1;
  r.p.n = rd.w & 0x8000;
  r.p.z = rd.w == 0;
}

auto WDC65816::algorithmORA8() -> void {
  r.a.l |= rd.l;
  r.p.n = r.a.l & 0x80;
  r.p.z = r.a.l == 0;
}

auto WDC65816::algorithmORA16() -> void {
  r.a.w |= rd.w;
  r.p.n = r.a.w & 0x8000;
  r.p.z = r.a.w == 0;
}

auto WDC65816::algorithmROL8() -> void {
  auto carry = (uint)r.p.c;
  r.p.c = rd.l & 0x80;
  rd.l = (rd.l << 1) | carry;
  r.p.n = rd.l & 0x80;
  r.p.z = rd.l == 0;
}

auto WDC65816::algorithmROL16() -> void {
  auto carry = (uint)r.p.c;
  r.p.c = rd.w & 0x8000;
  rd.w = (rd.w << 1) | carry;
  r.p.n = rd.w & 0x8000;
  r.p.z = rd.w == 0;
}

auto WDC65816::algorithmROR8() -> void {
  auto carry = (uint)r.p.c << 7;
  r.p.c = rd.l & 1;
  rd.l = carry | (rd.l >> 1);
  r.p.n = rd.l & 0x80;
  r.p.z = rd.l == 0;
}

auto WDC65816::algorithmROR16() -> void {
  auto carry = (uint)r.p.c << 15;
  r.p.c = rd.w & 1;
  rd.w = carry | (rd.w >> 1);
  r.p.n = rd.w & 0x8000;
  r.p.z = rd.w == 0;
}

auto WDC65816::algorithmSBC8() -> void {
  int result;
  rd.l ^= 0xff;

  if(!r.p.d) {
    result = r.a.l + rd.l + r.p.c;
  } else {
    result = (r.a.l & 0x0f) + (rd.l & 0x0f) + (r.p.c << 0);
    if(result <= 0x0f) result -= 0x06;
    r.p.c = result > 0x0f;
    result = (r.a.l & 0xf0) + (rd.l & 0xf0) + (r.p.c << 4) + (result & 0x0f);
  }

  r.p.v = ~(r.a.l ^ rd.l) & (r.a.l ^ result) & 0x80;
  if(r.p.d && result <= 0xff) result -= 0x60;
  r.p.c = result > 0xff;
  r.p.n = result & 0x80;
  r.p.z = (uint8_t)result == 0;

  r.a.l = result;
}

auto WDC65816::algorithmSBC16() -> void {
  int result;
  rd.w ^= 0xffff;

  if(!r.p.d) {
    result = r.a.w + rd.w + r.p.c;
  } else {
    result = (r.a.w & 0x000f) + (rd.w & 0x000f) + (r.p.c <<  0);
    if(result <= 0x000f) result -= 0x0006;
    r.p.c = result > 0x000f;
    result = (r.a.w & 0x00f0) + (rd.w & 0x00f0) + (r.p.c <<  4) + (result & 0x000f);
    if(result <= 0x00ff) result -= 0x0060;
    r.p.c = result > 0x00ff;
    result = (r.a.w & 0x0f00) + (rd.w & 0x0f00) + (r.p.c <<  8) + (result & 0x00ff);
    if(result <= 0x0fff) result -= 0x0600;
    r.p.c = result > 0x0fff;
    result = (r.a.w & 0xf000) + (rd.w & 0xf000) + (r.p.c << 12) + (result & 0x0fff);
  }

  r.p.v = ~(r.a.w ^ rd.w) & (r.a.w ^ result) & 0x8000;
  if(r.p.d && result <= 0xffff) result -= 0x6000;
  r.p.c = result > 0xffff;
  r.p.n = result & 0x8000;
  r.p.z = (uint16_t)result == 0;

  r.a.w = result;
}

auto WDC65816::algorithmTRB8() -> void {
  r.p.z = (rd.l & r.a.l) == 0;
  rd.l &= ~r.a.l;
}

auto WDC65816::algorithmTRB16() -> void {
  r.p.z = (rd.w & r.a.w) == 0;
  rd.w &= ~r.a.w;
}

auto WDC65816::algorithmTSB8() -> void {
  r.p.z = (rd.l & r.a.l) == 0;
  rd.l |= r.a.l;
}

auto WDC65816::algorithmTSB16() -> void {
  r.p.z = (rd.w & r.a.w) == 0;
  rd.w |= r.a.w;
}
