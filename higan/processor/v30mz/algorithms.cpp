//(0 = odd, 1 = even) number of bits set in value
auto V30MZ::parity(uint16 value) const -> bool {
  value ^= value >> 8;
  value ^= value >> 4;
  value ^= value >> 2;
  value ^= value >> 1;
  return !(value & 1);
}

auto V30MZ::alAnd(Size size, uint16 x, uint16 y) -> uint16 {
  uint16 bits = size == Byte ? 0xff : 0xffff;
  uint16 mask = size == Byte ? 0x80 : 0x8000;
  uint16 result = (x & y) & bits;
  r.f.c = 0;
  r.f.p = parity(result);
  r.f.h = 0;
  r.f.z = result == 0;
  r.f.s = result & mask;
  r.f.v = 0;
  return result;
}
