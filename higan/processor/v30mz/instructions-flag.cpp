//9e  sahf
auto V30MZ::opStoreFlagsAcc() {
  wait(3);
  r.f = (r.f & 0xff00) | r.ah;
}

//9f  lahf
auto V30MZ::opLoadAccFlags() {
  wait(1);
  r.ah = (r.f & 0x00ff);
}

//f5  cmc
auto V30MZ::opComplementCarry() {
  wait(3);
  r.f.c = !r.f.c;
}

auto V30MZ::opClearFlag(uint bit) {
  wait(3);
  r.f &= ~(1 << bit);
}

auto V30MZ::opSetFlag(uint bit) {
  wait(3);
  r.f |= 1 << bit;
  if(bit == r.f.i.bit) state.poll = false;
}
