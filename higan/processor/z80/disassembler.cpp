auto Z80::disassemble(uint16 pc) -> string {
  string output{hex(pc, 4L), "  "};

  output.append(pad(disassembleOpcode(pc), -18, ' '));

  output.append(
    " AF:", hex(r.af, 4L),
    " BC:", hex(r.bc, 4L),
    " DE:", hex(r.de, 4L),
    " HL:", hex(r.hl, 4L),
    " IX:", hex(r.ix, 4L),
    " IY:", hex(r.iy, 4L),
    " SP:", hex(r.sp, 4L)
  );

  return output;
}

#define X hex(x, 2L)
#define Y hex(y, 2L)
#define Z hex(z, 2L)
#define W hex(y << 8 | x << 0, 4L)
#define R hex(pc + 2 + (int8)x, 4L)

auto Z80::disassembleOpcode(uint16 pc) -> string {
  auto o = read(pc + 0);
  auto x = read(pc + 1);
  auto y = read(pc + 2);
  auto z = read(pc + 3);

  switch(o) {
  case 0x00: return {"nop "};
  case 0x18: return {"jr  $", R};
  case 0x20: return {"jr  nz,$", R};
  case 0x28: return {"jr  z,$", R};
  case 0x30: return {"jr  nc,$", R};
  case 0x38: return {"jr  c,$", R};
  case 0xb8: return {"cp  b"};
  case 0xb9: return {"cp  c"};
  case 0xba: return {"cp  d"};
  case 0xbb: return {"cp  e"};
  case 0xbc: return {"cp  h"};
  case 0xbd: return {"cp  l"};
  case 0xbe: return {"cp  (hl)"};
  case 0xbf: return {"cp  a"};
  case 0xc2: return {"jp  nz,$", W};
  case 0xc3: return {"jp  $", W};
  case 0xca: return {"jp  z,$", W};
  case 0xd2: return {"jp  nc,$", W};
  case 0xda: return {"jp  c,$", W};
  case 0xdb: return {"in  a,($", X, ")"};
  case 0xdd: return disassembleOpcodeDD(pc + 1);
  case 0xe2: return {"jp  po,$", W};
  case 0xe9: return {"jp  hl"};
  case 0xea: return {"jp  pe,$", W};
  case 0xed: return disassembleOpcodeED(pc + 1);
  case 0xf2: return {"jp  p,$", W};
  case 0xf3: return {"di  "};
  case 0xfa: return {"jp  m,$", W};
  case 0xfd: return disassembleOpcodeFD(pc + 1);
  case 0xfe: return {"cp  $", X};
  }

  return {"??? (", hex(o, 2L), " ", X, " ", Y, " ", Z, ")"};
}

auto Z80::disassembleOpcodeDD(uint16 pc) -> string {
  auto o = read(pc + 0);
  auto x = read(pc + 1);
  auto y = read(pc + 2);
  auto z = read(pc + 3);

  switch(o) {
  case 0xe9: return {"jp   ix"};
  }

  return {"??? (dd ", hex(o, 2L), " ", X, " ", Y, ")"};
}

auto Z80::disassembleOpcodeED(uint16 pc) -> string {
  auto o = read(pc + 0);
  auto x = read(pc + 1);
  auto y = read(pc + 2);
  auto z = read(pc + 3);

  switch(o) {
  case 0x40: return {"in   b,(c)"};
  case 0x46: return {"im   0"};
  case 0x48: return {"in   c,(c)"};
  case 0x50: return {"in   d,(c)"};
  case 0x56: return {"im   1"};
  case 0x58: return {"in   e,(c)"};
  case 0x5e: return {"im   2"};
  case 0x60: return {"in   h,(c)"};
  case 0x68: return {"in   l,(c)"};
  case 0x70: return {"in   (c)"};
  case 0x78: return {"in   a,(c)"};
  }

  return {"??? (ed ", hex(o, 2L), " ", X, " ", Y, ")"};
}

auto Z80::disassembleOpcodeFD(uint16 pc) -> string {
  auto o = read(pc + 0);
  auto x = read(pc + 1);
  auto y = read(pc + 2);
  auto z = read(pc + 3);

  switch(o) {
  case 0xe9: return {"jp   iy"};
  }

  return {"??? (fd ", hex(o, 2L), " ", X, " ", Y, ")"};
}

#undef X
#undef Y
#undef Z
#undef W
#undef R
