auto Z80::disassemble(uint16 pc) -> string {
  string s;
  s.append(hex(pc, 4L), "  ");

  uint8 prefix = 0x00;
  auto code = bus->read(pc++);
  if(code == 0xdd || code == 0xfd) {
    prefix = code;

    code = bus->read(pc++);
    if(code == 0xdd || code == 0xfd) {
      if(prefix == 0xdd) {
        s.append("ix:");
        goto finish;
      }
      if(prefix == 0xfd) {
        s.append("iy:");
        goto finish;
      }
    }
  }

  s.append(pad(disassemble__(pc, prefix, code), -18L, ' '));

  finish:
  s.append(" AF:", hex(r.af.word, 4L));
  s.append(" BC:", hex(r.bc.word, 4L));
  s.append(" DE:", hex(r.de.word, 4L));
  s.append(" HL:", hex(r.hl.word, 4L));
  s.append(" IX:", hex(r.ix.word, 4L));
  s.append(" IY:", hex(r.iy.word, 4L));
  s.append(" SP:", hex(r.sp, 4L));

  return s;
}

#define op(id, name, ...) case id: return {name, " ", string_vector{__VA_ARGS__}.merge(",")};

#define N   string{"$", hex(byte(), 2L)}
#define IN  string{"(", N, ")"}
#define NN  string{"$", hex(word(), 4L)}
#define INN string{"(", NN, ")"}
#define R   string{"$", hex(branch(), 4L)}

#define A   "a"
#define F   "f"
#define B   "b"
#define C   "c"
#define D   "d"
#define E   "e"
#define H   prefix == 0xdd ? "ixh" : prefix == 0xfd ? "iyh" : "h"
#define L   prefix == 0xdd ? "ixl" : prefix == 0xfd ? "iyl" : "l"
#define _H  "h"
#define _L  "l"
#define _HL "hl"

#define AF  "af"
#define BC  "bc"
#define DE  "de"
#define HL  prefix == 0xdd ? "ix" : prefix == 0xfd ? "iy" : "hl"

#define AF_ "af'"
#define BC_ "bc'"
#define DE_ "de'"
#define HL_ "hl'"

#define SP  "sp"
#define PC  "pc"

