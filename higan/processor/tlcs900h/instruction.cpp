template<> auto TLCS900H::registerLookup<Byte>(uint3 code) -> Register {
  return {0xe0 | code >> 1 << 2 | code & 1 ^ 1};  //W, A, B, C, D, E, H, L
}

template<> auto TLCS900H::registerLookup<Word>(uint3 code) -> Register {
  return {0xe0 | code << 2};  //WA, BC, DE, HL, IX, IY, IZ, SP
}

template<> auto TLCS900H::registerLookup<Long>(uint3 code) -> Register {
  return {0xe0 | code << 2};  //XWA, XBC, XDE, XHL, XIX, XIY, XIZ, XSP
}

auto TLCS900H::instruction() -> void {
  auto data = fetch();
  Register register;
  Memory memory;

  switch(data) {
  case 0x00: return instructionNoOperation();
  case 0x02: return instructionPush<Word>(r.sr);
  case 0x03: return instructionPop<Word>(r.sr);
  case 0x04: return (void)Undefined;
  case 0x09: return instructionPush<Byte>(Immediate{fetch<Byte>()});
  case 0x0b: return instructionPush<Word>(Immediate{fetch<Word>()});
  case 0x12: return instructionComplementCarry();
  case 0x14: return instructionPush<Byte>(A);
  case 0x15: return instructionPop<Byte>(A);
  case 0x18: return instructionPush<Byte>(r.sr.f);
  case 0x19: return instructionPop<Byte>(r.sr.f);
  case 0x1a: return instructionJump<Word>(Immediate{fetch<uint16>()});
  case 0x1b: return instructionJump<Long>(Immediate{fetch<uint24>()});
  case 0x1f: return (void)Undefined;

  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad<Byte>(registerLookup<Byte>(data), Immediate{fetch<Byte>()});

  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
    return instructionPush<Word>(registerLookup<Word>(data));

  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    return instructionLoad<Word>(registerLookup<Word>(data), Immediate{fetch<Word>()});

  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
    return instructionPush<Long>(registerLookup<Long>(data));

  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    return instructionLoad<Long>(registerLookup<Long>(data), Immediate{fetch<Long>()});

  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    return instructionPop<Word>(registerLookup<Word>(data));

  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    return (void)Undefined;

  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    return instructionPop<Long>(registerLookup<Long>(data));

  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    return instructionJumpRelative(data & 15, fetch< int8>());

  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    return instructionJumpRelative(data & 15, fetch<int16>());

  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    memory = {read<Long>(register)};
    return instructionSourceMemory<Byte>(memory);

  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionSourceMemory<Byte>(memory);

  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    memory = {read<Long>(register)};
    return instructionSourceMemory<Word>(memory);

  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionSourceMemory<Word>(memory);

  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    memory = {read<Long>(register)};
    return instructionSourceMemory<Long>(memory);

  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionSourceMemory<Long>(memory);

  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    memory = {read<Long>(register)};
    return instructionTargetMemory(memory);

  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    memory = {read<Long>(register) + (int8)fetch()};
    return instructionTargetMemory(memory);

  case 0xc0:
    memory = {fetch< uint8>()};
    return instructionSourceMemory<Byte>(memory);
  case 0xc1:
    memory = {fetch<uint16>()};
    return instructionSourceMemory<Byte>(memory);
  case 0xc2:
    memory = {fetch<uint24>()};
    return instructionSourceMemory<Byte>(memory);
  case 0xc3:
    data = fetch();
    memory = {read<Long>(Register{data})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch()});
    return instructionSourceMemory<Byte>(memory);
  case 0xc4:
    data = fetch();
    register = {data};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    if((data & 3) == 3) Undefined;
    memory = {read<Long>(register)};
    return instructionSourceMemory<Byte>(memory);
  case 0xc5:
    data = fetch();
    register = {data};
    memory = {read<Long>(register)};
    instructionSourceMemory<Byte>(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    if((data & 3) == 3) Undefined;
    return;
  case 0xc6:
    return (void)Undefined;
  case 0xc7:
    register = {fetch()};
    return instructionRegister<Byte>(register);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    register = registerLookup<Byte>(data);
    return instructionRegister<Byte>(register);

  case 0xd0:
    memory = {fetch< uint8>()};
    return instructionSourceMemory<Word>(memory);
  case 0xd1:
    memory = {fetch<uint16>()};
    return instructionSourceMemory<Word>(memory);
  case 0xd2:
    memory = {fetch<uint24>()};
    return instructionSourceMemory<Word>(memory);
  case 0xd3:
    data = fetch();
    memory = {read<Long>(Register{data})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch()});
    return instructionSourceMemory<Word>(memory);
  case 0xd4:
    data = fetch();
    register = {data};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    if((data & 3) == 3) Undefined;
    memory = {read<Long>(register)};
    return instructionSourceMemory<Word>(memory);
  case 0xd5:
    data = fetch();
    register = {data};
    memory = {read<Long>(register)};
    instructionSourceMemory<Word>(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    if((data & 3) == 3) Undefined;
    return;
  case 0xd6:
    return (void)Undefined;
  case 0xd7:
    register = {fetch()};
    return instructionRegister<Word>(register);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    register = registerLookup<Word>(data);
    return instructionRegister<Word>(register);

  case 0xe0:
    memory = {fetch< uint8>()};
    return instructionSourceMemory<Long>(memory);
  case 0xe1:
    memory = {fetch<uint16>()};
    return instructionSourceMemory<Long>(memory);
  case 0xe2:
    memory = {fetch<uint24>()};
    return instructionSourceMemory<Long>(memory);
  case 0xe3:
    data = fetch();
    memory = {read<Long>(Register{data})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch()});
    return instructionSourceMemory<Long>(memory);
  case 0xe4:
    data = fetch();
    register = {data};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    if((data & 3) == 3) Undefined;
    memory = {read<Long>(register)};
    return instructionSourceMemory<Long>(memory);
  case 0xe5:
    data = fetch();
    register = {data};
    memory = {read<Long>(register)};
    instructionSourceMemory<Long>(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    if((data & 3) == 3) Undefined;
    return;
  case 0xe6:
    return (void)Undefined;
  case 0xe7:
    register = {fetch()};
    return instructionRegister<Long>(register);
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    register = registerLookup<Long>(data);
    return instructionRegister<Long>(register);

  case 0xf0:
    memory = {fetch< uint8>()};
    return instructionTargetMemory(memory);
  case 0xf1:
    memory = {fetch<uint16>()};
    return instructionTargetMemory(memory);
  case 0xf2:
    memory = {fetch<uint24>()};
    return instructionTargetMemory(memory);
  case 0xf3:
    data = fetch();
    memory = {read<Long>(Register{data})};
    if((data & 3) == 1) memory.value += (int16)fetch<uint16>();
    if((data & 3) == 2) memory.value +=  (int8)read<Byte>(Register{fetch()});
    if((data & 3) == 3) memory.value += (int16)read<Word>(Register{fetch()});
    return instructionTargetMemory(memory);
  case 0xf4:
    data = fetch();
    register = {data};
    if((data & 3) == 0) write<Long>(register, read<Long>(register) - 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) - 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) - 4);
    if((data & 3) == 3) Undefined;
    memory = {read<Long>(register)};
    return instructionTargetMemory(memory);
  case 0xf5:
    data = fetch();
    register = {data};
    memory = {read<Long>(register)};
    instructionTargetMemory(memory);
    if((data & 3) == 0) write<Long>(register, read<Long>(register) + 1);
    if((data & 3) == 1) write<Long>(register, read<Long>(register) + 2);
    if((data & 3) == 2) write<Long>(register, read<Long>(register) + 4);
    if((data & 3) == 3) Undefined;
    return;
  case 0xf6: case 0xf7:
    return (void)Undefined;

  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionSoftwareInterrupt(data.bits(0,2));
  }
}

