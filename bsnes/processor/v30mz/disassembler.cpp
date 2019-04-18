auto V30MZ::disassemble() -> string {
  return disassemble(r.cs, r.ip);
}

auto V30MZ::disassemble(uint16 cs, uint16 ip) -> string {
  //hack: prefixes execute as separate instructions; combine them instead
  static uint32 suppress = 0xffffffff;
  if((cs << 16 | ip) == suppress) return {};

  string output, repeat, prefix;
  output.append(hex(r.cs * 16 + r.ip, 5L), "  ");

  auto read = [&](uint offset) -> uint8 {
    return V30MZ::read(Byte, cs, ip + offset);
  };

  auto modRM = [&](uint offset = 1) -> uint {
    auto modRM = read(offset++);
    if((modRM & 0xc0) == 0x40) offset += 1;
    if((modRM & 0xc0) == 0x80) offset += 2;
    return offset;
  };

  auto instruction = [&](string_view name) -> string {
    if(name.size() >= 7) return name;
    return pad(name, -7);
  };

  auto segment = [&](string_view name) -> string {
    if(prefix) return {prefix, ":"};
    return {name, ":"};
  };

  auto repeatable = [&](string_view opcode) -> string {
    if(repeat) return {pad(string{repeat, ":"}, -7), opcode};
    return {opcode};
  };

  auto segmentRegister = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string seg[] = {"es", "cs", "ss", "ds"};
    return {seg[modRM >> 3 & 2]};
  };

  auto readByte = [&](uint offset) -> string {
    return hex(read(offset), 2L);
  };

  auto immediateByte = [&](uint offset = 1) -> string {
    return {"0x", readByte(offset)};
  };

  auto immediateWord = [&](uint offset = 1) -> string {
    return {"0x", readByte(offset + 1), readByte(offset + 0)};
  };

  auto immediateLong = [&](uint offset = 1) -> string {
    return {"0x", readByte(offset + 3), readByte(offset + 2), ":",
            "0x", readByte(offset + 1), readByte(offset + 0)};
  };

  auto indirectByte = [&](uint offset = 1) -> string {
    return {"[", immediateByte(), "]"};
  };

  auto indirectWord = [&](uint offset = 1) -> string {
    return {"[", immediateWord(), "]"};
  };

  auto relativeByte = [&](uint offset = 1) -> string {
    int8 displacement = read(offset);
    return {"cs:0x", hex(ip + offset + 1 + displacement, 4L)};
  };

  auto relativeWord = [&](uint offset = 1) -> string {
    int16 displacement = read(offset + 1) << 8 | read(offset + 0) << 0;
    return {"cs:0x", hex(ip + offset + 2 + displacement, 4L)};
  };

  auto adjustByte = [&](uint offset = 2) -> string {
    int8 displacement = read(offset);
    if(displacement >= 0) return {"+0x", hex(displacement, 2L)};
    return {"-0x", hex(abs(displacement), 2L)};
  };

  auto adjustWord = [&](uint offset = 2) -> string {
    int16 displacement = read(offset + 1) << 8 | read(offset + 0) << 0;
    if(displacement >= 0) return {"+0x", hex(displacement, 4L)};
    return {"-0x", hex(abs(displacement), 2L)};
  };

  auto registerByte = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string reg[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    return reg[modRM >> 3 & 7];
  };

  auto registerWord = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string reg[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
    return reg[modRM >> 3 & 7];
  };

  auto memoryByte = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    if(modRM >= 0xc0) return registerByte(modRM & 7);
    if((modRM & 0xc7) == 0x06) return {"byte[", segment("ds"), immediateByte(), "]"};
    static const string seg[] = {"ds", "ds", "ss", "ss", "ds", "ds", "ss", "ds"};
    static const string mem[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
    if((modRM & 0xc0) == 0x40) return {"byte[", segment(seg[modRM & 7]), mem[modRM & 7], "+", adjustByte(), "]"};
    if((modRM & 0xc0) == 0x80) return {"byte[", segment(seg[modRM & 7]), mem[modRM & 7], "+", adjustWord(), "]"};
    return {"byte[", segment(seg[modRM & 7]), mem[modRM & 7], "]"};
  };

  auto memoryWord = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    if(modRM >= 0xc0) return registerWord(modRM & 7);
    if((modRM & 0xc7) == 0x06) return {"word[", segment("ds"), immediateWord(), "]"};
    static const string seg[] = {"ds", "ds", "ss", "ss", "ds", "ds", "ss", "ds"};
    static const string mem[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
    if((modRM & 0xc0) == 0x40) return {"word[", segment(seg[modRM & 7]), mem[modRM & 7], adjustByte(), "]"};
    if((modRM & 0xc0) == 0x80) return {"word[", segment(seg[modRM & 7]), mem[modRM & 7], adjustWord(), "]"};
    return {"word[", segment(seg[modRM & 7]), mem[modRM & 7], "]"};
  };

  auto group1 = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string opcode[] = {"add", "or", "adc", "sbb", "and", "sub", "xor", "cmp"};
    return opcode[modRM >> 3 & 7];
  };

  auto group2 = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string opcode[] = {"rol", "ror", "rcl", "rcr", "shl", "shr", "sal", "sar"};
    return opcode[modRM >> 3 & 7];
  };

  auto group3 = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string opcode[] = {"test", "test", "not", "neg", "mul", "imul", "div", "idiv"};
    return opcode[modRM >> 3 & 7];
  };

  auto group4 = [&](uint offset = 1) -> string {
    auto modRM = read(offset);
    static const string opcode[] = {"inc", "dec", "call", "callf", "jmp", "jmpf", "push", "push"};
    return opcode[modRM >> 3 & 7];
  };

  #define op(id, name, ...) case id: \
    output.append(instruction(name), vector<string>{__VA_ARGS__}.merge(",")); \
    break

  auto opcode = read(0);
  for(uint index : range(7)) {
    if(opcode == 0x26) { prefix = "es";    ip++; opcode = read(0); suppress = cs << 16 | ip; continue; }
    if(opcode == 0x2e) { prefix = "cs";    ip++; opcode = read(0); suppress = cs << 16 | ip; continue; }
    if(opcode == 0x36) { prefix = "ss";    ip++; opcode = read(0); suppress = cs << 16 | ip; continue; }
    if(opcode == 0x3e) { prefix = "ds";    ip++; opcode = read(0); suppress = cs << 16 | ip; continue; }
    if(opcode == 0xf2) { repeat = "repnz"; ip++; opcode = read(0); suppress = cs << 16 | ip; continue; }
    if(opcode == 0xf3) { repeat = "repz";  ip++; opcode = read(0); suppress = cs << 16 | ip; continue; }
    break;
  }

  switch(opcode) {
  op(0x00, "add", memoryByte(), registerByte());
  op(0x01, "add", memoryWord(), registerWord());
  op(0x02, "add", registerByte(), memoryByte());
  op(0x03, "add", registerWord(), memoryWord());
  op(0x04, "add", "al", immediateByte());
  op(0x05, "add", "ax", immediateWord());
  op(0x06, "push", "es");
  op(0x07, "pop", "es");
  op(0x08, "or", memoryByte(), registerByte());
  op(0x09, "or", memoryWord(), registerWord());
  op(0x0a, "or", registerByte(), memoryByte());
  op(0x0b, "or", registerWord(), memoryWord());
  op(0x0c, "or", "al", immediateByte());
  op(0x0d, "or", "ax", immediateWord());
  op(0x0e, "push", "cx");
  op(0x0f, "pop", "cs");
  op(0x10, "adc", memoryByte(), registerByte());
  op(0x11, "adc", memoryWord(), registerWord());
  op(0x12, "adc", registerByte(), memoryByte());
  op(0x13, "adc", registerWord(), memoryWord());
  op(0x14, "adc", "al", immediateByte());
  op(0x15, "adc", "ax", immediateWord());
  op(0x16, "push", "ss");
  op(0x17, "pop", "ss");
  op(0x18, "sbb", memoryByte(), registerByte());
  op(0x19, "sbb", memoryWord(), registerWord());
  op(0x1a, "sbb", registerByte(), memoryByte());
  op(0x1b, "sbb", registerWord(), memoryWord());
  op(0x1c, "sbb", "al", immediateByte());
  op(0x1d, "sbb", "ax", immediateWord());
  op(0x1e, "push", "ds");
  op(0x1f, "pop", "ds");
  op(0x20, "and", memoryByte(), registerByte());
  op(0x21, "and", memoryWord(), registerWord());
  op(0x22, "and", registerByte(), memoryByte());
  op(0x23, "and", registerWord(), memoryWord());
  op(0x24, "and", "al", immediateByte());
  op(0x25, "and", "ax", immediateWord());
  op(0x26, "es:");
  op(0x27, "daa");
  op(0x28, "sub", memoryByte(), registerByte());
  op(0x29, "sub", memoryWord(), registerWord());
  op(0x2a, "sub", registerByte(), memoryByte());
  op(0x2b, "sub", registerWord(), memoryWord());
  op(0x2c, "sub", "al", immediateByte());
  op(0x2d, "sub", "ax", immediateWord());
  op(0x2e, "cs:");
  op(0x2f, "das");
  op(0x30, "xor", memoryByte(), registerByte());
  op(0x31, "xor", memoryWord(), registerWord());
  op(0x32, "xor", registerByte(), memoryByte());
  op(0x33, "xor", registerWord(), memoryWord());
  op(0x34, "xor", "al", immediateByte());
  op(0x35, "xor", "ax", immediateWord());
  op(0x36, "ss:");
  op(0x37, "aaa");
  op(0x38, "cmp", memoryByte(), registerByte());
  op(0x39, "cmp", memoryWord(), registerWord());
  op(0x3a, "cmp", registerByte(), memoryByte());
  op(0x3b, "cmp", registerWord(), memoryWord());
  op(0x3c, "cmp", "al", immediateByte());
  op(0x3d, "cmp", "ax", immediateWord());
  op(0x3e, "ds:");
  op(0x3f, "aas");
  op(0x40, "inc", "ax");
  op(0x41, "inc", "cx");
  op(0x42, "inc", "dx");
  op(0x43, "inc", "bx");
  op(0x44, "inc", "sp");
  op(0x45, "inc", "bp");
  op(0x46, "inc", "si");
  op(0x47, "inc", "di");
  op(0x48, "dec", "ax");
  op(0x49, "dec", "cx");
  op(0x4a, "dec", "dx");
  op(0x4b, "dec", "bx");
  op(0x4c, "dec", "sp");
  op(0x4d, "dec", "bp");
  op(0x4e, "dec", "si");
  op(0x4f, "dec", "di");
  op(0x50, "push", "ax");
  op(0x51, "push", "cx");
  op(0x52, "push", "dx");
  op(0x53, "push", "bx");
  op(0x54, "push", "sp");
  op(0x55, "push", "bp");
  op(0x56, "push", "si");
  op(0x57, "push", "di");
  op(0x58, "pop", "ax");
  op(0x59, "pop", "cx");
  op(0x5a, "pop", "dx");
  op(0x5b, "pop", "bx");
  op(0x5c, "pop", "sp");
  op(0x5d, "pop", "bp");
  op(0x5e, "pop", "si");
  op(0x5f, "pop", "di");
  op(0x60, "pusha");
  op(0x61, "popa");
  op(0x62, "bound", registerWord(), memoryWord());
//op(0x63);
//op(0x64);
//op(0x65);
//op(0x66);
//op(0x67);
  op(0x68, "push", immediateWord());
  op(0x69, "imul", registerWord(), memoryWord(), immediateWord(modRM()));
  op(0x6a, "push", adjustByte(1));
  op(0x6b, "imul", registerWord(), memoryWord(), adjustByte(modRM()));
  op(0x6c, repeatable("insb"));
  op(0x6d, repeatable("insw"));
  op(0x6e, repeatable("outsb"));
  op(0x6f, repeatable("outsw"));
  op(0x70, "jo", relativeByte());
  op(0x71, "jno", relativeByte());
  op(0x72, "jb", relativeByte());
  op(0x73, "jnb", relativeByte());
  op(0x74, "jz", relativeByte());
  op(0x75, "jnz", relativeByte());
  op(0x76, "jbe", relativeByte());
  op(0x77, "ja", relativeByte());
  op(0x78, "js", relativeByte());
  op(0x79, "jns", relativeByte());
  op(0x7a, "jpe", relativeByte());
  op(0x7b, "jpo", relativeByte());
  op(0x7c, "jl", relativeByte());
  op(0x7d, "jge", relativeByte());
  op(0x7e, "jle", relativeByte());
  op(0x7f, "jg", relativeByte());
  op(0x80, group1(), memoryByte(), immediateByte(modRM()));
  op(0x81, group1(), memoryWord(), immediateWord(modRM()));
  op(0x82, group1(), memoryByte(), adjustByte(modRM()));
  op(0x83, group1(), memoryWord(), adjustByte(modRM()));
  op(0x84, "test", memoryByte(), registerByte());
  op(0x85, "test", memoryWord(), registerWord());
  op(0x86, "xchg", memoryByte(), registerByte());
  op(0x87, "xchg", memoryWord(), registerWord());
  op(0x88, "mov", memoryByte(), registerByte());
  op(0x89, "mov", memoryWord(), registerWord());
  op(0x8a, "mov", registerByte(), memoryByte());
  op(0x8b, "mov", registerWord(), memoryWord());
  op(0x8c, "mov", memoryWord(), segmentRegister());
  op(0x8d, "lea", registerWord(), memoryWord());
  op(0x8e, "mov", segmentRegister(), memoryWord());
  op(0x8f, "pop", memoryWord());
  op(0x90, "nop");
  op(0x91, "xchg", "ax", "cx");
  op(0x92, "xchg", "ax", "dx");
  op(0x93, "xchg", "ax", "bx");
  op(0x94, "xchg", "ax", "sp");
  op(0x95, "xchg", "ax", "bp");
  op(0x96, "xchg", "ax", "si");
  op(0x97, "xchg", "ax", "di");
  op(0x98, "cbw");
  op(0x99, "cwd");
  op(0x9a, "call", immediateLong());
  op(0x9b, "wait");
  op(0x9c, "pushf");
  op(0x9d, "popf");
  op(0x9e, "sahf");
  op(0x9f, "lahf");
  op(0xa0, "mov", "al", indirectByte());
  op(0xa1, "mov", "ax", indirectWord());
  op(0xa2, "mov", indirectByte(), "al");
  op(0xa3, "mov", indirectWord(), "ax");
  op(0xa4, repeatable("movsb"));
  op(0xa5, repeatable("movsw"));
  op(0xa6, repeatable("cmpsb"));
  op(0xa7, repeatable("cmpsw"));
  op(0xa8, "test", immediateByte());
  op(0xa9, "test", immediateWord());
  op(0xaa, repeatable("stosb"));
  op(0xab, repeatable("stosw"));
  op(0xac, repeatable("lodsb"));
  op(0xad, repeatable("lodsw"));
  op(0xae, repeatable("scasb"));
  op(0xaf, repeatable("scasw"));
  op(0xb0, "mov", "al", immediateByte());
  op(0xb1, "mov", "cl", immediateByte());
  op(0xb2, "mov", "dl", immediateByte());
  op(0xb3, "mov", "bl", immediateByte());
  op(0xb4, "mov", "ah", immediateByte());
  op(0xb5, "mov", "ch", immediateByte());
  op(0xb6, "mov", "dh", immediateByte());
  op(0xb7, "mov", "bh", immediateByte());
  op(0xb8, "mov", "ax", immediateWord());
  op(0xb9, "mov", "cx", immediateWord());
  op(0xba, "mov", "dx", immediateWord());
  op(0xbb, "mov", "bx", immediateWord());
  op(0xbc, "mov", "sp", immediateWord());
  op(0xbd, "mov", "bp", immediateWord());
  op(0xbe, "mov", "si", immediateWord());
  op(0xbf, "mov", "di", immediateWord());
  op(0xc0, group2(), memoryByte(), immediateByte(modRM()));
  op(0xc1, group2(), memoryWord(), immediateByte(modRM()));
  op(0xc2, "ret", immediateWord());
  op(0xc3, "ret");
  op(0xc4, "les", memoryWord());
  op(0xc5, "lds", memoryWord());
  op(0xc6, "mov", memoryByte(), immediateByte(modRM()));
  op(0xc7, "mov", memoryWord(), immediateWord(modRM()));
  op(0xc8, "enter", immediateWord(), immediateByte(3));
  op(0xc9, "leave");
  op(0xca, "retf", immediateWord());
  op(0xcb, "retf");
  op(0xcc, "int", "0x3");
  op(0xcd, "int", immediateByte());
  op(0xce, "into");
  op(0xcf, "iret");
  op(0xd0, group2(), memoryByte(), "1");
  op(0xd1, group2(), memoryWord(), "1");
  op(0xd2, group2(), memoryByte(), "cl");
  op(0xd3, group2(), memoryWord(), "cl");
  op(0xd4, "aam", immediateByte());
  op(0xd5, "aad", immediateByte());
  op(0xd6, "xlat");  //undocumented mirror
  op(0xd7, "xlat");
//op(0xd8);
//op(0xd9);
//op(0xda);
//op(0xdb);
//op(0xdc);
//op(0xdd);
//op(0xde);
//op(0xdf);
  op(0xe0, "loopnz");
  op(0xe1, "loopz");
  op(0xe2, "loop");
  op(0xe3, "jcxz", relativeByte());
  op(0xe4, "in", "al", immediateByte());
  op(0xe5, "in", "ax", immediateWord());
  op(0xe6, "out", immediateByte(), "al");
  op(0xe7, "out", immediateWord(), "ax");
  op(0xe8, "call", relativeWord());
  op(0xe9, "jmp", relativeWord());
  op(0xea, "jmp", immediateLong());
  op(0xeb, "jmp", relativeByte());
  op(0xec, "in", "al", "dx");
  op(0xed, "in", "ax", "dx");
  op(0xee, "out", "dx", "al");
  op(0xef, "out", "dx", "ax");
  op(0xf0, "lock:");
//op(0xf1);
  op(0xf2, "repnz:");
  op(0xf3, "repz:");
  op(0xf4, "hlt");
  op(0xf5, "cmc");
  op(0xf6, group3(), memoryByte(), immediateByte(modRM()));
  op(0xf7, group3(), memoryWord(), immediateWord(modRM()));
  op(0xf8, "clc");
  op(0xf9, "stc");
  op(0xfa, "cli");
  op(0xfb, "sti");
  op(0xfc, "cld");
  op(0xfd, "std");
  op(0xfe, group4(), memoryByte(), immediateByte(modRM()));
  op(0xff, group4(), memoryWord(), immediateWord(modRM()));
  default: output.append("??? ", hex(read(0), 2L)); break;
  }

  #undef op

  output.size(-48);  //todo: determine the minimum value that will never clip here
  output.append(" ",
    " ax:", hex(r.ax, 4L),
    " bx:", hex(r.bx, 4L),
    " cx:", hex(r.cx, 4L),
    " dx:", hex(r.dx, 4L),
    " si:", hex(r.si, 4L),
    " di:", hex(r.di, 4L),
    " bp:", hex(r.bp, 4L),
    " sp:", hex(r.sp, 4L),
    " ip:", hex(r.ip, 4L),
    " cs:", hex(r.cs, 4L),
    " ds:", hex(r.ds, 4L),
    " es:", hex(r.es, 4L),
    " ss:", hex(r.ss, 4L), " ",
    r.f.m ? "M" : "m",
    r.f.v ? "V" : "v",
    r.f.d ? "D" : "d",
    r.f.i ? "I" : "i",
    r.f.b ? "B" : "b",
    r.f.s ? "S" : "s",
    r.f.z ? "Z" : "z",
    r.f.h ? "H" : "h",
    r.f.p ? "P" : "p",
    r.f.c ? "C" : "c"
  );

  return output;
}
