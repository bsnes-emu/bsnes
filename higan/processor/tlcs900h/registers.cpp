#define PC   r.pc.l.l0

#define CF   r.sr.f.c
#define NF   r.sr.f.n
#define VF   r.sr.f.v
#define HF   r.sr.f.h
#define ZF   r.sr.f.z
#define SF   r.sr.f.s

#define RFP  r.sr.rfp
#define RFPP r.sr.rfpp
#define IFF  r.sr.iff

#define a RFP
#define p RFPP

template<> auto TLCS900H::map<Byte>(Register register) -> maybe<Byte&> {
  switch(register.value) {
  #define r(id, name) case id: return r.name;
  r(0x00, xwa[0].b.b0) r(0x01, xwa[0].b.b1) r(0x02, xwa[0].b.b2) r(0x03, xwa[0].b.b3)
  r(0x04, xbc[0].b.b0) r(0x05, xbc[0].b.b1) r(0x06, xbc[0].b.b2) r(0x07, xbc[0].b.b3)
  r(0x08, xde[0].b.b0) r(0x09, xde[0].b.b1) r(0x0a, xde[0].b.b2) r(0x0b, xde[0].b.b3)
  r(0x0c, xhl[0].b.b0) r(0x0d, xhl[0].b.b1) r(0x0e, xhl[0].b.b2) r(0x0f, xhl[0].b.b3)
  r(0x10, xwa[1].b.b0) r(0x11, xwa[1].b.b1) r(0x12, xwa[1].b.b2) r(0x13, xwa[1].b.b3)
  r(0x14, xbc[1].b.b0) r(0x15, xbc[1].b.b1) r(0x16, xbc[1].b.b2) r(0x17, xbc[1].b.b3)
  r(0x18, xde[1].b.b0) r(0x19, xde[1].b.b1) r(0x1a, xde[1].b.b2) r(0x1b, xde[1].b.b3)
  r(0x1c, xhl[1].b.b0) r(0x1d, xhl[1].b.b1) r(0x1e, xhl[1].b.b2) r(0x1f, xhl[1].b.b3)
  r(0x20, xwa[2].b.b0) r(0x21, xwa[2].b.b1) r(0x22, xwa[2].b.b2) r(0x23, xwa[2].b.b3)
  r(0x24, xbc[2].b.b0) r(0x25, xbc[2].b.b1) r(0x26, xbc[2].b.b2) r(0x27, xbc[2].b.b3)
  r(0x28, xde[2].b.b0) r(0x29, xde[2].b.b1) r(0x2a, xde[2].b.b2) r(0x2b, xde[2].b.b3)
  r(0x2c, xhl[2].b.b0) r(0x2d, xhl[2].b.b1) r(0x2e, xhl[2].b.b2) r(0x2f, xhl[2].b.b3)
  r(0x30, xwa[3].b.b0) r(0x31, xwa[3].b.b1) r(0x32, xwa[3].b.b2) r(0x33, xwa[3].b.b3)
  r(0x34, xbc[3].b.b0) r(0x35, xbc[3].b.b1) r(0x36, xbc[3].b.b2) r(0x37, xbc[3].b.b3)
  r(0x38, xde[3].b.b0) r(0x39, xde[3].b.b1) r(0x3a, xde[3].b.b2) r(0x3b, xde[3].b.b3)
  r(0x3c, xhl[3].b.b0) r(0x3d, xhl[3].b.b1) r(0x3e, xhl[3].b.b2) r(0x3f, xhl[3].b.b3)
  r(0xd0, xwa[p].b.b0) r(0xd1, xwa[p].b.b1) r(0xd2, xwa[p].b.b2) r(0xd3, xwa[p].b.b3)
  r(0xd4, xbc[p].b.b0) r(0xd5, xbc[p].b.b1) r(0xd6, xbc[p].b.b2) r(0xd7, xbc[p].b.b3)
  r(0xd8, xde[p].b.b0) r(0xd9, xde[p].b.b1) r(0xda, xde[p].b.b2) r(0xdb, xde[p].b.b3)
  r(0xdc, xhl[p].b.b0) r(0xdd, xhl[p].b.b1) r(0xde, xhl[p].b.b2) r(0xdf, xhl[p].b.b3)
  r(0xe0, xwa[a].b.b0) r(0xe1, xwa[a].b.b1) r(0xe2, xwa[a].b.b2) r(0xe3, xwa[a].b.b3)
  r(0xe4, xbc[a].b.b0) r(0xe5, xbc[a].b.b1) r(0xe6, xbc[a].b.b2) r(0xe7, xbc[a].b.b3)
  r(0xe8, xde[a].b.b0) r(0xe9, xde[a].b.b1) r(0xea, xde[a].b.b2) r(0xeb, xde[a].b.b3)
  r(0xec, xhl[a].b.b0) r(0xed, xhl[a].b.b1) r(0xee, xhl[a].b.b2) r(0xef, xhl[a].b.b3)
  r(0xf0, xix   .b.b0) r(0xf1, xix   .b.b1) r(0xf2, xix   .b.b2) r(0xf3, xix   .b.b3)
  r(0xf4, xiy   .b.b0) r(0xf5, xiy   .b.b1) r(0xf6, xiy   .b.b2) r(0xf7, xiy   .b.b3)
  r(0xf8, xiz   .b.b0) r(0xf9, xiz   .b.b1) r(0xfa, xiz   .b.b2) r(0xfb, xiz   .b.b3)
  r(0xfc, xsp   .b.b0) r(0xfd, xsp   .b.b1) r(0xfe, xsp   .b.b2) r(0xff, xsp   .b.b3)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::map<Word>(Register register) -> maybe<Word&> {
  switch(register.value & ~1) {
  #define r(id, name) case id: return r.name;
  r(0x00, xwa[0].w.w0) r(0x02, xwa[0].w.w1) r(0x04, xbc[0].w.w0) r(0x06, xbc[0].w.w1)
  r(0x08, xde[0].w.w0) r(0x0a, xde[0].w.w1) r(0x0c, xhl[0].w.w0) r(0x0e, xhl[0].w.w1)
  r(0x10, xwa[1].w.w0) r(0x12, xwa[1].w.w1) r(0x14, xbc[1].w.w0) r(0x16, xbc[1].w.w1)
  r(0x18, xde[1].w.w0) r(0x1a, xde[1].w.w1) r(0x1c, xhl[1].w.w0) r(0x1e, xhl[1].w.w1)
  r(0x20, xwa[2].w.w0) r(0x22, xwa[2].w.w1) r(0x24, xbc[2].w.w0) r(0x26, xbc[2].w.w1)
  r(0x28, xde[2].w.w0) r(0x2a, xde[2].w.w1) r(0x2c, xhl[2].w.w0) r(0x2e, xhl[2].w.w1)
  r(0x30, xwa[3].w.w0) r(0x32, xwa[3].w.w1) r(0x34, xbc[3].w.w0) r(0x36, xbc[3].w.w1)
  r(0x38, xde[3].w.w0) r(0x3a, xde[3].w.w1) r(0x3c, xhl[3].w.w0) r(0x3e, xhl[3].w.w1)
  r(0xd0, xwa[p].w.w0) r(0xd2, xwa[p].w.w1) r(0xd4, xbc[p].w.w0) r(0xd6, xbc[p].w.w1)
  r(0xd8, xde[p].w.w0) r(0xda, xde[p].w.w1) r(0xdc, xhl[p].w.w0) r(0xde, xhl[p].w.w1)
  r(0xe0, xwa[p].w.w0) r(0xe2, xwa[p].w.w1) r(0xe4, xbc[p].w.w0) r(0xe6, xbc[p].w.w1)
  r(0xe8, xde[p].w.w0) r(0xea, xde[p].w.w1) r(0xec, xhl[p].w.w0) r(0xee, xhl[p].w.w1)
  r(0xf0, xix   .w.w0) r(0xf2, xix   .w.w1) r(0xf4, xiy   .w.w0) r(0xf6, xiy   .w.w1)
  r(0xf8, xiz   .w.w0) r(0xfa, xiz   .w.w1) r(0xfc, xsp   .w.w0) r(0xfe, xsp   .w.w0)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::map<Long>(Register register) -> maybe<Long&> {
  switch(register.value & ~3) {
  #define r(id, name) case id: return r.name;
  r(0x00, xwa[0].l.l0) r(0x04, xbc[0].l.l0) r(0x08, xde[0].l.l0) r(0x0c, xhl[0].l.l0)
  r(0x10, xwa[1].l.l0) r(0x14, xbc[1].l.l0) r(0x18, xde[1].l.l0) r(0x1c, xhl[1].l.l0)
  r(0x20, xwa[2].l.l0) r(0x24, xbc[2].l.l0) r(0x28, xde[2].l.l0) r(0x2c, xhl[2].l.l0)
  r(0x30, xwa[3].l.l0) r(0x34, xbc[3].l.l0) r(0x38, xde[3].l.l0) r(0x3c, xhl[3].l.l0)
  r(0xd0, xwa[p].l.l0) r(0xd4, xbc[p].l.l0) r(0xd8, xde[p].l.l0) r(0xdc, xhl[p].l.l0)
  r(0xe0, xwa[a].l.l0) r(0xe4, xbc[a].l.l0) r(0xe8, xde[a].l.l0) r(0xec, xhl[a].l.l0)
  r(0xf0, xix   .l.l0) r(0xf4, xiy   .l.l0) r(0xf8, xiz   .l.l0) r(0xfc, xsp   .l.l0)
  #undef r
  }
  return nothing;
}

#undef a
#undef p

template<> auto TLCS900H::read<Byte>(Register register) -> Byte {
  return map<Byte>(register)(0);
}

template<> auto TLCS900H::read<Word>(Register register) -> Word {
  return map<Word>(register)(0);
}

template<> auto TLCS900H::read<Long>(Register register) -> Long {
  return map<Long>(register)(0);
}

template<> auto TLCS900H::write<Byte>(Register register, Byte data) -> void {
  if(auto r = map<Byte>(register)) r() = data;
}

template<> auto TLCS900H::write<Word>(Register register, Word data) -> void {
  if(auto r = map<Word>(register)) r() = data;
}

template<> auto TLCS900H::write<Long>(Register register, Long data) -> void {
  if(auto r = map<Long>(register)) r() = data;
}

//

//todo: this is pretty hacky ... the templates pass by-value, but we need to modify the status register
//since there's only one, we ignore the parameter and access the underlying register directly instead

template<> auto TLCS900H::read<Word>(StatusRegister) -> Word {
  return r.sr.f.c << 0 | r.sr.f.n << 1 | r.sr.f.v << 2 | r.sr.f.h << 4 | r.sr.f.z << 6 | r.sr.f.s << 7
       | r.sr.rfp << 8 | 1 << 11 | r.sr.iff << 12 | 1 << 15;
}

template<> auto TLCS900H::write<Word>(StatusRegister, Word data) -> void {
  r.sr.f.c = data.bit(0);
  r.sr.f.n = data.bit(1);
  r.sr.f.v = data.bit(2);
  r.sr.f.h = data.bit(4);
  r.sr.f.z = data.bit(6);
  r.sr.f.s = data.bit(7);
  r.sr.rfp = data.bits(8,9);
  r.sr.iff = data.bits(12,14);

  r.sr.rfpp = r.sr.rfp - 1;
}

//todo: the same thing for the flag register

template<> auto TLCS900H::read<Byte>(FlagRegister) -> Byte {
  return r.sr.f.c << 0 | r.sr.f.n << 1 | r.sr.f.v << 2 | r.sr.f.h << 4 | r.sr.f.z << 6 | r.sr.f.s << 7;
}

template<> auto TLCS900H::write<Byte>(FlagRegister, Byte data) -> void {
  r.sr.f.c = data.bit(0);
  r.sr.f.n = data.bit(1);
  r.sr.f.v = data.bit(2);
  r.sr.f.h = data.bit(4);
  r.sr.f.z = data.bit(6);
  r.sr.f.s = data.bit(7);
}
