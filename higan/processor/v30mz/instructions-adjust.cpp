auto V30MZ::instructionDecimalAdjust(bool negate) -> void {
  wait(9);
  uint8 al = r.al;
  if(r.f.h || ((al & 0x0f) > 0x09)) {
    r.al += negate ? -0x06 : 0x06;
    r.f.h = 1;
  }
  if(r.f.c || (al > 0x99)) {
    r.al += negate ? -0x60 : 0x60;
    r.f.c = 1;
  }
  r.f.s = r.al & 0x80;
  r.f.z = r.al == 0;
  r.f.p = parity(r.al);
}

auto V30MZ::instructionAsciiAdjust(bool negate) -> void {
  wait(8);
  if(r.f.h || ((r.al & 0x0f) > 0x09)) {
    r.al += negate ? -0x06 : 0x06;
    r.ah += negate ? -0x01 : 0x01;
    r.f.h = 1;
    r.f.c = 1;
  } else {
    r.f.h = 0;
    r.f.c = 0;
  }
  r.al &= 0x0f;
}

auto V30MZ::instructionAdjustAfterMultiply() -> void {
  wait(16);
  auto imm = fetch();
  if(imm == 0) return interrupt(0);
  //NEC CPUs do not honor the immediate and always use (base) 10
  r.ah = r.al / 10;
  r.al %= imm;
  r.f.p = parity(r.al);
  r.f.s = r.ax & 0x8000;
  r.f.z = r.ax == 0;
}

auto V30MZ::instructionAdjustAfterDivide() -> void {
  wait(5);
  auto imm = fetch();
  //NEC CPUs do not honor the immediate and always use (base) 10
  r.al += r.ah * 10;
  r.ah = 0;
  r.f.p = parity(r.al);
  r.f.s = r.ax & 0x8000;
  r.f.z = r.ax == 0;
}
