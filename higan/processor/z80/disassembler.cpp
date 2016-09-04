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

#define op(id, name, ...) case id: return {name, " ", string_vector{__VA_ARGS__}.merge(",")};

#define N   string{"$", hex(byte(), 2L)}
#define IN  string{"(", N, ")"}
#define NN  string{"$", hex(word(), 4L)}
#define INN string{"(", NN, ")"}
#define E   string{"$", hex(branch(), 4L)}

#define H   string{prefix == 0xdd ? "ixh" : prefix == 0xfd ? "iyh" : "h"}
#define L   string{prefix == 0xdd ? "ixl" : prefix == 0xfd ? "iyl" : "l"}
#define HL  string{prefix == 0xdd ? "ix" : prefix == 0xfd ? "iy" : "hl"}
#define IHL string{"(", HL, displace(), ")"}

auto Z80::disassemble__(uint16 pc, uint8 prefix, uint8 code) -> string {
  auto byte = [&] {
    return bus->read(pc++);
  };

  auto word = [&] {
    uint16 data = byte() << 0;
    return data | byte() << 8;
  };

  auto branch = [&] {
    auto d = byte();
    return pc + (int8)d;
  };

  auto displace = [&] {
    if(!prefix) return string{};
    auto d = (int8)byte();
    return d >= 0 ? string{"+$", hex(d, 2L)} : string{"-$", hex(-d, 2L)};
  };

  if(code == 0xcb) return code = byte(), disassembleCB(pc, prefix, code);
  if(code == 0xed) return code = byte(), disassembleED(pc, prefix, code);

  switch(code) {
  op(0x00, "nop ")
  op(0x01, "ld  ", "bc", NN)
  op(0x06, "ld  ", "b", N)
  op(0x0e, "ld  ", "c", N)
  op(0x11, "ld  ", "de", NN)
  op(0x16, "ld  ", "d", N)
  op(0x18, "jr  ", E)
  op(0x1e, "ld  ", "e", N)
  op(0x20, "jr  ", "nz", E)
  op(0x21, "ld  ", HL, NN)
  op(0x26, "ld  ", H, N)
  op(0x28, "jr  ", "z", E)
  op(0x2e, "ld  ", L, N)
  op(0x30, "jr  ", "nc", E)
  op(0x31, "ld  ", "sp", NN)
  op(0x32, "ld  ", INN, "a")
  op(0x36, "ld  ", IHL, N)
  op(0x38, "jr  ", "c", E)
  op(0x3e, "ld  ", "a", N)
  op(0x70, "ld  ", IHL, "b")
  op(0x71, "ld  ", IHL, "c")
  op(0x72, "ld  ", IHL, "d")
  op(0x73, "ld  ", IHL, "e")
  op(0x74, "ld  ", IHL, "h")
  op(0x75, "ld  ", IHL, "l")
  op(0x77, "ld  ", IHL, "a")
  op(0xb8, "cp  ", "b")
  op(0xb9, "cp  ", "c")
  op(0xba, "cp  ", "d")
  op(0xbb, "cp  ", "e")
  op(0xbc, "cp  ", H)
  op(0xbd, "cp  ", L)
  op(0xbe, "cp  ", IHL)
  op(0xbf, "cp  ", "a")
  op(0xc2, "jp  ", "nz", NN)
  op(0xc3, "jp  ", NN)
  op(0xca, "jp  ", "z", NN)
  op(0xcb, "cb: ")
  op(0xd2, "jp  ", "nc", NN)
  op(0xda, "jp  ", "c", NN)
  op(0xdb, "in  ", "a", IN)
  op(0xe2, "jp  ", "po", NN)
  op(0xea, "jp  ", "pe", NN)
  op(0xed, "ed: ")
  op(0xf2, "jp  ", "p", NN)
  op(0xf3, "di  ")
  op(0xfa, "jp  ", "m", NN)
  op(0xfb, "ei  ")
  op(0xfe, "cp  ", N)
  }

  return {"???: ", hex(code, 2L)};
}

auto Z80::disassembleCB(uint16 pc, uint8 prefix, uint8 code) -> string {
  auto read = [&] { return bus->read(pc++); };

  switch(code) {
  }

  return {"???: cb ", hex(code, 2L)};
}

auto Z80::disassembleED(uint16 pc, uint8 prefix, uint8 code) -> string {
  auto read = [&] { return bus->read(pc++); };

  switch(code) {
  op(0x46, "im  ", "0")
  op(0x56, "im  ", "1")
  op(0x5e, "im  ", "2")
  }

  return {"???: ed ", hex(code, 2L)};
}

#undef op

#undef N
#undef IN
#undef NN
#undef INN
#undef E

#undef H
#undef L
#undef HL
#undef IHL
