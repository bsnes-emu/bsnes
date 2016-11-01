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
#define REL string{"$", hex(branch(), 4L)}

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

#define I "i"
#define R "r"

#define IC "(c)"
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
  op(0x10, "djnz", REL)
  op(0x11, "ld  ", DE, NN)
  op(0x12, "ld  ", IDE, A)
  op(0x13, "inc ", DE)
  op(0x14, "inc ", D)
  op(0x15, "dec ", D)
  op(0x16, "ld  ", E, N)
  op(0x17, "rla ")
  op(0x18, "jr  ", REL)
  op(0x19, "add ", HL, DE)
  op(0x1a, "ld  ", A, IDE)
  op(0x1b, "dec ", DE)
  op(0x1c, "inc ", E)
  op(0x1d, "dec ", E)
  op(0x1e, "ld  ", E, N)
  op(0x1f, "rra ")
  op(0x20, "jr  ", "nz", REL)
  op(0x21, "ld  ", HL, NN)
  op(0x22, "ld  ", INN, HL)
  op(0x23, "inc ", HL)
  op(0x24, "inc ", H)
  op(0x25, "dec ", H)
  op(0x26, "ld  ", H, N)
  op(0x27, "daa ")
  op(0x28, "jr  ", "z", REL)
  op(0x29, "add ", HL, HL)
  op(0x2a, "ld  ", HL, INN)
  op(0x2b, "dec ", HL)
  op(0x2c, "inc ", L)
  op(0x2d, "dec ", L)
  op(0x2e, "ld  ", L, N)
  op(0x2f, "cpl ")
  op(0x30, "jr  ", "nc", REL)
  op(0x31, "ld  ", SP, NN)
  op(0x32, "ld  ", INN, A)
  op(0x33, "inc ", SP)
  op(0x34, "inc ", IHL)
  op(0x35, "dec ", IHL)
  op(0x36, "ld  ", IHL, N)
  op(0x37, "scf ")
  op(0x38, "jr  ", "c", REL)
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
  op(0xc0, "ret ", "nz")
  op(0xc1, "pop ", BC)
  op(0xc2, "jp  ", "nz", NN)
  op(0xc3, "jp  ", NN)
  op(0xc4, "call", "nz", NN)
  op(0xc5, "push", BC)
  op(0xc6, "add ", A, N)
  op(0xc7, "rst ", "0")
  op(0xc8, "ret ", "z")
  op(0xc9, "ret ")
  op(0xca, "jp  ", "z", NN)
  op(0xcb, "cb: ")
  op(0xcc, "call", "z", NN)
  op(0xcd, "call", NN)
  op(0xce, "adc ", A, N)
  op(0xcf, "rst ", "1")
  op(0xd0, "ret ", "nc")
  op(0xd1, "pop ", DE)
  op(0xd2, "jp  ", "nc", NN)
  op(0xd3, "out ", IN, A)
  op(0xd4, "call", "nc", NN)
  op(0xd5, "push", DE)
  op(0xd6, "sub ", A, N)
  op(0xd7, "rst ", "2")
  op(0xd8, "ret ", "c")
  op(0xd9, "exx ")
  op(0xda, "jp  ", "c", NN)
  op(0xdb, "in  ", A, IN)
  op(0xdc, "call", "c", NN)
  op(0xdd, "ix: ")
  op(0xde, "sbc ", A, N)
  op(0xdf, "rst ", "3")
  op(0xe0, "ret ", "po")
  op(0xe1, "pop ", HL)
  op(0xe2, "jp  ", "po", NN)
  op(0xe4, "call", "po", NN)
  op(0xe5, "push", HL)
  op(0xe6, "and ", A, N)
  op(0xe7, "rst ", "4")
  op(0xe8, "ret ", "pe")
  op(0xe9, "jp  ", HL)  //officially jp (hl); but as read is not indirect, use jp hl
  op(0xea, "jp  ", "pe", NN)
  op(0xeb, "ex  ", DE, _HL)
  op(0xec, "call", "pe", NN)
  op(0xed, "ed: ")
  op(0xee, "xor ", A, N)
  op(0xef, "rst ", "5")
  op(0xf0, "ret ", "p")
  op(0xf1, "pop ", AF)
  op(0xf2, "jp  ", "p", NN)
  op(0xf3, "di  ")
  op(0xf4, "call", "p", NN)
  op(0xf5, "push", AF)
  op(0xf6, "or  ", A, N)
  op(0xf7, "rst ", "6")
  op(0xf8, "ret ", "m")
  op(0xf9, "ld  ", SP, HL)
  op(0xfa, "jp  ", "m", NN)
  op(0xfb, "ei  ")
  op(0xfc, "call", "m", NN)
  op(0xfd, "iy: ")
  op(0xfe, "cp  ", A, N)
  op(0xff, "rst ", "7")
  }

  unreachable;
}

