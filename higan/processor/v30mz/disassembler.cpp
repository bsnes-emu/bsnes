auto V30MZ::disassemble(uint16 cs, uint16 ip, bool registers, bool bytes) -> string {
  string s;
  uint20 ea = cs * 16 + ip;
  vector<uint8> bytesRead;

  auto fetch = [&](bool inc = true) -> uint8 {
    uint8 data = read(cs * 16 + ip);
    if(inc) ip++, bytesRead.append(data);
    return data;
  };
  auto readByte = [&]() -> string {
    uint8 byte = fetch();
    return {"$", hex(byte, 2L)};
  };
  auto readWord = [&]() -> string {
    uint16 word = fetch(); word |= fetch() << 8;
    return {"$", hex(word, 4L)};
  };
  auto readByteSigned = [&]() -> string {
    uint8 byte = fetch();
    return {"$", byte & 0x80 ? "-" : "+", hex(byte & 0x80 ? 256 - byte : byte, 2L)};
  };
  auto readRelativeByte = [&]() -> string {
    uint8 byte = fetch();
    return {"$", hex(ip + (int8)byte, 4L)};
  };
  auto readRelativeWord = [&]() -> string {
    uint16 word = fetch(); word |= fetch() << 8;
    return {"$", hex(ip + (int16)word, 4L)};
  };
  auto readIndirectByte = [&]() -> string {
    return {"[", readWord(), "]"};
  };
  auto readIndirectWord = [&]() -> string {
    return {"{", readWord(), "}"};
  };
  auto readRegByte = [&](bool inc = true) -> string {
    uint8 modRM = fetch(inc);
    static const string reg[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    return reg[(uint3)(modRM >> 3)];
  };
  auto readRegWord = [&](bool inc = true) -> string {
    uint8 modRM = fetch(inc);
    static const string reg[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
    return reg[(uint3)(modRM >> 3)];
  };
  auto readSeg = [&](bool inc = true) -> string {
    uint8 modRM = fetch(inc);
    static const string seg[] = {"es", "cs", "ss", "ds"};
    return seg[(uint2)(modRM >> 3)];
  };
  auto readMemByte = [&](bool inc = true) -> string {
    uint8 modRM = fetch(inc);
    static const string reg[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    if(modRM >= 0xc0) return reg[(uint3)modRM];
    if((modRM & 0xc7) == 0x06) return {"[", readWord(), "]"};
    static const string mem[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
    if((modRM & 0xc0) == 0x40) return {"[", mem[(uint3)modRM], "+", readByte(), "]"};
    if((modRM & 0xc0) == 0x80) return {"[", mem[(uint3)modRM], "+", readWord(), "]"};
    return {"[", mem[(uint3)modRM], "]"};
  };
  auto readMemWord = [&](bool inc = true) -> string {
    uint8 modRM = fetch(inc);
    static const string reg[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
    if(modRM >= 0xc0) return reg[(uint3)modRM];
    if((modRM & 0xc7) == 0x06) return {"{", readWord(), "}"};
    static const string mem[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
    if((modRM & 0xc0) == 0x40) return {"{", mem[(uint3)modRM], "+", readByte(), "}"};
    if((modRM & 0xc0) == 0x80) return {"{", mem[(uint3)modRM], "+", readWord(), "}"};
    return {"{", mem[(uint3)modRM], "}"};
  };
  auto readGroup = [&](uint group) -> string {
    uint8 modRM = fetch(false);
    static const string opcode[4][8] = {
      {"add ", "or  ", "adc ", "sbb ", "and ", "sub ", "xor ", "cmp "},
      {"rol ", "ror ", "rcl ", "rcr ", "shl ", "shr ", "sal ", "sar "},
      {"test", "??? ", "not ", "neg ", "mul ", "imul", "div ", "idiv"},
      {"inc ", "dec ", "??? ", "??? ", "??? ", "??? ", "??? ", "??? "},
    };
    return opcode[group - 1][(uint3)(modRM >> 3)];
  };

  auto opcode = fetch();
  switch(opcode) {
  case 0x00: s = {"add    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x01: s = {"add    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x02: s = {"add    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x03: s = {"add    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x04: s = {"add    al,{0}", format{readByte()}}; break;
  case 0x05: s = {"add    ax,{0}", format{readWord()}}; break;
  case 0x06: s = {"push   es"}; break;
  case 0x07: s = {"pop    es"}; break;
  case 0x08: s = {"or     {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x09: s = {"or     {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x0a: s = {"or     {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x0b: s = {"or     {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x0c: s = {"or     al,{0}", format{readByte()}}; break;
  case 0x0d: s = {"or     ax,{0}", format{readWord()}}; break;
  case 0x0e: s = {"push   cs"}; break;
//case 0x0f:
  case 0x10: s = {"adc    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x11: s = {"adc    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x12: s = {"adc    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x13: s = {"adc    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x14: s = {"adc    al,{0}", format{readByte()}}; break;
  case 0x15: s = {"adc    ax,{0}", format{readWord()}}; break;
  case 0x16: s = {"push   ss"}; break;
  case 0x17: s = {"pop    ss"}; break;
  case 0x18: s = {"sbb    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x19: s = {"sbb    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x1a: s = {"sbb    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x1b: s = {"sbb    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x1c: s = {"sbb    al,{0}", format{readByte()}}; break;
  case 0x1d: s = {"sbb    ax,{0}", format{readWord()}}; break;
  case 0x1e: s = {"push   ds"}; break;
  case 0x1f: s = {"pop    ds"}; break;
  case 0x20: s = {"and    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x21: s = {"and    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x22: s = {"and    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x23: s = {"and    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x24: s = {"and    al,{0}", format{readByte()}}; break;
  case 0x25: s = {"and    ax,{0}", format{readWord()}}; break;
  case 0x26: s = {"es:    "}; break;
  case 0x27: s = {"daa    "}; break;
  case 0x28: s = {"sub    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x29: s = {"sub    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x2a: s = {"sub    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x2b: s = {"sub    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x2c: s = {"sub    al,{0}", format{readByte()}}; break;
  case 0x2d: s = {"sub    ax,{0}", format{readWord()}}; break;
  case 0x2e: s = {"cs:    "}; break;
  case 0x2f: s = {"das    "}; break;
  case 0x30: s = {"xor    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x31: s = {"xor    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x32: s = {"xor    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x33: s = {"xor    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x34: s = {"xor    al,{0}", format{readByte()}}; break;
  case 0x35: s = {"xor    ax,{0}", format{readWord()}}; break;
  case 0x36: s = {"ss:    "}; break;
  case 0x37: s = {"aaa    "}; break;
  case 0x38: s = {"cmp    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x39: s = {"cmp    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x3a: s = {"cmp    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x3b: s = {"cmp    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x3c: s = {"cmp    al,{0}", format{readByte()}}; break;
  case 0x3d: s = {"cmp    ax,{0}", format{readWord()}}; break;
  case 0x3e: s = {"ds:    "}; break;
  case 0x3f: s = {"aas    "}; break;
  case 0x40: s = {"inc    ax"}; break;
  case 0x41: s = {"inc    cx"}; break;
  case 0x42: s = {"inc    dx"}; break;
  case 0x43: s = {"inc    bx"}; break;
  case 0x44: s = {"inc    sp"}; break;
  case 0x45: s = {"inc    bp"}; break;
  case 0x46: s = {"inc    si"}; break;
  case 0x47: s = {"inc    di"}; break;
  case 0x48: s = {"dec    ax"}; break;
  case 0x49: s = {"dec    cx"}; break;
  case 0x4a: s = {"dec    dx"}; break;
  case 0x4b: s = {"dec    bx"}; break;
  case 0x4c: s = {"dec    sp"}; break;
  case 0x4d: s = {"dec    bp"}; break;
  case 0x4e: s = {"dec    si"}; break;
  case 0x4f: s = {"dec    di"}; break;
  case 0x50: s = {"push   ax"}; break;
  case 0x51: s = {"push   cx"}; break;
  case 0x52: s = {"push   dx"}; break;
  case 0x53: s = {"push   bx"}; break;
  case 0x54: s = {"push   sp"}; break;
  case 0x55: s = {"push   bp"}; break;
  case 0x56: s = {"push   si"}; break;
  case 0x57: s = {"push   di"}; break;
  case 0x58: s = {"pop    ax"}; break;
  case 0x59: s = {"pop    cx"}; break;
  case 0x5a: s = {"pop    dx"}; break;
  case 0x5b: s = {"pop    bx"}; break;
  case 0x5c: s = {"pop    sp"}; break;
  case 0x5d: s = {"pop    bp"}; break;
  case 0x5e: s = {"pop    si"}; break;
  case 0x5f: s = {"pop    di"}; break;
  case 0x60: s = {"pusha  "}; break;
  case 0x61: s = {"popa   "}; break;
  case 0x62: s = {"bound  {0},{1}", format{readRegWord(0), readMemWord()}}; break;
//case 0x63:
//case 0x64:
//case 0x65:
//case 0x66:
//case 0x67:
  case 0x68: s = {"push   {0}", format{readWord()}}; break;
  case 0x69: s = {"imul   {0},{1},{2}", format{readRegWord(0), readMemWord(), readWord()}}; break;
  case 0x6a: s = {"push   {0}", format{readByteSigned()}}; break;
  case 0x6b: s = {"imul   {0},{1},{2}", format{readRegWord(0), readMemWord(), readByteSigned()}}; break;
  case 0x6c: s = {"insb   "}; break;
  case 0x6d: s = {"insw   "}; break;
  case 0x6e: s = {"outsb  "}; break;
  case 0x6f: s = {"outsw  "}; break;
  case 0x70: s = {"jo     {0}", format{readRelativeByte()}}; break;
  case 0x71: s = {"jno    {0}", format{readRelativeByte()}}; break;
  case 0x72: s = {"jc     {0}", format{readRelativeByte()}}; break;
  case 0x73: s = {"jnc    {0}", format{readRelativeByte()}}; break;
  case 0x74: s = {"jz     {0}", format{readRelativeByte()}}; break;
  case 0x75: s = {"jnz    {0}", format{readRelativeByte()}}; break;
  case 0x76: s = {"jcz    {0}", format{readRelativeByte()}}; break;
  case 0x77: s = {"jncz   {0}", format{readRelativeByte()}}; break;
  case 0x78: s = {"js     {0}", format{readRelativeByte()}}; break;
  case 0x79: s = {"jns    {0}", format{readRelativeByte()}}; break;
  case 0x7a: s = {"jp     {0}", format{readRelativeByte()}}; break;
  case 0x7b: s = {"jnp    {0}", format{readRelativeByte()}}; break;
  case 0x7c: s = {"jl     {0}", format{readRelativeByte()}}; break;
  case 0x7d: s = {"jnl    {0}", format{readRelativeByte()}}; break;
  case 0x7e: s = {"jle    {0}", format{readRelativeByte()}}; break;
  case 0x7f: s = {"jnle   {0}", format{readRelativeByte()}}; break;
  case 0x80: s =  {"{0}   {1},{2}", format{readGroup(1), readMemByte(), readByte()}}; break;
  case 0x81: s =  {"{0}   {1},{2}", format{readGroup(1), readMemWord(), readWord()}}; break;
  case 0x82: s =  {"{0}   {1},{2}", format{readGroup(1), readMemByte(), readByteSigned()}}; break;
  case 0x83: s =  {"{0}   {1},{2}", format{readGroup(1), readMemWord(), readByteSigned()}}; break;
  case 0x84: s = {"test   {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x85: s = {"test   {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x86: s = {"xchg   {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x87: s = {"xchg   {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x88: s = {"mov    {0},{1}", format{readMemByte(0), readRegByte()}}; break;
  case 0x89: s = {"mov    {0},{1}", format{readMemWord(0), readRegWord()}}; break;
  case 0x8a: s = {"mov    {0},{1}", format{readRegByte(0), readMemByte()}}; break;
  case 0x8b: s = {"mov    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x8c: s = {"mov    {0},{1}", format{readMemWord(0), readSeg()}}; break;
  case 0x8d: s = {"lea    {0},{1}", format{readRegWord(0), readMemWord()}}; break;
  case 0x8e: s = {"mov    {0},{1}", format{readSeg(0), readMemWord()}}; break;
  case 0x8f: s = {"pop    {0}", format{readMemWord()}}; break;
  case 0x90: s = {"nop    "}; break;
  case 0x91: s = {"xchg   ax,cx"}; break;
  case 0x92: s = {"xchg   ax,dx"}; break;
  case 0x93: s = {"xchg   ax,bx"}; break;
  case 0x94: s = {"xchg   ax,sp"}; break;
  case 0x95: s = {"xchg   ax,bp"}; break;
  case 0x96: s = {"xchg   ax,si"}; break;
  case 0x97: s = {"xchg   ax,di"}; break;
  case 0x98: s = {"cbw    "}; break;
  case 0x99: s = {"cwd    "}; break;
  case 0x9a: s = {"call   {1}:{0}", format{readWord(), readWord()}}; break;
  case 0x9b: s = {"wait   "}; break;
  case 0x9c: s = {"pushf  "}; break;
  case 0x9d: s = {"popf   "}; break;
  case 0x9e: s = {"sahf   "}; break;
  case 0x9f: s = {"lahf   "}; break;
  case 0xa0: s = {"mov    al,{0}", format{readIndirectByte()}}; break;
  case 0xa1: s = {"mov    ax,{0}", format{readIndirectWord()}}; break;
  case 0xa2: s = {"mov    {0},al", format{readIndirectByte()}}; break;
  case 0xa3: s = {"mov    {0},ax", format{readIndirectWord()}}; break;
  case 0xa4: s = {"movsb  "}; break;
  case 0xa5: s = {"movsw  "}; break;
  case 0xa6: s = {"cmpsb  "}; break;
  case 0xa7: s = {"cmpsw  "}; break;
  case 0xa8: s = {"test   al,{0}", format{readByte()}}; break;
  case 0xa9: s = {"test   ax,{0}", format{readWord()}}; break;
  case 0xaa: s = {"stosb  "}; break;
  case 0xab: s = {"stosw  "}; break;
  case 0xac: s = {"lodsb  "}; break;
  case 0xad: s = {"lodsw  "}; break;
  case 0xae: s = {"scasb  "}; break;
  case 0xaf: s = {"scasw  "}; break;
  case 0xb0: s = {"mov    al,{0}", format{readByte()}}; break;
  case 0xb1: s = {"mov    cl,{0}", format{readByte()}}; break;
  case 0xb2: s = {"mov    dl,{0}", format{readByte()}}; break;
  case 0xb3: s = {"mov    bl,{0}", format{readByte()}}; break;
  case 0xb4: s = {"mov    ah,{0}", format{readByte()}}; break;
  case 0xb5: s = {"mov    ch,{0}", format{readByte()}}; break;
  case 0xb6: s = {"mov    dh,{0}", format{readByte()}}; break;
  case 0xb7: s = {"mov    bh,{0}", format{readByte()}}; break;
  case 0xb8: s = {"mov    ax,{0}", format{readWord()}}; break;
  case 0xb9: s = {"mov    cx,{0}", format{readWord()}}; break;
  case 0xba: s = {"mov    dx,{0}", format{readWord()}}; break;
  case 0xbb: s = {"mov    bx,{0}", format{readWord()}}; break;
  case 0xbc: s = {"mov    sp,{0}", format{readWord()}}; break;
  case 0xbd: s = {"mov    bp,{0}", format{readWord()}}; break;
  case 0xbe: s = {"mov    si,{0}", format{readWord()}}; break;
  case 0xbf: s = {"mov    di,{0}", format{readWord()}}; break;
  case 0xc0: s =  {"{0}   {1},{2}", format{readGroup(2), readMemByte(), readByte()}}; break;
  case 0xc1: s =  {"{0}   {1},{2}", format{readGroup(2), readMemWord(), readByte()}}; break;
  case 0xc2: s = {"ret    {0}", format{readWord()}}; break;
  case 0xc3: s = {"ret    "}; break;
  case 0xc4: s = {"les    {0}", format{readMemWord()}}; break;
  case 0xc5: s = {"lds    {0}", format{readMemWord()}}; break;
  case 0xc6: s = {"mov    {0},{1}", format{readMemByte(), readByte()}}; break;
  case 0xc7: s = {"mov    {0},{1}", format{readMemWord(), readWord()}}; break;
  case 0xc8: s = {"enter  {0},{1}", format{readWord(), readByte()}}; break;
  case 0xc9: s = {"leave  "}; break;
  case 0xca: s = {"retf   {0}", format{readWord()}}; break;
  case 0xcb: s = {"retf   "}; break;
  case 0xcc: s = {"int3   "}; break;
  case 0xcd: s = {"int    ", format{readByte()}}; break;
  case 0xce: s = {"into   "}; break;
  case 0xcf: s = {"iret   "}; break;
  case 0xd0: s =  {"{0}   {1},1", format{readGroup(2), readMemByte()}}; break;
  case 0xd1: s =  {"{0}   {1},1", format{readGroup(2), readMemWord()}}; break;
  case 0xd2: s =  {"{0}   {1},cl", format{readGroup(2), readMemByte()}}; break;
  case 0xd3: s =  {"{0}   {1},cl", format{readGroup(2), readMemWord()}}; break;
  case 0xd4: s = {"aam    {0}", format{readByte()}}; break;
  case 0xd5: s = {"aad    {0}", format{readByte()}}; break;
//case 0xd6:
  case 0xd7: s = {"xlat   "}; break;
//case 0xd8:
//case 0xd9:
//case 0xda:
//case 0xdb:
//case 0xdc:
//case 0xdd:
//case 0xde:
//case 0xdf:
  case 0xe0: s = {"loopnz "}; break;
  case 0xe1: s = {"loopz  "}; break;
  case 0xe2: s = {"loop   "}; break;
  case 0xe3: s = {"jcxz   {0}", format{readRelativeByte()}}; break;
  case 0xe4: s = {"in     al,{0}", format{readByte()}}; break;
  case 0xe5: s = {"in     ax,{0}", format{readByte()}}; break;
  case 0xe6: s = {"out    {0},al", format{readByte()}}; break;
  case 0xe7: s = {"out    {0},ax", format{readByte()}}; break;
  case 0xe8: s = {"call   {0}", format{readRelativeWord()}}; break;
  case 0xe9: s = {"jmp    {0}", format{readRelativeWord()}}; break;
  case 0xea: s = {"jmp    {1}:{0}", format{readWord(), readWord()}}; break;
  case 0xeb: s = {"jmp    {0}", format{readRelativeByte()}}; break;
  case 0xec: s = {"in     al,dx"}; break;
  case 0xed: s = {"in     ax,dx"}; break;
  case 0xee: s = {"out    dx,al"}; break;
  case 0xef: s = {"out    dx,ax"}; break;
  case 0xf0: s = {"lock:  "}; break;
//case 0xf1:
  case 0xf2: s = {"repnz: "}; break;
  case 0xf3: s = {"repz:  "}; break;
  case 0xf4: s = {"hlt    "}; break;
  case 0xf5: s = {"cmc    "}; break;
  case 0xf6: s =  {"{0}   {1},{2}", format{readGroup(3), readMemByte(), readByte()}}; break;
  case 0xf7: s =  {"{0}   {1},{2}", format{readGroup(3), readMemWord(), readWord()}}; break;
  case 0xf8: s = {"clc    "}; break;
  case 0xf9: s = {"stc    "}; break;
  case 0xfa: s = {"cli    "}; break;
  case 0xfb: s = {"sti    "}; break;
  case 0xfc: s = {"cld    "}; break;
  case 0xfd: s = {"std    "}; break;
  case 0xfe: s =  {"{0}   {1},{2}", format{readGroup(4), readMemByte(), readByte()}}; break;
  case 0xff: s =  {"{0}   {1},{2}", format{readGroup(4), readMemWord(), readWord()}}; break;
  default:   s = {"???    {0}", format{hex(opcode, 2L)}}; break;
  }
  while(s.size() < 24) s.append(" ");

  string l;
  if(registers) {
    l = {
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
    };
  }

  string b;
  if(bytes) {
    b = "  ";
    while(bytesRead) {
      b.append(hex(bytesRead.takeFirst(), 2L), " ");
    }
    b.rstrip();
  }

  return {hex(ea, 5L), "  ", s, l, b};
}
