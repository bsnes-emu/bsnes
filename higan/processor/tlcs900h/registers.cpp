#define PC r.pc.l.l0

#define CF r.sr.f.c
#define NF r.sr.f.n
#define VF r.sr.f.v
#define HF r.sr.f.h
#define ZF r.sr.f.z
#define SF r.sr.f.s

#define RFP  r.sr.rfp
#define RFPP r.sr.rfpp
#define IFF  r.sr.iff

#define a RFP
#define p RFPP

template<> auto TLCS900H::map<Byte>(Register register) -> maybe<uint8&> {
  switch(register.value) {
  #define r(id, name) case id: return r.name;
  r(RA0, xwa[0].b.b0) r(RW0, xwa[0].b.b1) r(QA0, xwa[0].b.b2) r(QW0, xwa[0].b.b3)
  r(RC0, xbc[0].b.b0) r(RB0, xbc[0].b.b1) r(QC0, xbc[0].b.b2) r(QB0, xbc[0].b.b3)
  r(RE0, xde[0].b.b0) r(RD0, xde[0].b.b1) r(QE0, xde[0].b.b2) r(QD0, xde[0].b.b3)
  r(RL0, xhl[0].b.b0) r(RH0, xhl[0].b.b1) r(QL0, xhl[0].b.b2) r(QH0, xhl[0].b.b3)
  r(RA1, xwa[1].b.b0) r(RW1, xwa[1].b.b1) r(QA1, xwa[1].b.b2) r(QW1, xwa[1].b.b3)
  r(RC1, xbc[1].b.b0) r(RB1, xbc[1].b.b1) r(QC1, xbc[1].b.b2) r(QB1, xbc[1].b.b3)
  r(RE1, xde[1].b.b0) r(RD1, xde[1].b.b1) r(QE1, xde[1].b.b2) r(QD1, xde[1].b.b3)
  r(RL1, xhl[1].b.b0) r(RH1, xhl[1].b.b1) r(QL1, xhl[1].b.b2) r(QH1, xhl[1].b.b3)
  r(RA2, xwa[2].b.b0) r(RW2, xwa[2].b.b1) r(QA2, xwa[2].b.b2) r(QW2, xwa[2].b.b3)
  r(RC2, xbc[2].b.b0) r(RB2, xbc[2].b.b1) r(QC2, xbc[2].b.b2) r(QB2, xbc[2].b.b3)
  r(RE2, xde[2].b.b0) r(RD2, xde[2].b.b1) r(QE2, xde[2].b.b2) r(QD2, xde[2].b.b3)
  r(RL2, xhl[2].b.b0) r(RH2, xhl[2].b.b1) r(QL2, xhl[2].b.b2) r(QH2, xhl[2].b.b3)
  r(RA3, xwa[3].b.b0) r(RW3, xwa[3].b.b1) r(QA3, xwa[3].b.b2) r(QW3, xwa[3].b.b3)
  r(RC3, xbc[3].b.b0) r(RB3, xbc[3].b.b1) r(QC3, xbc[3].b.b2) r(QB3, xbc[3].b.b3)
  r(RE3, xde[3].b.b0) r(RD3, xde[3].b.b1) r(QE3, xde[3].b.b2) r(QD3, xde[3].b.b3)
  r(RL3, xhl[3].b.b0) r(RH3, xhl[3].b.b1) r(QL3, xhl[3].b.b2) r(QH3, xhl[3].b.b3)
  r( AP, xwa[p].b.b0) r( WP, xwa[p].b.b1) r(QAP, xwa[p].b.b2) r(QWP, xwa[p].b.b3)
  r( CP, xbc[p].b.b0) r( BP, xbc[p].b.b1) r(QCP, xbc[p].b.b2) r(QBP, xbc[p].b.b3)
  r( EP, xde[p].b.b0) r( DP, xde[p].b.b1) r(QEP, xde[p].b.b2) r(QDP, xde[p].b.b3)
  r( LP, xhl[p].b.b0) r( HP, xhl[p].b.b1) r(QLP, xhl[p].b.b2) r(QHP, xhl[p].b.b3)
  r( A,  xwa[a].b.b0) r( W,  xwa[a].b.b1) r(QA,  xwa[a].b.b2) r(QW,  xwa[a].b.b3)
  r( C,  xbc[a].b.b0) r( B,  xbc[a].b.b1) r(QC,  xbc[a].b.b2) r(QB,  xbc[a].b.b3)
  r( E,  xde[a].b.b0) r( D,  xde[a].b.b1) r(QE,  xde[a].b.b2) r(QD,  xde[a].b.b3)
  r( L,  xhl[a].b.b0) r( H,  xhl[a].b.b1) r(QL,  xhl[a].b.b2) r(QH,  xhl[a].b.b3)
  r(IXL, xix.b.b0) r(IXH, xix.b.b1) r(QIXL, xix.b.b2) r(QIXH, xix.b.b3)
  r(IYL, xiy.b.b0) r(IYH, xiy.b.b1) r(QIYL, xiy.b.b2) r(QIYH, xiy.b.b3)
  r(IZL, xiz.b.b0) r(IZH, xiz.b.b1) r(QIZL, xiz.b.b2) r(QIZH, xiz.b.b3)
  r(SPL, xsp.b.b0) r(SPH, xsp.b.b1) r(QSPL, xsp.b.b2) r(QSPH, xsp.b.b3)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::map<Word>(Register register) -> maybe<uint16&> {
  switch(register.value) {
  #define r(id, name) case id: return r.name;
  r(RWA0, xwa[0].w.w0) r(QWA0, xwa[0].w.w1) r(RBC0, xbc[0].w.w0) r(QBC0, xbc[0].w.w1)
  r(RDE0, xde[0].w.w0) r(QDE0, xde[0].w.w1) r(RHL0, xhl[0].w.w0) r(QHL0, xhl[0].w.w1)
  r(RWA1, xwa[1].w.w0) r(QWA1, xwa[1].w.w1) r(RBC1, xbc[1].w.w0) r(QBC1, xbc[1].w.w1)
  r(RDE1, xde[1].w.w0) r(QDE1, xde[1].w.w1) r(RHL1, xhl[1].w.w0) r(QHL1, xhl[1].w.w1)
  r(RWA2, xwa[2].w.w0) r(QWA2, xwa[2].w.w1) r(RBC2, xbc[2].w.w0) r(QBC2, xbc[2].w.w1)
  r(RDE2, xde[2].w.w0) r(QDE2, xde[2].w.w1) r(RHL2, xhl[2].w.w0) r(QHL2, xhl[2].w.w1)
  r(RWA3, xwa[3].w.w0) r(QWA3, xwa[3].w.w1) r(RBC3, xbc[3].w.w0) r(QBC3, xbc[3].w.w1)
  r(RDE3, xde[3].w.w0) r(QDE3, xde[3].w.w1) r(RHL3, xhl[3].w.w0) r(QHL3, xhl[3].w.w1)
  r( WAP, xwa[p].w.w0) r(QWAP, xwa[p].w.w1) r( BCP, xbc[p].w.w0) r(QBCP, xbc[p].w.w1)
  r( DEP, xde[p].w.w0) r(QDEP, xde[p].w.w1) r( HLP, xhl[p].w.w0) r(QHLP, xhl[p].w.w1)
  r( WA,  xwa[p].w.w0) r(QWA, xwa[p].w.w1) r( BC,  xbc[p].w.w0) r(QBC, xbc[p].w.w1)
  r( DE,  xde[p].w.w0) r(QDE, xde[p].w.w1) r( HL,  xhl[p].w.w0) r(QHL, xhl[p].w.w1)
  r(IX, xix.w.w0) r(QIX, xix.w.w1) r(IY, xiy.w.w0) r(QIY, xiy.w.w1)
  r(IZ, xiz.w.w0) r(QIZ, xiz.w.w1) r(SP, xsp.w.w0) r(QSP, xsp.w.w0)
  #undef r
  }
  return nothing;
}

template<> auto TLCS900H::map<Long>(Register register) -> maybe<uint32&> {
  switch(register.value) {
  #define r(id, name) case id: return r.name;
  r(XWA0, xwa[0].l.l0) r(XBC0, xbc[0].l.l0) r(XDE0, xde[0].l.l0) r(XHL0, xhl[0].l.l0)
  r(XWA1, xwa[1].l.l0) r(XBC1, xbc[1].l.l0) r(XDE1, xde[1].l.l0) r(XHL1, xhl[1].l.l0)
  r(XWA2, xwa[2].l.l0) r(XBC2, xbc[2].l.l0) r(XDE2, xde[2].l.l0) r(XHL2, xhl[2].l.l0)
  r(XWA3, xwa[3].l.l0) r(XBC3, xbc[3].l.l0) r(XDE3, xde[3].l.l0) r(XHL3, xhl[3].l.l0)
  r(XWAP, xwa[p].l.l0) r(XBCP, xbc[p].l.l0) r(XDEP, xde[p].l.l0) r(XHLP, xhl[p].l.l0)
  r(XWA,  xwa[a].l.l0) r(XBC,  xbc[a].l.l0) r(XDE,  xde[a].l.l0) r(XHL,  xhl[a].l.l0)
  r(XIX,  xix.l.l0) r(XIY,  xiy.l.l0) r(XIZ,  xiz.l.l0) r(XSP,  xsp.l.l0)
  #undef r
  }
  return nothing;
}

#undef a
#undef p

template<> auto TLCS900H::read<Byte>(Register register) -> uint8 {
  return map<Byte>(register)(0);
}

template<> auto TLCS900H::read<Word>(Register register) -> uint16 {
  return map<Word>(register)(0);
}

template<> auto TLCS900H::read<Long>(Register register) -> uint32 {
  return map<Long>(register)(0);
}

template<> auto TLCS900H::write<Byte>(Register register, uint8 data) -> void {
  if(auto r = map<Byte>(register)) r() = data;
}

template<> auto TLCS900H::write<Word>(Register register, uint16 data) -> void {
  if(auto r = map<Word>(register)) r() = data;
}

template<> auto TLCS900H::write<Long>(Register register, uint32 data) -> void {
  if(auto r = map<Word>(register)) r() = data;
}