auto Z80::disassembleCB(uint16 pc, uint8 prefix, uint8 code) -> string {
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

  switch(code) {
  op(0x00, "rlc ", B)
  op(0x01, "rlc ", C)
  op(0x02, "rlc ", D)
  op(0x03, "rlc ", E)
  op(0x04, "rlc ", H)
  op(0x05, "rlc ", L)
  op(0x06, "rlc ", IHL)
  op(0x07, "rlc ", A)
  op(0x08, "rrc ", B)
  op(0x09, "rrc ", C)
  op(0x0a, "rrc ", D)
  op(0x0b, "rrc ", E)
  op(0x0c, "rrc ", H)
  op(0x0d, "rrc ", L)
  op(0x0e, "rrc ", IHL)
  op(0x0f, "rrc ", A)
  op(0x10, "rl  ", B)
  op(0x11, "rl  ", C)
  op(0x12, "rl  ", D)
  op(0x13, "rl  ", E)
  op(0x14, "rl  ", H)
  op(0x15, "rl  ", L)
  op(0x16, "rl  ", IHL)
  op(0x17, "rl  ", A)
  op(0x18, "rr  ", B)
  op(0x19, "rr  ", C)
  op(0x1a, "rr  ", D)
  op(0x1b, "rr  ", E)
  op(0x1c, "rr  ", H)
  op(0x1d, "rr  ", L)
  op(0x1e, "rr  ", IHL)
  op(0x1f, "rr  ", A)
  op(0x20, "sla ", B)
  op(0x21, "sla ", C)
  op(0x22, "sla ", D)
  op(0x23, "sla ", E)
  op(0x24, "sla ", H)
  op(0x25, "sla ", L)
  op(0x26, "sla ", IHL)
  op(0x27, "sla ", A)
  op(0x28, "sra ", B)
  op(0x29, "sra ", C)
  op(0x2a, "sra ", D)
  op(0x2b, "sra ", E)
  op(0x2c, "sra ", H)
  op(0x2d, "sra ", L)
  op(0x2e, "sra ", IHL)
  op(0x2f, "sra ", A)
  op(0x30, "sll ", B)
  op(0x31, "sll ", C)
  op(0x32, "sll ", D)
  op(0x33, "sll ", E)
  op(0x34, "sll ", H)
  op(0x35, "sll ", L)
  op(0x36, "sll ", IHL)
  op(0x37, "sll ", A)
  op(0x38, "srl ", B)
  op(0x39, "srl ", C)
  op(0x3a, "srl ", D)
  op(0x3b, "srl ", E)
  op(0x3c, "srl ", H)
  op(0x3d, "srl ", L)
  op(0x3e, "srl ", IHL)
  op(0x3f, "srl ", A)
  op(0x40, "bit ", "0,", B)
  op(0x41, "bit ", "0,", C)
  op(0x42, "bit ", "0,", D)
  op(0x43, "bit ", "0,", E)
  op(0x44, "bit ", "0,", H)
  op(0x45, "bit ", "0,", L)
  op(0x46, "bit ", "0,", IHL)
  op(0x47, "bit ", "0,", A)
  op(0x48, "bit ", "1,", B)
  op(0x49, "bit ", "1,", C)
  op(0x4a, "bit ", "1,", D)
  op(0x4b, "bit ", "1,", E)
  op(0x4c, "bit ", "1,", H)
  op(0x4d, "bit ", "1,", L)
  op(0x4e, "bit ", "1,", IHL)
  op(0x4f, "bit ", "1,", A)
  op(0x50, "bit ", "2,", B)
  op(0x51, "bit ", "2,", C)
  op(0x52, "bit ", "2,", D)
  op(0x53, "bit ", "2,", E)
  op(0x54, "bit ", "2,", H)
  op(0x55, "bit ", "2,", L)
  op(0x56, "bit ", "2,", IHL)
  op(0x57, "bit ", "2,", A)
  op(0x58, "bit ", "3,", B)
  op(0x59, "bit ", "3,", C)
  op(0x5a, "bit ", "3,", D)
  op(0x5b, "bit ", "3,", E)
  op(0x5c, "bit ", "3,", H)
  op(0x5d, "bit ", "3,", L)
  op(0x5e, "bit ", "3,", IHL)
  op(0x5f, "bit ", "3,", A)
  op(0x60, "bit ", "4,", B)
  op(0x61, "bit ", "4,", C)
  op(0x62, "bit ", "4,", D)
  op(0x63, "bit ", "4,", E)
  op(0x64, "bit ", "4,", H)
  op(0x65, "bit ", "4,", L)
  op(0x66, "bit ", "4,", IHL)
  op(0x67, "bit ", "4,", A)
  op(0x68, "bit ", "5,", B)
  op(0x69, "bit ", "5,", C)
  op(0x6a, "bit ", "5,", D)
  op(0x6b, "bit ", "5,", E)
  op(0x6c, "bit ", "5,", H)
  op(0x6d, "bit ", "5,", L)
  op(0x6e, "bit ", "5,", IHL)
  op(0x6f, "bit ", "5,", A)
  op(0x70, "bit ", "6,", B)
  op(0x71, "bit ", "6,", C)
  op(0x72, "bit ", "6,", D)
  op(0x73, "bit ", "6,", E)
  op(0x74, "bit ", "6,", H)
  op(0x75, "bit ", "6,", L)
  op(0x76, "bit ", "6,", IHL)
  op(0x77, "bit ", "6,", A)
  op(0x78, "bit ", "7,", B)
  op(0x79, "bit ", "7,", C)
  op(0x7a, "bit ", "7,", D)
  op(0x7b, "bit ", "7,", E)
  op(0x7c, "bit ", "7,", H)
  op(0x7d, "bit ", "7,", L)
  op(0x7e, "bit ", "7,", IHL)
  op(0x7f, "bit ", "7,", A)
  op(0x80, "res ", "0,", B)
  op(0x81, "res ", "0,", C)
  op(0x82, "res ", "0,", D)
  op(0x83, "res ", "0,", E)
  op(0x84, "res ", "0,", H)
  op(0x85, "res ", "0,", L)
  op(0x86, "res ", "0,", IHL)
  op(0x87, "res ", "0,", A)
  op(0x88, "res ", "1,", B)
  op(0x89, "res ", "1,", C)
  op(0x8a, "res ", "1,", D)
  op(0x8b, "res ", "1,", E)
  op(0x8c, "res ", "1,", H)
  op(0x8d, "res ", "1,", L)
  op(0x8e, "res ", "1,", IHL)
  op(0x8f, "res ", "1,", A)
  op(0x90, "res ", "2,", B)
  op(0x91, "res ", "2,", C)
  op(0x92, "res ", "2,", D)
  op(0x93, "res ", "2,", E)
  op(0x94, "res ", "2,", H)
  op(0x95, "res ", "2,", L)
  op(0x96, "res ", "2,", IHL)
  op(0x97, "res ", "2,", A)
  op(0x98, "res ", "3,", B)
  op(0x99, "res ", "3,", C)
  op(0x9a, "res ", "3,", D)
  op(0x9b, "res ", "3,", E)
  op(0x9c, "res ", "3,", H)
  op(0x9d, "res ", "3,", L)
  op(0x9e, "res ", "3,", IHL)
  op(0x9f, "res ", "3,", A)
  op(0xa0, "res ", "4,", B)
  op(0xa1, "res ", "4,", C)
  op(0xa2, "res ", "4,", D)
  op(0xa3, "res ", "4,", E)
  op(0xa4, "res ", "4,", H)
  op(0xa5, "res ", "4,", L)
  op(0xa6, "res ", "4,", IHL)
  op(0xa7, "res ", "4,", A)
  op(0xa8, "res ", "5,", B)
  op(0xa9, "res ", "5,", C)
  op(0xaa, "res ", "5,", D)
  op(0xab, "res ", "5,", E)
  op(0xac, "res ", "5,", H)
  op(0xad, "res ", "5,", L)
  op(0xae, "res ", "5,", IHL)
  op(0xaf, "res ", "5,", A)
  op(0xb0, "res ", "6,", B)
  op(0xb1, "res ", "6,", C)
  op(0xb2, "res ", "6,", D)
  op(0xb3, "res ", "6,", E)
  op(0xb4, "res ", "6,", H)
  op(0xb5, "res ", "6,", L)
  op(0xb6, "res ", "6,", IHL)
  op(0xb7, "res ", "6,", A)
  op(0xb8, "res ", "7,", B)
  op(0xb9, "res ", "7,", C)
  op(0xba, "res ", "7,", D)
  op(0xbb, "res ", "7,", E)
  op(0xbc, "res ", "7,", H)
  op(0xbd, "res ", "7,", L)
  op(0xbe, "res ", "7,", IHL)
  op(0xbf, "res ", "7,", A)
  op(0xc0, "set ", "0,", B)
  op(0xc1, "set ", "0,", C)
  op(0xc2, "set ", "0,", D)
  op(0xc3, "set ", "0,", E)
  op(0xc4, "set ", "0,", H)
  op(0xc5, "set ", "0,", L)
  op(0xc6, "set ", "0,", IHL)
  op(0xc7, "set ", "0,", A)
  op(0xc8, "set ", "1,", B)
  op(0xc9, "set ", "1,", C)
  op(0xca, "set ", "1,", D)
  op(0xcb, "set ", "1,", E)
  op(0xcc, "set ", "1,", H)
  op(0xcd, "set ", "1,", L)
  op(0xce, "set ", "1,", IHL)
  op(0xcf, "set ", "1,", A)
  op(0xd0, "set ", "2,", B)
  op(0xd1, "set ", "2,", C)
  op(0xd2, "set ", "2,", D)
  op(0xd3, "set ", "2,", E)
  op(0xd4, "set ", "2,", H)
  op(0xd5, "set ", "2,", L)
  op(0xd6, "set ", "2,", IHL)
  op(0xd7, "set ", "2,", A)
  op(0xd8, "set ", "3,", B)
  op(0xd9, "set ", "3,", C)
  op(0xda, "set ", "3,", D)
  op(0xdb, "set ", "3,", E)
  op(0xdc, "set ", "3,", H)
  op(0xdd, "set ", "3,", L)
  op(0xde, "set ", "3,", IHL)
  op(0xdf, "set ", "3,", A)
  op(0xe0, "set ", "4,", B)
  op(0xe1, "set ", "4,", C)
  op(0xe2, "set ", "4,", D)
  op(0xe3, "set ", "4,", E)
  op(0xe4, "set ", "4,", H)
  op(0xe5, "set ", "4,", L)
  op(0xe6, "set ", "4,", IHL)
  op(0xe7, "set ", "4,", A)
  op(0xe8, "set ", "5,", B)
  op(0xe9, "set ", "5,", C)
  op(0xea, "set ", "5,", D)
  op(0xeb, "set ", "5,", E)
  op(0xec, "set ", "5,", H)
  op(0xed, "set ", "5,", L)
  op(0xee, "set ", "5,", IHL)
  op(0xef, "set ", "5,", A)
  op(0xf0, "set ", "6,", B)
  op(0xf1, "set ", "6,", C)
  op(0xf2, "set ", "6,", D)
  op(0xf3, "set ", "6,", E)
  op(0xf4, "set ", "6,", H)
  op(0xf5, "set ", "6,", L)
  op(0xf6, "set ", "6,", IHL)
  op(0xf7, "set ", "6,", A)
  op(0xf8, "set ", "7,", B)
  op(0xf9, "set ", "7,", C)
  op(0xfa, "set ", "7,", D)
  op(0xfb, "set ", "7,", E)
  op(0xfc, "set ", "7,", H)
  op(0xfd, "set ", "7,", L)
  op(0xfe, "set ", "7,", IHL)
  op(0xff, "set ", "7,", A)
  }

  unreachable;
}