#define IBC "(bc)"
#define IDE "(de)"
#define IHL string{"(", HL, displace(), ")"}
#define ISP "(sp)"

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
  op(0x01, "ld  ", BC, NN)
  op(0x02, "ld  ", IBC, A)
  op(0x03, "inc ", BC)
  op(0x04, "inc ", B)
  op(0x05, "dec ", B)
  op(0x06, "ld  ", B, N)
  op(0x07, "rlca")
  op(0x08, "ex  ", AF, AF_)
  op(0x09, "add ", HL, BC)
  op(0x0a, "ld  ", A, IBC)
  op(0x0b, "dec ", BC)
  op(0x0c, "inc ", C)
  op(0x0d, "dec ", C)
  op(0x0e, "ld  ", C, N)
  op(0x0f, "rrca")
  op(0x11, "ld  ", DE, NN)
  op(0x12, "ld  ", IDE, A)
  op(0x13, "inc ", DE)
  op(0x14, "inc ", D)
  op(0x15, "dec ", D)
  op(0x16, "ld  ", E, N)
  op(0x17, "rla ")
  op(0x18, "jr  ", R)
  op(0x19, "add ", HL, DE)
  op(0x1a, "ld  ", A, IDE)
  op(0x1b, "dec ", DE)
  op(0x1c, "inc ", E)
  op(0x1d, "dec ", E)
  op(0x1e, "ld  ", E, N)
  op(0x1f, "rra ")
  op(0x20, "jr  ", "nz", R)
  op(0x21, "ld  ", HL, NN)
  op(0x22, "ld  ", INN, HL)
  op(0x23, "inc ", HL)
  op(0x24, "inc ", H)
  op(0x25, "dec ", H)
  op(0x26, "ld  ", H, N)
  op(0x28, "jr  ", "z", R)
  op(0x29, "add ", HL, HL)
  op(0x2a, "ld  ", HL, INN)
  op(0x2b, "dec ", HL)
  op(0x2c, "inc ", L)
  op(0x2d, "dec ", L)
  op(0x2e, "ld  ", L, N)
  op(0x2f, "cpl ")
  op(0x30, "jr  ", "nc", R)
  op(0x31, "ld  ", SP, NN)
  op(0x32, "ld  ", INN, A)
  op(0x33, "inc ", SP)
  op(0x34, "inc ", IHL)
  op(0x35, "dec ", IHL)
  op(0x36, "ld  ", IHL, N)
  op(0x37, "scf ")
  op(0x38, "jr  ", "c", R)
  op(0x39, "add ", HL, SP)
  op(0x3a, "ld  ", A, INN)
  op(0x3b, "dec ", SP)
  op(0x3c, "inc ", A)
  op(0x3d, "dec ", A)
  op(0x3e, "ld  ", A, N)
  op(0x3f, "ccf ")
  op(0x40, "ld  ", B, B)
  op(0x41, "ld  ", B, C)
  op(0x42, "ld  ", B, D)
  op(0x43, "ld  ", B, E)
  op(0x44, "ld  ", B, H)
  op(0x45, "ld  ", B, L)
  op(0x46, "ld  ", B, IHL)
  op(0x47, "ld  ", B, A)
  op(0x48, "ld  ", C, B)
  op(0x49, "ld  ", C, C)
  op(0x4a, "ld  ", C, D)
  op(0x4b, "ld  ", C, E)
  op(0x4c, "ld  ", C, H)
  op(0x4d, "ld  ", C, L)
  op(0x4e, "ld  ", C, IHL)
  op(0x4f, "ld  ", C, A)
  op(0x50, "ld  ", D, B)
  op(0x51, "ld  ", D, C)
  op(0x52, "ld  ", D, D)
  op(0x53, "ld  ", D, E)
  op(0x54, "ld  ", D, H)
  op(0x55, "ld  ", D, L)
  op(0x56, "ld  ", D, IHL)
  op(0x57, "ld  ", D, A)
  op(0x58, "ld  ", E, B)
  op(0x59, "ld  ", E, C)
  op(0x5a, "ld  ", E, D)
  op(0x5b, "ld  ", E, E)
  op(0x5c, "ld  ", E, H)
  op(0x5d, "ld  ", E, L)
  op(0x5e, "ld  ", E, IHL)
  op(0x5f, "ld  ", E, A)
  op(0x60, "ld  ", H, B)
  op(0x61, "ld  ", H, C)
  op(0x62, "ld  ", H, D)
  op(0x63, "ld  ", H, E)
  op(0x64, "ld  ", H, H)
  op(0x65, "ld  ", H, L)
  op(0x66, "ld  ", _H, IHL)
  op(0x67, "ld  ", H, A)
  op(0x68, "ld  ", L, B)
  op(0x69, "ld  ", L, C)
  op(0x6a, "ld  ", L, D)
  op(0x6b, "ld  ", L, E)
  op(0x6c, "ld  ", L, H)
  op(0x6d, "ld  ", L, L)
  op(0x6e, "ld  ", _L, IHL)
  op(0x6f, "ld  ", L, A)
  op(0x70, "ld  ", IHL, B)
  op(0x71, "ld  ", IHL, C)
  op(0x72, "ld  ", IHL, D)
  op(0x73, "ld  ", IHL, E)
  op(0x74, "ld  ", IHL, _H)
  op(0x75, "ld  ", IHL, _L)
  op(0x76, "halt")
  op(0x77, "ld  ", IHL, A)
  op(0x78, "ld  ", A, B)
  op(0x79, "ld  ", A, C)
  op(0x7a, "ld  ", A, D)
  op(0x7b, "ld  ", A, E)
  op(0x7c, "ld  ", A, H)
  op(0x7d, "ld  ", A, L)
  op(0x7e, "ld  ", A, IHL)
  op(0x7f, "ld  ", A, A)
  op(0x80, "add ", A, B)
  op(0x81, "add ", A, C)
  op(0x82, "add ", A, D)
  op(0x83, "add ", A, E)
  op(0x84, "add ", A, H)
  op(0x85, "add ", A, L)
  op(0x86, "add ", A, IHL)
  op(0x87, "add ", A, A)
  op(0x88, "adc ", A, B)
  op(0x89, "adc ", A, C)
  op(0x8a, "adc ", A, D)
  op(0x8b, "adc ", A, E)
  op(0x8c, "adc ", A, H)
  op(0x8d, "adc ", A, L)
  op(0x8e, "adc ", A, IHL)
  op(0x8f, "adc ", A, A)
  op(0x90, "sub ", A, B)
  op(0x91, "sub ", A, C)
  op(0x92, "sub ", A, D)
  op(0x93, "sub ", A, E)
  op(0x94, "sub ", A, H)
  op(0x95, "sub ", A, L)
  op(0x96, "sub ", A, IHL)
  op(0x97, "sub ", A, A)
  op(0x98, "sbc ", A, B)
  op(0x99, "sbc ", A, C)
  op(0x9a, "sbc ", A, D)
  op(0x9b, "sbc ", A, E)
  op(0x9c, "sbc ", A, H)
  op(0x9d, "sbc ", A, L)
  op(0x9e, "sbc ", A, IHL)
  op(0x9f, "sbc ", A, A)
  op(0xa0, "and ", A, B)
  op(0xa1, "and ", A, C)
  op(0xa2, "and ", A, D)
  op(0xa3, "and ", A, E)
  op(0xa4, "and ", A, H)
  op(0xa5, "and ", A, L)
  op(0xa6, "and ", A, IHL)
  op(0xa7, "and ", A, A)
  op(0xa8, "xor ", A, B)
  op(0xa9, "xor ", A, C)
  op(0xaa, "xor ", A, D)
  op(0xab, "xor ", A, E)
  op(0xac, "xor ", A, H)
  op(0xad, "xor ", A, L)
  op(0xae, "xor ", A, HL)
  op(0xaf, "xor ", A, A)
  op(0xb0, "or  ", A, B)
  op(0xb1, "or  ", A, C)
  op(0xb2, "or  ", A, D)
  op(0xb3, "or  ", A, E)
  op(0xb4, "or  ", A, H)
  op(0xb5, "or  ", A, L)
  op(0xb6, "or  ", A, IHL)
  op(0xb7, "or  ", A, A)
  op(0xb8, "cp  ", A, B)
  op(0xb9, "cp  ", A, C)
  op(0xba, "cp  ", A, D)
  op(0xbb, "cp  ", A, E)
  op(0xbc, "cp  ", A, H)
  op(0xbd, "cp  ", A, L)
  op(0xbe, "cp  ", A, IHL)
  op(0xbf, "cp  ", A, A)
  op(0xc2, "jp  ", "nz", NN)
  op(0xc3, "jp  ", NN)
  op(0xca, "jp  ", "z", NN)
  op(0xcb, "cb: ")
  op(0xd2, "jp  ", "nc", NN)
  op(0xda, "jp  ", "c", NN)
  op(0xdb, "in  ", A, IN)
  op(0xe2, "jp  ", "po", NN)
  op(0xea, "jp  ", "pe", NN)
  op(0xeb, "ex  ", DE, _HL)
  op(0xed, "ed: ")
  op(0xf2, "jp  ", "p", NN)
  op(0xf3, "di  ")
  op(0xfa, "jp  ", "m", NN)
  op(0xfb, "ei  ")
  op(0xfe, "cp  ", A, N)
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
#undef R

#undef A
#undef F
#undef B
#undef C
#undef D
#undef E
#undef H
#undef L
#undef _H
#undef _L
#undef _HL

#undef AF
#undef BC
#undef DE
#undef HL

#undef AF_
#undef BC_
#undef DE_
#undef HL_

#undef SP
#undef PC

#undef IBC
#undef IDE
#undef IHL
#undef ISP
