auto HuC6280::disassemble(uint16 pc_) -> string {
  uint21 pc = mmu(pc_);
  string s{hex((uint)pc.bits(13,20), 2L), ":", hex((uint)pc.bits(0,12), 4L), "  "};

  auto readByte = [&]() -> uint8 {
    return read(pc++);
  };

  auto readWord = [&]() -> uint16 {
    uint16 data = read(pc++);
    return data | read(pc++) << 8;
  };

  auto absolute = [&]() -> string { return {"$", hex(readWord(), 4L)}; };
  auto absoluteX = [&]() -> string { return {"$", hex(readWord(), 4L), ",x"}; };
  auto absoluteY = [&]() -> string { return {"$", hex(readWord(), 4L), ",y"}; };
  auto immediate = [&]() -> string { return {"#$", hex(readByte(), 2L)}; };
  auto indirect = [&]() -> string { return {"($", hex(readByte(), 2L), ")"}; };
  auto indirectX = [&]() -> string { return {"($", hex(readByte(), 2L), ",x)"}; };
  auto indirectY = [&]() -> string { return {"($", hex(readByte(), 2L), "),y"}; };
  auto memory = [&]() -> string { return {"(x),#$", hex(readByte(), 2L)}; };
  auto relative = [&]() -> string { auto displacement = readByte(); return {"$", hex(pc + (int8)displacement, 4L)}; };
  auto zeropage = [&]() -> string { return {"$", hex(readByte(), 2L)}; };
  auto zeropageX = [&]() -> string { return {"$", hex(readByte(), 2L), ",x"}; };
  auto zeropageY = [&]() -> string { return {"$", hex(readByte(), 2L), ",y"}; };

  uint8 opcode = readByte();

  #define op(id, name, ...) case id: o = {name, " ", string_vector{__VA_ARGS__}.merge(",")}; break;
  string o;

  if(T == 1) switch(opcode) {
  op(0x09, "ora", memory())
  op(0x29, "and", memory())
  op(0x49, "eor", memory())
  op(0x69, "adc", memory())
  }

  if(T == 0) switch(opcode) {
  op(0x01, "ora", indirectX())
  op(0x02, "sxy")
  op(0x05, "ora", zeropage())
  op(0x06, "asl", zeropage())
  op(0x08, "php")
  op(0x09, "ora", immediate())
  op(0x0a, "asl")
  op(0x0d, "ora", absolute())
  op(0x0e, "asl", absolute())
  op(0x10, "bpl", relative())
  op(0x11, "ora", indirectY())
  op(0x12, "ora", indirect())
  op(0x15, "ora", zeropageX())
  op(0x16, "asl", zeropageX())
  op(0x18, "clc")
  op(0x19, "ora", absoluteY())
  op(0x1a, "inc")
  op(0x1d, "ora", absoluteX())
  op(0x1e, "asl", absoluteX())
  op(0x21, "and", indirectX())
  op(0x22, "sax")
  op(0x24, "bit", zeropage())
  op(0x25, "and", zeropage())
  op(0x26, "rol", zeropage())
  op(0x28, "plp")
  op(0x29, "and", immediate())
  op(0x2a, "rol")
  op(0x2c, "bit", absolute())
  op(0x2d, "and", absolute())
  op(0x2e, "rol", absolute())
  op(0x30, "bmi", relative())
  op(0x31, "and", indirectY())
  op(0x32, "and", indirect())
  op(0x34, "bit", zeropageX())
  op(0x35, "and", zeropageX())
  op(0x36, "rol", zeropageX())
  op(0x38, "sec")
  op(0x39, "and", absoluteY())
  op(0x3a, "dec")
  op(0x3c, "bit", absoluteX())
  op(0x3d, "and", absoluteX())
  op(0x3e, "rol", absoluteX())
  op(0x41, "eor", indirectX())
  op(0x42, "say")
  op(0x43, "tma", immediate())
  op(0x45, "eor", zeropage())
  op(0x46, "lsr", zeropage())
  op(0x48, "pha")
  op(0x49, "eor", immediate())
  op(0x4a, "lsr")
  op(0x4d, "eor", absolute())
  op(0x4e, "lsr", absolute())
  op(0x50, "bvc", relative())
  op(0x51, "eor", indirectY())
  op(0x52, "eor", indirect())
  op(0x53, "tam", immediate())
  op(0x54, "csl")
  op(0x55, "eor", zeropageX())
  op(0x56, "lsr", zeropageX())
  op(0x58, "cli")
  op(0x59, "eor", absoluteY())
  op(0x5a, "phy")
  op(0x5d, "eor", absoluteX())
  op(0x5e, "lsr", absoluteX())
  op(0x61, "adc", indirectX())
  op(0x62, "cla")
  op(0x64, "stz", zeropage())
  op(0x65, "adc", zeropage())
  op(0x66, "ror", zeropage())
  op(0x68, "pla")
  op(0x69, "adc", immediate())
  op(0x6a, "ror")
  op(0x6d, "adc", absolute())
  op(0x6e, "ror", absolute())
  op(0x70, "bvs", relative())
  op(0x71, "adc", indirectY())
  op(0x72, "adc", indirect())
  op(0x74, "stz", zeropageX())
  op(0x75, "adc", zeropageX())
  op(0x76, "ror", zeropageX())
  op(0x78, "sei")
  op(0x79, "adc", absoluteY())
  op(0x7a, "ply")
  op(0x7d, "adc", absoluteX())
  op(0x7e, "ror", absoluteX())
  op(0x80, "bra", relative())
  op(0x81, "sta", indirectX())
  op(0x82, "clx")
  op(0x84, "sty", zeropage())
  op(0x85, "sta", zeropage())
  op(0x86, "stx", zeropage())
  op(0x88, "dey")
  op(0x89, "bit", immediate())
  op(0x8a, "txa")
  op(0x8c, "sty", absolute())
  op(0x8d, "sta", absolute())
  op(0x8e, "stx", absolute())
  op(0x90, "bcc", relative())
  op(0x91, "sta", indirectY())
  op(0x92, "sta", indirect())
  op(0x94, "sty", zeropageX())
  op(0x95, "sta", zeropageX())
  op(0x96, "stx", zeropageY())
  op(0x98, "tya")
  op(0x99, "sta", absoluteY())
  op(0x9a, "txs")
  op(0x9c, "stz", absolute())
  op(0x9d, "sta", absoluteX())
  op(0x9e, "stz", absoluteX())
  op(0xa0, "ldy", immediate())
  op(0xa1, "lda", indirectX())
  op(0xa2, "ldx", immediate())
  op(0xa4, "ldy", zeropage())
  op(0xa5, "lda", zeropage())
  op(0xa6, "ldx", zeropage())
  op(0xa8, "tay")
  op(0xa9, "lda", immediate())
  op(0xaa, "tax")
  op(0xac, "ldy", absolute())
  op(0xad, "lda", absolute())
  op(0xae, "ldx", absolute())
  op(0xb0, "bcs", relative())
  op(0xb1, "lda", indirectY())
  op(0xb2, "lda", indirect())
  op(0xb4, "ldy", zeropageX())
  op(0xb5, "lda", zeropageX())
  op(0xb6, "ldx", zeropageY())
  op(0xb8, "clv")
  op(0xb9, "lda", absoluteY())
  op(0xba, "tsx")
  op(0xbc, "ldy", absoluteX())
  op(0xbd, "lda", absoluteX())
  op(0xbe, "ldx", absoluteY())
  op(0xc0, "cpy", immediate())
  op(0xc1, "cmp", indirectX())
  op(0xc2, "cly")
  op(0xc5, "cmp", zeropage())
  op(0xc6, "dec", zeropage())
  op(0xc8, "iny")
  op(0xc9, "cmp", immediate())
  op(0xca, "dex")
  op(0xcd, "cmp", absolute())
  op(0xce, "dec", absolute())
  op(0xd0, "bne", relative())
  op(0xd1, "cmp", indirectY())
  op(0xd2, "cmp", indirect())
  op(0xd4, "csh")
  op(0xd5, "cmp", zeropageX())
  op(0xd6, "dec", zeropageX())
  op(0xd8, "cld")
  op(0xd9, "cmp", absoluteY())
  op(0xda, "phx")
  op(0xdd, "cmp", absoluteX())
  op(0xde, "dec", absoluteX())
  op(0xe0, "cpx", immediate())
  op(0xe1, "sbc", indirectX())
  op(0xe5, "sbc", zeropage())
  op(0xe6, "inc", zeropage())
  op(0xe8, "inx")
  op(0xe9, "sbc", immediate())
  op(0xea, "nop")
  op(0xed, "sbc", absolute())
  op(0xee, "inc", absolute())
  op(0xf0, "beq", relative())
  op(0xf1, "sbc", indirectY())
  op(0xf2, "sbc", indirect())
  op(0xf4, "set")
  op(0xf5, "sbc", zeropageX())
  op(0xf6, "inc", zeropageX())
  op(0xf8, "sed")
  op(0xf9, "sbc", absoluteY())
  op(0xfa, "plx")
  op(0xfd, "sbc", absoluteX())
  op(0xfe, "inc", absoluteX())
  }

  if(!o) o = {"??? (", hex(opcode, 2L), ")"};
  s.append(pad(o, -16L, ' '));
  #undef op

  s.append(" A:", hex(r.a, 2L));
  s.append(" X:", hex(r.x, 2L));
  s.append(" Y:", hex(r.y, 2L));
  s.append(" S:", hex(r.s, 2L));
  s.append(" PC:", hex(pc_, 4L));
  s.append(" ");
  s.append(r.p.n ? "N" : "n");
  s.append(r.p.v ? "V" : "v");
  s.append(r.p.t ? "T" : "t");
  s.append(r.p.b ? "B" : "b");
  s.append(r.p.d ? "D" : "d");
  s.append(r.p.i ? "I" : "i");
  s.append(r.p.z ? "Z" : "z");
  s.append(r.p.c ? "C" : "c");
  s.append(r.cs == 3 ? "+" : "-");

  return s;
}