auto Z80::disassembleED(uint16 pc, uint8 prefix, uint8 code) -> string {
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

  switch(code) {
  op(0x40, "in  ", B, IC)
  op(0x41, "out ", IC, B)
  op(0x42, "sbc ", HL, BC)
  op(0x43, "ld  ", INN, BC)
  op(0x44, "neg ")
  op(0x45, "retn")
  op(0x46, "im  ", "0")
  op(0x47, "ld  ", I, A)
  op(0x48, "in  ", C, IC)
  op(0x49, "out ", IC, C)
  op(0x4a, "adc ", HL, BC)
  op(0x4b, "ld  ", BC, INN)
  op(0x4c, "neg ")
  op(0x4d, "reti")
  op(0x4e, "im  ", "0")
  op(0x4f, "ld  ", R, A)
  op(0x50, "in  ", D, IC)
  op(0x51, "out ", IC, D)
  op(0x52, "sbc ", HL, DE)
  op(0x53, "ld  ", INN, DE)
  op(0x54, "neg ")
  op(0x55, "retn")
  op(0x56, "im  ", "1")
  op(0x57, "ld  ", A, I)
  op(0x58, "in  ", E, IC)
  op(0x59, "out ", IC, E)
  op(0x5a, "adc ", HL, DE)
  op(0x5b, "ld  ", DE, INN)
  op(0x5c, "neg ")
  op(0x5d, "reti")
  op(0x5e, "im  ", "2")
  op(0x5f, "ld  ", A, R)
  op(0x60, "in  ", H, IC)
  op(0x61, "out ", IC, H)
  op(0x62, "sbc ", HL, HL)
  op(0x63, "ld  ", INN, HL)
  op(0x64, "neg ")
  op(0x65, "retn")
  op(0x66, "im  ", "0")
  op(0x67, "rrd ")
  op(0x68, "in  ", L, IC)
  op(0x69, "out ", IC, L)
  op(0x6a, "adc ", HL, HL)
  op(0x6b, "ld  ", HL, INN)
  op(0x6c, "neg ")
  op(0x6d, "reti")
  op(0x6e, "im  ", "0")
  op(0x6f, "rld ")
  op(0x70, "in  ", F, IC)
  op(0x71, "out ", IC, F)
  op(0x72, "sbc ", HL, SP)
  op(0x73, "ld  ", INN, SP)
  op(0x74, "neg ")
  op(0x75, "retn")
  op(0x76, "im  ", "1")
  op(0x77, "nop ")
  op(0x78, "in  ", A, IC)
  op(0x79, "out ", IC, A)
  op(0x7a, "adc ", HL, SP)
  op(0x7b, "ld  ", SP, INN)
  op(0x7c, "neg ")
  op(0x7d, "reti")
  op(0x7e, "im  ", "2")
  op(0x7f, "nop ")
  op(0xa0, "ldi ")
  op(0xa1, "cpi ")
  op(0xa2, "ini ")
  op(0xa3, "outi")
  op(0xa8, "ldd ")
  op(0xa9, "cpd ")
  op(0xaa, "ind ")
  op(0xab, "outd")
  op(0xb0, "ldir")
  op(0xb1, "cpir")
  op(0xb2, "inir")
  op(0xb3, "otir")
  op(0xb8, "lddr")
  op(0xb9, "cpdr")
  op(0xba, "indr")
  op(0xbb, "otdr")
  }

  return {"nop ", "(ed ", hex(code, 2L), ")"};
}

#undef op

#undef N
#undef IN
#undef NN
#undef INN
#undef REL

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

#undef I
#undef R

#undef IC
#undef IBC
#undef IDE
#undef IHL
#undef ISP
