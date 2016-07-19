template<uint Size> auto M68K::read(Register reg) -> uint32 {
  switch(reg.number) {
  case  0: return clip<Size>(r.d0);
  case  1: return clip<Size>(r.d1);
  case  2: return clip<Size>(r.d2);
  case  3: return clip<Size>(r.d3);
  case  4: return clip<Size>(r.d4);
  case  5: return clip<Size>(r.d5);
  case  6: return clip<Size>(r.d6);
  case  7: return clip<Size>(r.d7);
  case  8: return clip<Size>(r.a0);
  case  9: return clip<Size>(r.a1);
  case 10: return clip<Size>(r.a2);
  case 11: return clip<Size>(r.a3);
  case 12: return clip<Size>(r.a4);
  case 13: return clip<Size>(r.a5);
  case 14: return clip<Size>(r.a6);
  case 15: return r.s ? clip<Size>(r.ssp) : clip<Size>(r.usp);
  }
  unreachable;
}

template<uint Size> auto M68K::write(Register reg, uint32 data) -> void {
  switch(reg.number) {
  case  0: r.d0 = clip<Size>(data); return;
  case  1: r.d1 = clip<Size>(data); return;
  case  2: r.d2 = clip<Size>(data); return;
  case  3: r.d3 = clip<Size>(data); return;
  case  4: r.d4 = clip<Size>(data); return;
  case  5: r.d5 = clip<Size>(data); return;
  case  6: r.d6 = clip<Size>(data); return;
  case  7: r.d7 = clip<Size>(data); return;
  case  8: r.a0 = clip<Size>(data); return;
  case  9: r.a1 = clip<Size>(data); return;
  case 10: r.a2 = clip<Size>(data); return;
  case 11: r.a3 = clip<Size>(data); return;
  case 12: r.a4 = clip<Size>(data); return;
  case 13: r.a5 = clip<Size>(data); return;
  case 14: r.a6 = clip<Size>(data); return;
  case 15: r.s ? r.ssp = clip<Size>(data) : r.usp = clip<Size>(data); return;
  }
}
