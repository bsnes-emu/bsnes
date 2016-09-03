auto Z80::disassemble(uint16 pc) -> string {
  string s;
  s.append(hex(pc, 4L), "  ");

  uint8 prefix = 0x00;
  while(true) {
    auto code = bus->read(pc++);
    if(code == 0xdd || code == 0xfd) {
      prefix = code;
      continue;
    }
    s.append(pad(disassemble__(pc, prefix, code), -18L, ' '));
    break;
  }

  s.append(" AF:", hex(r.af, 4L));
  s.append(" BC:", hex(r.bc, 4L));
  s.append(" DE:", hex(r.de, 4L));
  s.append(" HL:", hex(r.hl, 4L));
  s.append(" IX:", hex(r.ix, 4L));
  s.append(" IY:", hex(r.iy, 4L));
  s.append(" SP:", hex(r.sp, 4L));

  return s;
}

#define H (prefix == 0xdd ? "ixh" : prefix == 0xfd ? "iyh" : "h")
#define L (prefix == 0xdd ? "ixl" : prefix == 0xfd ? "iyl" : "l")
#define HL (prefix == 0xdd ? "ix" : prefix == 0xfd ? "iy" : "hl")
#define W hex(y << 8 | x << 0, 4L)
#define X hex(x, 2L)
#define Y hex(y, 2L)
#define Z hex(z, 2L)
#define R hex(pc + 1 + (int8)x, 4L)

auto Z80::disassemble__(uint16 pc, uint8 prefix, uint8 code) -> string {
  auto x = bus->read(pc + 0);
  auto y = bus->read(pc + 1);
  auto z = bus->read(pc + 2);

  switch(code) {
  case 0x00: return {"nop  "};
  case 0x06: return {"ld   b,$", X};
  case 0x0e: return {"ld   c,$", X};
  case 0x16: return {"ld   d,$", X};
  case 0x18: return {"jr   $", R};
  case 0x1e: return {"ld   e,$", X};
  case 0x20: return {"jr   nz,$", R};
  case 0x26: return {"ld   ", H, ",$", X};
  case 0x28: return {"jr   z,$", R};
  case 0x2e: return {"ld   ", L, ",$", X};
  case 0x30: return {"jr   nc,$", R};
  case 0x36: return {"ld   (", HL, "),$", X};
  case 0x38: return {"jr   c,$", R};
  case 0x3e: return {"ld   a,$", X};
  case 0xc2: return {"jp   nz,$", W};
  case 0xc3: return {"jp   $", W};
  case 0xca: return {"jp   z,$", W};
  case 0xcb: return disassembleCB(++pc, prefix, x);
  case 0xd2: return {"jp   nc,$", W};
  case 0xda: return {"jp   c,$", W};
  case 0xdb: return {"in   a,($", X, ")"};
  case 0xe2: return {"jp   po,$", W};
  case 0xea: return {"jp   pe,$", W};
  case 0xed: return disassembleED(++pc, prefix, x);
  case 0xf2: return {"jp   p,$", W};
  case 0xf3: return {"di   "};
  case 0xfa: return {"jp   m,$", W};
  case 0xfb: return {"ei   "};
  case 0xfe: return {"cp   $", X};
  }

  return {"???: ", hex(code, 2L)};
}

auto Z80::disassembleCB(uint16 pc, uint8 prefix, uint8 code) -> string {
  auto x = bus->read(pc + 0);
  auto y = bus->read(pc + 1);
  auto z = bus->read(pc + 2);

  switch(code) {
  }

  return {"???: cb ", hex(code, 2L)};
}

auto Z80::disassembleED(uint16 pc, uint8 prefix, uint8 code) -> string {
  auto x = bus->read(pc + 0);
  auto y = bus->read(pc + 1);
  auto z = bus->read(pc + 2);

  switch(code) {
  case 0x46: return {"im   0"};
  case 0x56: return {"im   1"};
  case 0x5e: return {"im   2"};
  }

  return {"???: ed ", hex(code, 2L)};
}

#undef H
#undef L
#undef HL
#undef W
#undef X
#undef Y
#undef Z
#undef R
