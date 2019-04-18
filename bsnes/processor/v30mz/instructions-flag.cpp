auto V30MZ::instructionStoreFlagsAcc() -> void {
  wait(3);
  r.f = (r.f & 0xff00) | r.ah;
}

auto V30MZ::instructionLoadAccFlags() -> void {
  wait(1);
  r.ah = (r.f & 0x00ff);
}

auto V30MZ::instructionComplementCarry() -> void {
  wait(3);
  r.f.c = !r.f.c;
}

auto V30MZ::instructionClearFlag(uint bit) -> void {
  wait(3);
  r.f &= ~(1 << bit);
}

auto V30MZ::instructionSetFlag(uint bit) -> void {
  wait(3);
  r.f |= 1 << bit;
  if(bit == r.f.i.bit) state.poll = false;
}
