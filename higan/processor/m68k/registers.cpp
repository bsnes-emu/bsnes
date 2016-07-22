template<uint Size> auto M68K::read(Register reg) -> uint32 {
  return clip<Size>(r.da[reg.number]);
}

template<uint Size> auto M68K::write(Register reg, uint32 data) -> void {
  r.da[reg.number] = (r.da[reg.number] & ~mask<Size>()) | (data & mask<Size>());
}

auto M68K::setSR(uint16 sr) -> void {
  //when entering or exiting supervisor mode; swap SSP and USP into A7
  if(r.sr.bit(13) != sr.bit(13)) swap(r.da[A7], r.sp);
  r.sr = sr;
}
