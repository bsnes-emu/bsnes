template<> auto TLCS900H::map(ControlRegister<uint8> register) -> maybe<uint8&> {
  switch(register.id) {
  #define r(id, name) case id: return r.name;
  r(0x00, dmas[0].b.b0) r(0x01, dmas[0].b.b1) r(0x02, dmas[0].b.b2) r(0x03, dmas[0].b.b3)
  r(0x04, dmas[1].b.b0) r(0x05, dmas[1].b.b1) r(0x06, dmas[1].b.b2) r(0x07, dmas[1].b.b3)
  r(0x08, dmas[2].b.b0) r(0x09, dmas[2].b.b1) r(0x0a, dmas[2].b.b2) r(0x0b, dmas[2].b.b3)
  r(0x0c, dmas[3].b.b0) r(0x0d, dmas[3].b.b1) r(0x0e, dmas[3].b.b2) r(0x0f, dmas[3].b.b3)
  r(0x10, dmad[0].b.b0) r(0x11, dmad[0].b.b1) r(0x12, dmad[0].b.b2) r(0x13, dmad[0].b.b3)
  r(0x14, dmad[1].b.b0) r(0x15, dmad[1].b.b1) r(0x16, dmad[1].b.b2) r(0x17, dmad[1].b.b3)
  r(0x18, dmad[2].b.b0) r(0x19, dmad[2].b.b1) r(0x1a, dmad[2].b.b2) r(0x1b, dmad[2].b.b3)
  r(0x1c, dmad[3].b.b0) r(0x1d, dmad[3].b.b1) r(0x1e, dmad[3].b.b2) r(0x1f, dmad[3].b.b3)
  r(0x20, dmam[0].b.b0) r(0x21, dmam[0].b.b1) r(0x22, dmam[0].b.b2) r(0x23, dmam[0].b.b3)
  r(0x24, dmam[1].b.b0) r(0x25, dmam[1].b.b1) r(0x26, dmam[1].b.b2) r(0x27, dmam[1].b.b3)
  r(0x28, dmam[2].b.b0) r(0x29, dmam[2].b.b1) r(0x2a, dmam[2].b.b2) r(0x2b, dmam[2].b.b3)
  r(0x2c, dmam[3].b.b0) r(0x2d, dmam[3].b.b1) r(0x2e, dmam[3].b.b3) r(0x2f, dmam[3].b.b3)
  r(0x3c, intnest.b.b0) r(0x3d, intnest.b.b1)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::map(ControlRegister<uint16> register) -> maybe<uint16&> {
  switch(register.id & ~1) {
  #define r(id, name) case id: return r.name;
  r(0x00, dmas[0].w.w0) r(0x02, dmas[0].w.w1)
  r(0x04, dmas[1].w.w0) r(0x06, dmas[1].w.w1)
  r(0x08, dmas[2].w.w0) r(0x0a, dmas[2].w.w1)
  r(0x0c, dmas[3].w.w0) r(0x0e, dmas[3].w.w1)
  r(0x10, dmad[0].w.w0) r(0x12, dmad[0].w.w1)
  r(0x14, dmad[1].w.w0) r(0x16, dmad[1].w.w1)
  r(0x18, dmad[2].w.w0) r(0x1a, dmad[2].w.w1)
  r(0x1c, dmad[3].w.w0) r(0x1e, dmad[3].w.w1)
  r(0x20, dmam[0].w.w0) r(0x22, dmam[0].w.w1)
  r(0x24, dmam[1].w.w0) r(0x26, dmam[1].w.w1)
  r(0x28, dmam[2].w.w0) r(0x2a, dmam[2].w.w1)
  r(0x2c, dmam[3].w.w0) r(0x2e, dmam[3].w.w1)
  r(0x3c, intnest.w.w0)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::map(ControlRegister<uint32> register) -> maybe<uint32&> {
  switch(register.id & ~1) {
  #define r(id, name) case id: return r.name;
  r(0x00, dmas[0].l.l0)
  r(0x04, dmas[1].l.l0)
  r(0x08, dmas[2].l.l0)
  r(0x0c, dmas[3].l.l0)
  r(0x10, dmad[0].l.l0)
  r(0x14, dmad[1].l.l0)
  r(0x18, dmad[2].l.l0)
  r(0x1c, dmad[3].l.l0)
  r(0x20, dmam[0].l.l0)
  r(0x24, dmam[1].l.l0)
  r(0x28, dmam[2].l.l0)
  r(0x2c, dmam[3].l.l0)
  r(0x3c, intnest.l.l0)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::load< uint8>(ControlRegister< uint8> register) ->  uint8 { return map(register)(Undefined); }
template<> auto TLCS900H::load<uint16>(ControlRegister<uint16> register) -> uint16 { return map(register)(Undefined); }
template<> auto TLCS900H::load<uint32>(ControlRegister<uint32> register) -> uint32 { return map(register)(Undefined); }

template<> auto TLCS900H::store<uint8>(ControlRegister<uint8> register, uint32 data) -> void {
  if(auto r = map(register)) r() = data;
}

template<> auto TLCS900H::store<uint16>(ControlRegister<uint16> register, uint32 data) -> void {
  if(auto r = map(register)) r() = data;
}

template<> auto TLCS900H::store<uint32>(ControlRegister<uint32> register, uint32 data) -> void {
  //INTNEST is 16-bit: this isn't the nicest way to handle this, but ...
  if((register.id & ~3) == 0x3c) data = (uint16)data;
  if(auto r = map(register)) r() = data;
}
