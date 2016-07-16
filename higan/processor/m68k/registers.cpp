auto M68K::Registers::d(uint3 r) -> uint32& {
  switch(r) {
  case 0: return d0;
  case 1: return d1;
  case 2: return d2;
  case 3: return d3;
  case 4: return d4;
  case 5: return d5;
  case 6: return d6;
  case 7: return d7;
  }
  unreachable;
}

auto M68K::Registers::a(uint3 r) -> uint32& {
  switch(r) {
  case 0: return a0;
  case 1: return a1;
  case 2: return a2;
  case 3: return a3;
  case 4: return a4;
  case 5: return a5;
  case 6: return a6;
  case 7: return s ? ssp : usp;
  }
}
