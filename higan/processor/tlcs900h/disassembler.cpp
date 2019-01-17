auto TLCS900H::disassemble() -> string {
  string output;

  auto pc = r.pc.l.l0;
  output.append(hex(pc, 6L), "  ");

  uint8 op[8] = {}, ops = 0;

  auto read8 = [&]() -> uint8 { return op[ops++] = disassembleRead(pc++); };
  auto read16 = [&]() -> uint16 { uint16 data = read8(); return data | read8() << 8; };
  auto read24 = [&]() -> uint32 { uint24 data = read8(); data |= read8() << 8; return data | read8() << 16; };

  enum : uint {
    Undefined,
    Text,
    Implied,
    Immediate,  //#i
    ImmediateByteRegister,  //immediate:byte,register:size
    ControlRegisterRegister,  //controlRegister:size,register:size
    RegisterControlRegister,  //register:size,controlRegister:size
    Register,
    RegisterImmediate,
    RegisterImmediateByte,     //register:size,immediate:byte
    RegisterImmediateWord,     //register:size,immediate:word
    RegisterDisplacementByte,  //register:size,displacement:byte
    RegisterDisplacementWord,  //register:size,displacement:word
    RegisterWordImmediate,  //r16,#is
    RegisterRegisterAlt,  //register:size,registerAlt:size
    RegisterAltRegister,  //registerAlt:size,register:size
    RegisterAltByteRegister,  //registerAlt:byte,register:size
    Address,  //as
    Displacement,
    DisplacementPC,  //(pc+ds)
    ConditionDisplacementPC,  //c,(pc+ds)
    ConditionRegister,  //condition,register:size
    IndirectRegister,  //(r32)
    IndirectRegisterDecrement,  //(-r32)
    IndirectRegisterIncrement,  //(r32+)
    IndirectRegisterRegisterByte,  //(r32+r8)
    IndirectRegisterRegisterWord,  //(r32+r16)
    IndirectRegisterDisplacementByte,  //(r32+d8)
    IndirectRegisterDisplacementWord,  //(r32+d16)
    IndirectAddressByte,  //(a8)
    IndirectAddressWord,  //(a16)
    IndirectAddressLong,  //(a32)
    IndirectImmediateImmediate,  //(#8),#
  };

  enum : uint {
    Byte,
    Word,
    Long,
    Unknown,
  };

  string  name;
  string  text;
  natural mode;
  natural size;
  natural controlRegister;
  natural register;
  natural registerAlt;
  natural immediate;
  natural indirectImmediate;
  natural condition;
  natural address;
  natural addressSize;
  integer displacement;

  boolean opRegister;
  boolean opSourceMemory;
  boolean opTargetMemory;

  switch(read8()) {
  case 0x00: name = "nop"; break;
  case 0x01: break;
  case 0x02: name = "push"; mode = Text; text = "sr"; break;
  case 0x03: name = "pop"; mode = Text; text = "sr"; break;
  case 0x04: break;
  case 0x05: name = "halt"; break;
  case 0x06: name = "ei"; mode = Immediate; size = Byte; immediate = (uint3)read8(); break;
  case 0x07: name = "reti"; break;
  case 0x08:
    name = "ld"; mode = IndirectImmediateImmediate; size = Byte;
    indirectImmediate = read8(); immediate = read8(); break;
  case 0x09: name = "push"; mode = Immediate; size = Byte; immediate = read8(); break;
  case 0x0a:
    name = "ldx"; mode = IndirectImmediateImmediate; size = Byte;
    indirectImmediate = read8(); immediate = read16(); break;
  case 0x0b: name = "pushw"; mode = Immediate; size = Word; immediate = read16(); break;
  case 0x0c: name = "incf"; break;
  case 0x0d: name = "decf"; break;
  case 0x0e: name = "ret"; break;
  case 0x0f: name = "retd"; mode = Displacement; size = Word; displacement = (int16)read16(); break;
  case 0x10: name = "rcf"; break;
  case 0x11: name = "scf"; break;
  case 0x12: name = "ccf"; break;
  case 0x13: name = "zcf"; break;
  case 0x14: name = "push"; mode = Text; text = "a"; break;
  case 0x15: name = "pop"; mode = Text; text = "a"; break;
  case 0x16: name = "ex"; mode = Text; text = "f,f'"; break;
  case 0x17: name = "ldf"; mode = Immediate; size = Byte; immediate = read8(); break;
  case 0x18: name = "push"; mode = Text; text = "f"; break;
  case 0x19: name = "pop"; mode = Text; text = "f"; break;
  case 0x1a: name = "jp"; mode = Address; size = Word; address = read16(); break;
  case 0x1b: name = "jp"; mode = Address; size = Long; address = read24(); break;
  case 0x1c: name = "call"; mode = Address; size = Word; address = read16(); break;
  case 0x1d: name = "call"; mode = Address; size = Long; address = read24(); break;
  case 0x1e: name = "calr"; mode = DisplacementPC; size = Word; displacement = (int16)read16(); break;
  case 0x1f: break;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    name = "ld"; mode = RegisterImmediate; size = Byte;
    register = toRegister3<uint8>(op[0]).id; immediate = read8(); break;
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
    name = "push"; mode = Register; size = Word;
    register = toRegister3<uint16>(op[0]).id; break;
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    name = "ld"; mode = RegisterImmediate; size = Word;
    register = toRegister3<uint16>(op[0]).id; immediate = read16(); break;
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
    name = "push"; mode = Register; size = Long;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    name = "ld"; mode = RegisterImmediate; size = Long;
    register = toRegister3<uint32>(op[0]).id; immediate = read24(); break;
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    name = "pop"; mode = Register; size = Word;
    register = toRegister3<uint16>(op[0]).id; break;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: break;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    name = "pop"; mode = Register; size = Long;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    name = "jr"; mode = ConditionDisplacementPC; size = Byte;
    condition = (uint4)op[0]; displacement = (int8)read8(); break;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    name = "jrl"; mode = ConditionDisplacementPC; size = Word;
    condition = (uint4)op[0]; displacement = (int16)read16(); break;
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    opSourceMemory = true; mode = IndirectRegister; size = Byte;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    opSourceMemory = true; mode = IndirectRegisterDisplacementByte; size = Byte;
    register = toRegister3<uint32>(op[0]).id; displacement = (int8)read8(); break;
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    opSourceMemory = true; mode = IndirectRegister; size = Word;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    opSourceMemory = true; mode = IndirectRegisterDisplacementByte; size = Word;
    register = toRegister3<uint32>(op[0]).id; displacement = (int8)read8(); break;
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    opSourceMemory = true; mode = IndirectRegister; size = Long;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    opSourceMemory = true; mode = IndirectRegisterDisplacementByte; size = Long;
    register = toRegister3<uint32>(op[0]).id; displacement = (int8)read8(); break;
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    opTargetMemory = true; mode = IndirectRegister;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    opTargetMemory = true; mode = IndirectRegisterDisplacementByte;
    register = toRegister3<uint32>(op[0]).id; displacement = (int8)read8(); break;
  case 0xc0: case 0xd0: case 0xe0: case 0xf0:
    opSourceMemory = op[0] < 0xf0; opTargetMemory = !opSourceMemory; mode = IndirectAddressByte; size = op[0] >> 4;
    address = read8(); break;
  case 0xc1: case 0xd1: case 0xe1: case 0xf1:
    opSourceMemory = op[0] < 0xf0; opTargetMemory = !opSourceMemory; mode = IndirectAddressWord; size = op[0] >> 4;
    address = read16(); break;
  case 0xc2: case 0xd2: case 0xe2: case 0xf2:
    opSourceMemory = op[0] < 0xf0; opTargetMemory = !opSourceMemory; mode = IndirectAddressLong; size = op[0] >> 4;
    address = read24(); break;
  case 0xc3: case 0xd3: case 0xe3: case 0xf3:
    opSourceMemory = op[0] < 0xf0; opTargetMemory = !opSourceMemory;
    read8();
    if((op[1] & 3) == 0) { mode = IndirectRegister; register = read8(); }
    if((op[1] & 3) == 1) { mode = IndirectRegisterDisplacementWord; register = read8(); displacement = (int16)read16(); }
    if(op[1] == 0x03) { mode = IndirectRegisterRegisterByte; register = read8(); registerAlt =  read8(); }
    if(op[1] == 0x07) { mode = IndirectRegisterRegisterWord; register = read8(); registerAlt = read16(); }
    break;
  case 0xc4: case 0xd4: case 0xe4: case 0xf4:
    opSourceMemory = op[0] < 0xf0; opTargetMemory = !opSourceMemory; mode = IndirectRegisterDecrement; size = op[0] >> 4;
    register = read8(); break;
  case 0xc5: case 0xd5: case 0xe5: case 0xf5:
    opSourceMemory = op[0] < 0xf0; opTargetMemory = !opSourceMemory; mode = IndirectRegisterIncrement; size = op[0] >> 4;
    register = read8(); break;
  case 0xc6: case 0xd6: case 0xe6: case 0xf6: break;
  case 0xc7: case 0xd7: case 0xe7:
    opRegister = true; mode = Register; size = op[0] >> 4;
    register = read8(); break;
  case 0xf7:
    name = "ldx"; mode = IndirectImmediateImmediate; size = Byte;
    read8(); indirectImmediate = read8(); read8(); immediate = read8(); read8(); break;
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    opRegister = true; mode = Register; size = Byte;
    register = toRegister3<uint8>(op[0]).id; break;
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    opRegister = true; mode = Register; size = Word;
    register = toRegister3<uint16>(op[0]).id; break;
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    opRegister = true; mode = Register; size = Long;
    register = toRegister3<uint32>(op[0]).id; break;
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    name = "swi"; mode = Immediate; size = Byte;
    immediate = (uint3)op[0]; break;
  }

  auto reads = [&]() -> uint32 {
    if(size == Byte) return  read8();
    if(size == Word) return read16();
    if(size == Long) return read24();
    return 0;
  };

  auto toRegister3s = [&](uint3 data) -> uint8 {
    if(size == Byte) return toRegister3< uint8>(data).id;
    if(size == Word) return toRegister3<uint16>(data).id;
    if(size == Long) return toRegister3<uint32>(data).id;
    return 0;
  };

  //size defined
  if(opRegister)
  switch(read8()) {
  case 0x00: case 0x01: case 0x02: break;
  case 0x03: name = "ld"; mode = RegisterImmediate; immediate = reads(); break;
  case 0x04: name = "push"; mode = Register; break;
  case 0x05: name = "pop"; mode = Register; break;
  case 0x06: name = "cpl"; mode = Register; break;
  case 0x07: name = "neg"; mode = Register; break;
  case 0x08: name = "mul"; mode = RegisterWordImmediate; immediate = reads(); break;
  case 0x09: name = "muls"; mode = RegisterWordImmediate; immediate = reads(); break;
  case 0x0a: name = "div"; mode = RegisterWordImmediate; immediate = reads(); break;
  case 0x0b: name = "divs"; mode = RegisterWordImmediate; immediate = reads(); break;
  case 0x0c: name = "link"; mode = RegisterDisplacementWord; displacement = (int16)read16(); break;
  case 0x0d: name = "unlk"; mode = Register; break;
  case 0x0e: name = "bs1f"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x0f: name = "bs1b"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x10: name = "daa"; mode = Register; break;
  case 0x11: break;
  case 0x12: name = "extz"; mode = Register; break;
  case 0x13: name = "exts"; mode = Register; break;
  case 0x14: name = "paa"; mode = Register; break;
  case 0x15: break;
  case 0x16: name = "mirr"; mode = Register; break;
  case 0x17: case 0x18: break;
  case 0x19: name = "mula"; mode = Register; break;
  case 0x1a: case 0x1b: break;
  case 0x1c: name = "djnz"; mode = RegisterDisplacementByte; displacement = (int8)read8(); break;
  case 0x1d: case 0x1e: case 0x1f: break;
  case 0x20: name = "andcf"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x21: name = "orcf"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x22: name = "xorcf"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x23: name = "ldcf"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x24: name = "stcf"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x25: case 0x26: case 0x27: break;
  case 0x28: name = "andcf"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x29: name = "orcf"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x2a: name = "xorcf"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x2b: name = "ldcf"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x2c: name = "stcf"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0x2d: break;
  case 0x2e: name = "ldc"; mode = ControlRegisterRegister; controlRegister = read8(); break;
  case 0x2f: name = "ldc"; mode = RegisterControlRegister; controlRegister = read8(); break;
  case 0x30: name = "res"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x31: name = "set"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x32: name = "chg"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x33: name = "bit"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x34: name = "tset"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0x35: case 0x36: case 0x37: break;
  case 0x38: name = "minc1"; mode = RegisterImmediateWord; immediate = read16(); break;
  case 0x39: name = "minc2"; mode = RegisterImmediateWord; immediate = read16(); break;
  case 0x3a: name = "minc4"; mode = RegisterImmediateWord; immediate = read16(); break;
  case 0x3b: break;
  case 0x3c: name = "mdec1"; mode = RegisterImmediateWord; immediate = read16(); break;
  case 0x3d: name = "mdec2"; mode = RegisterImmediateWord; immediate = read16(); break;
  case 0x3e: name = "mdec4"; mode = RegisterImmediateWord; immediate = read16(); break;
  case 0x3f: break;
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    name = "mul"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    name = "muls"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    name = "div"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    name = "divs"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    name = "inc"; mode = ImmediateByteRegister; immediate = (uint3)op[1]; if(!immediate) immediate = 8; break;
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    name = "dec"; mode = ImmediateByteRegister; immediate = (uint3)op[1]; if(!immediate) immediate = 8; break;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    name = "scc"; mode = ConditionRegister; condition = (uint4)op[1]; break;
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    name = "add"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    name = "ld"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    name = "adc"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    name = "ld"; mode = RegisterRegisterAlt; registerAlt = toRegister3s(op[1]); break;
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    name = "sub"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    name = "ld"; mode = RegisterImmediateByte; immediate = (uint3)op[1]; break;
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    name = "sbb"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    name = "ex"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    name = "and"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xc8: name = "add"; mode = RegisterImmediate; immediate = reads(); break;
  case 0xc9: name = "adc"; mode = RegisterImmediate; immediate = reads(); break;
  case 0xca: name = "sub"; mode = RegisterImmediate; immediate = reads(); break;
  case 0xcb: name = "sbb"; mode = RegisterImmediate; immediate = reads(); break;
  case 0xcc: name = "and"; mode = RegisterImmediate; immediate = reads(); break;
  case 0xcd: name = "xor"; mode = RegisterImmediate; immediate = reads(); break;
  case 0xce: name = "or";  mode = RegisterImmediate; immediate = reads(); break;
  case 0xcf: name = "cp";  mode = RegisterImmediate; immediate = reads(); break;
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    name = "xor"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    name = "cp"; mode = RegisterImmediateByte; immediate = (uint3)op[1]; break;
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    name = "or"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xe8: name = "rlc"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xe9: name = "rrc"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xea: name = "rl";  mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xeb: name = "rr";  mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xec: name = "sla"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xed: name = "sra"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xee: name = "sll"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xef: name = "srl"; mode = RegisterImmediateByte; immediate = (uint4)read8(); break;
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    name = "cp"; mode = RegisterAltRegister; registerAlt = toRegister3s(op[1]); break;
  case 0xf8: name = "rlc"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xf9: name = "rrc"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xfa: name = "rl";  mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xfb: name = "rr";  mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xfc: name = "sla"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xfd: name = "sra"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xfe: name = "sll"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  case 0xff: name = "srl"; mode = RegisterAltByteRegister; registerAlt = A.id; break;
  }

  //size defined
  if(opSourceMemory)
  switch(read8()) {
  case 0x00: case 0x01: case 0x02: case 0x03: break;
  case 0x05: break;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: break;
  case 0x18: break;
  case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: break;
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: break;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: break;
  }

  //size undefined
  if(opTargetMemory)
  switch(read8()) {
  case 0x01: break;
  case 0x03: break;
  case 0x05: break;
  case 0x07: case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: break;
  case 0x10: case 0x11: case 0x12: case 0x13: break;
  case 0x15: break;
  case 0x17: case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: break;
  case 0x2d: case 0x2e: case 0x2f: break;
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: break;
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f: break;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f: break;
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: break;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: break;
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: break;
  }

  output.size(-48);
  output.append("I", r.iff);
  output.append("R", r.rfp);
  output.append(r.s ? "S" : "s");
  output.append(r.z ? "Z" : "z");
  output.append(r.h ? "H" : "h");
  output.append(r.v ? "V" : "v");
  output.append(r.n ? "N" : "n");
  output.append(r.c ? "C" : "c");
  return output;
}