template<typename Size>
auto TLCS900H::instructionRegister(Register register) -> void {
  auto data = fetch();

  switch(data) {
  case 0x04: return instructionPush<Size>(register);
  case 0x05: return instructionPop<Size>(register);

  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd<Size>(registerLookup<Size>(data), register);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionLoad<Size>(registerLookup<Size>(data), register);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry<Size>(registerLookup<Size>(data), register);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionLoad<Size>(register, registerLookup<Size>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSubtract<Size>(registerLookup<Size>(data), register);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionLoad<Size>(register, Immediate{data.bits(0,2)});
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionSubtractCarry<Size>(registerLookup<Size>(data), register);
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionAnd<Size>(registerLookup<Size>(data), register);
  case 0xc8: return instructionAdd<Size>(register, Immediate{fetch<Size>()});
  case 0xc9: return instructionAddCarry<Size>(register, Immediate{fetch<Size>()});
  case 0xca: return instructionSubtract<Size>(register, Immediate{fetch<Size>()});
  case 0xcb: return instructionSubtractCarry<Size>(register, Immediate{fetch<Size>()});
  case 0xcc: return instructionAnd<Size>(register, Immediate{fetch<Size>()});
  case 0xcd: return instructionXor<Size>(register, Immediate{fetch<Size>()});
  case 0xce: return instructionOr<Size>(register, Immediate{fetch<Size>()});
  case 0xcf: return instructionCompare<Size>(register, Immediate{fetch<Size>()});
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    return instructionXor<Size>(registerLookup<Size>(data), register);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionCompare<Size>(register, Immediate{data.bits(0,2)});
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    return instructionOr<Size>(registerLookup<Size>(data), register);
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    return instructionCompare<Size>(registerLookup<Size>(data), register);
  }
}

template<typename Size>
auto TLCS900H::instructionSourceMemory(Memory memory) -> void {
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02: case 0x03: return (void)Undefined;
  case 0x04: if constexpr(isLong<Size>()) return (void)Undefined;
    return instructionPush<Size>(memory);
  case 0x05: return (void)Undefined;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: return (void)Undefined;
  case 0x18: return (void)Undefined;
  case 0x19: if constexpr(isLong<Size>()) return (void)Undefined;
    return instructionLoad<Size>(Memory{fetch<Size>()}, memory);
  case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad<Size>(registerLookup<Size>(data), memory);
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: return (void)Undefined;
  case 0x38: if constexpr(isLong<Size>()) return (void)Undefined;
    return instructionAdd<Size>(memory, Immediate{fetch<Size>()});
  case 0x39: if constexpr(isLong<Size>()) return (void)Undefined;
    return instructionAddCarry<Size>(memory, Immediate{fetch<Size>()});
  case 0x3a: if constexpr(isLong<Size>()) return (void)Undefined;
    return instructionSubtract<Size>(memory, Immediate{fetch<Size>()});
  case 0x3b: if constexpr(isLong<Size>()) return (void)Undefined;
    return instructionSubtractCarry<Size>(memory, Immediate{fetch<Size>()});
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd<Size>(registerLookup<Size>(data), memory);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionAdd<Size>(memory, registerLookup<Size>(data));
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry<Size>(registerLookup<Size>(data), memory);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionAddCarry<Size>(memory, registerLookup<Size>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSubtract<Size>(registerLookup<Size>(data), memory);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionSubtract<Size>(memory, registerLookup<Size>(data));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionSubtractCarry<Size>(registerLookup<Size>(data), memory);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    return instructionSubtractCarry<Size>(memory, registerLookup<Size>(data));
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionAnd<Size>(registerLookup<Size>(data), memory);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionAnd<Size>(memory, registerLookup<Size>(data));
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    return instructionXor<Size>(registerLookup<Size>(data), memory);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionXor<Size>(memory, registerLookup<Size>(data));
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    return instructionOr<Size>(registerLookup<Size>(data), memory);
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionOr<Size>(memory, registerLookup<Size>(data));
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    return instructionCompare<Size>(registerLookup<Size>(data), memory);
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionCompare<Size>(memory, registerLookup<Size>(data));
  }
}

auto TLCS900H::instructionTargetMemory(Memory memory) -> void {
  auto data = fetch();

  switch(data) {
  case 0x01: return (void)Undefined;
  case 0x03: return (void)Undefined;
  case 0x04: return instructionPop<Byte>(memory);
  case 0x05: return (void)Undefined;
  case 0x06: return instructionPop<Word>(memory);
  case 0x07: return (void)Undefined;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: return (void)Undefined;
  case 0x10: case 0x11: case 0x12: case 0x13: return (void)Undefined;
  case 0x15: return (void)Undefined;
  case 0x17: return (void)Undefined;
  case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad<Word>(registerLookup<Word>(data), memory);
  case 0x2d: case 0x2e: case 0x2f: return (void)Undefined;
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    return instructionLoad<Long>(registerLookup<Long>(data), memory);
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: return (void)Undefined;
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    return instructionLoad<Byte>(memory, registerLookup<Byte>(data));
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f: return (void)Undefined;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    return instructionLoad<Word>(memory, registerLookup<Word>(data));
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f: return (void)Undefined;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    return instructionLoad<Long>(memory, registerLookup<Long>(data));
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: return (void)Undefined;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: return (void)Undefined;
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: return (void)Undefined;
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionJump<Long>(data.bits(0,3), memory);
  }
}
