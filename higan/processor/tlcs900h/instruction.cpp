template<> auto TLCS900H::registers< uint8>(uint3 code) const -> Register< uint8> {
  static const Register< uint8> lookup[] = {W, A, B, C, D, E, H, L};
  return lookup[code];
}

template<> auto TLCS900H::registers<uint16>(uint3 code) const -> Register<uint16> {
  static const Register<uint16> lookup[] = {WA, BC, DE, HL, IX, IY, IZ, SP};
  return lookup[code];
}

template<> auto TLCS900H::registers<uint32>(uint3 code) const -> Register<uint32> {
  static const Register<uint32> lookup[] = {XWA, XBC, XDE, XHL, XIX, XIY, XIZ, XSP};
  return lookup[code];
}

auto TLCS900H::instruction() -> void {
  auto data = fetch();

  switch(data) {
  case 0x00: return instructionNoOperation();
  case 0x01: return (void)Undefined;
  case 0x02: return instructionPush(SR);
  case 0x03: return instructionPop(SR);
  case 0x04: return (void)Undefined;
  case 0x05: return instructionHalt();
  case 0x06: return instructionSetInterruptFlags((uint3)fetch());
  case 0x07: return instructionReturnInterrupt();
  case 0x09: return instructionPush(Immediate< uint8>{fetch< uint8>()});
  case 0x0b: return instructionPush(Immediate<uint16>{fetch<uint16>()});
  case 0x12: return instructionComplementCarry();
  case 0x14: return instructionPush(A);
  case 0x15: return instructionPop(A);
  case 0x16: return instructionExchange(F, FP);
  case 0x18: return instructionPush(F);
  case 0x19: return instructionPop(F);
  case 0x1a: return instructionJump(True, Immediate<uint16>{fetch<uint16>()});
  case 0x1b: return instructionJump(True, Immediate<uint24>{fetch<uint24>()});
  case 0x1c: return instructionCall(True, Immediate<uint16>{fetch<uint16>()});
  case 0x1d: return instructionCall(True, Immediate<uint24>{fetch<uint24>()});
  case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad(registers<uint8>(data), Immediate<uint8>{fetch<uint8>()});
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
    return instructionPush(registers<uint16>(data));
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    return instructionLoad(registers<uint16>(data), Immediate<uint16>{fetch<uint16>()});
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
    return instructionPush(registers<uint32>(data));
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    return instructionLoad(registers<uint32>(data), Immediate<uint32>{fetch<uint32>()});
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    return instructionPop(registers<uint16>(data));
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    return (void)Undefined;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    return instructionPop(registers<uint32>(data));
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    return instructionJumpRelative((uint4)data, Immediate<int8>{fetch<int8>()});
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    return instructionJumpRelative((uint4)data, Immediate<int16>{fetch<int16>()});
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionSourceMemory(Memory<uint8>{load(registers<uint32>(data))});
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionSourceMemory(Memory<uint8>{load(registers<uint32>(data)) + fetch<int8>()});
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionSourceMemory(Memory<uint16>{load(registers<uint32>(data))});
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionSourceMemory(Memory<uint16>{load(registers<uint32>(data)) + fetch<int8>()});
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSourceMemory(Memory<uint32>{load(registers<uint32>(data))});
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionSourceMemory(Memory<uint32>{load(registers<uint32>(data)) + fetch<int8>()});
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionTargetMemory(load(registers<uint32>(data)));
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    return instructionTargetMemory(load(registers<uint32>(data)) + fetch<int8>());
  case 0xc0: return instructionSourceMemory(Memory<uint8>{fetch< uint8>()});
  case 0xc1: return instructionSourceMemory(Memory<uint8>{fetch<uint16>()});
  case 0xc2: return instructionSourceMemory(Memory<uint8>{fetch<uint24>()});
  case 0xc3: {
    data = fetch();
    if((data & 3) == 0) return instructionSourceMemory(Memory<uint8>{load(Register<uint32>{data})});
    if((data & 3) == 1) return instructionSourceMemory(Memory<uint8>{load(Register<uint32>{data}) + fetch<int16>()});
    if(data == 0x03) {
      auto r32 = load(Register<uint32>{fetch()});
      auto  r8 = load(Register< uint8>{fetch()});
      return instructionSourceMemory(Memory<uint8>{r32 + (int8)r8});
    }
    if(data == 0x07) {
      auto r32 = load(Register<uint32>{fetch()});
      auto r16 = load(Register<uint16>{fetch()});
      return instructionSourceMemory(Memory<uint8>{r32 + (int16)r16});
    }
    return (void)Undefined; }
  case 0xc4: {
    data = fetch();
    auto register = Register<uint32>{data};
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionSourceMemory(Memory<uint8>{load(register)}); }
  case 0xc5: {
    data = fetch();
    auto register = Register<uint32>{data};
    instructionSourceMemory(Memory<uint8>{load(register)});
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xc6: return (void)Undefined;
  case 0xc7: return instructionRegister(Register<uint8>{fetch()});
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionRegister(registers<uint8>(data));
  case 0xd0: return instructionSourceMemory(Memory<uint16>{fetch< uint8>()});
  case 0xd1: return instructionSourceMemory(Memory<uint16>{fetch<uint16>()});
  case 0xd2: return instructionSourceMemory(Memory<uint16>{fetch<uint24>()});
  case 0xd3: {
    data = fetch();
    if((data & 3) == 0) return instructionSourceMemory(Memory<uint16>{load(Register<uint32>{data})});
    if((data & 3) == 1) return instructionSourceMemory(Memory<uint16>{load(Register<uint32>{data}) + fetch<int16>()});
    if(data == 0x03) {
      auto r32 = load(Register<uint32>{fetch()});
      auto  r8 = load(Register< uint8>{fetch()});
      return instructionSourceMemory(Memory<uint16>{r32 + (int8)r8});
    }
    if(data == 0x07) {
      auto r32 = load(Register<uint32>{fetch()});
      auto r16 = load(Register<uint16>{fetch()});
      return instructionSourceMemory(Memory<uint16>{r32 + (int16)r16});
    }
    return (void)Undefined; }
  case 0xd4: {
    data = fetch();
    auto register = Register<uint32>{data};
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionSourceMemory(Memory<uint16>{load(register)}); }
  case 0xd5: {
    data = fetch();
    auto register = Register<uint32>{data};
    instructionSourceMemory(Memory<uint16>{load(register)});
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xd6: return (void)Undefined;
  case 0xd7: return instructionRegister(Register<uint16>{fetch()});
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionRegister(registers<uint16>(data));
  case 0xe0: return instructionSourceMemory(Memory<uint32>{fetch<uint8>()});
  case 0xe1: return instructionSourceMemory(Memory<uint32>{fetch<uint16>()});
  case 0xe2: return instructionSourceMemory(Memory<uint32>{fetch<uint24>()});
  case 0xe3: {
    data = fetch();
    if((data & 3) == 0) return instructionSourceMemory(Memory<uint32>{load(Register<uint32>{data})});
    if((data & 3) == 1) return instructionSourceMemory(Memory<uint32>{load(Register<uint32>{data}) + fetch<int16>()});
    if(data == 0x03) {
      auto r32 = load(Register<uint32>{fetch()});
      auto  r8 = load(Register< uint8>{fetch()});
      return instructionSourceMemory(Memory<uint32>{r32 + (int8)r8});
    }
    if(data == 0x07) {
      auto r32 = load(Register<uint32>{fetch()});
      auto r16 = load(Register< uint8>{fetch()});
      return instructionSourceMemory(Memory<uint32>{r32 + (int16)r16});
    }
    return (void)Undefined; }
  case 0xe4: {
    data = fetch();
    auto register = Register<uint32>{data};
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionSourceMemory(Memory<uint32>{load(register)}); }
  case 0xe5: {
    data = fetch();
    auto register = Register<uint32>{data};
    instructionSourceMemory(Memory<uint32>{load(register)});
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xe6: return (void)Undefined;
  case 0xe7: return instructionRegister(Register<uint32>{fetch()});
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionRegister(registers<uint32>(data));
  case 0xf0: return instructionTargetMemory(fetch<uint8>());
  case 0xf1: return instructionTargetMemory(fetch<uint16>());
  case 0xf2: return instructionTargetMemory(fetch<uint24>());
  case 0xf3: {
    data = fetch();
    if((data & 3) == 0) return instructionTargetMemory(load(Register<uint32>{data}));
    if((data & 3) == 1) return instructionTargetMemory(load(Register<uint32>{data}) + fetch<int16>());
    if(data == 0x03) {
      auto r32 = load(Register<uint32>{fetch()});
      auto  r8 = load(Register< uint8>{fetch()});
      return instructionTargetMemory(r32 + (int8)r8);
    }
    if(data == 0x07) {
      auto r32 = load(Register<uint32>{fetch()});
      auto r16 = load(Register<uint16>{fetch()});
      return instructionTargetMemory(r32 + (int16)r16);
    }
    return (void)Undefined; }
  case 0xf4: {
    data = fetch();
    auto register = Register<uint32>{data};
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionTargetMemory(load(register)); }
  case 0xf5: {
    data = fetch();
    auto register = Register<uint32>{data};
    instructionTargetMemory(load(register));
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xf6: case 0xf7: return (void)Undefined;
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionSoftwareInterrupt((uint3)data);
  }
}

template<typename R>
auto TLCS900H::instructionRegister(R register) -> void {
  using type = typename R::type;
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02: return (void)Undefined;
  case 0x04: return instructionPush(register);
  case 0x05: return instructionPop(register);
  case 0x11: return (void)Undefined;
  case 0x15: return (void)Undefined;
  case 0x17: case 0x18: return (void)Undefined;
  case 0x1a: case 0x1b: return (void)Undefined;
  case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x25: case 0x26: case 0x27: return (void)Undefined;
  case 0x2d: return (void)Undefined;
  case 0x35: case 0x36: case 0x37: return (void)Undefined;
  case 0x3b: return (void)Undefined;
  case 0x3f: return (void)Undefined;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    if constexpr(R::bits == 32) return (void)Undefined;
    return instructionSetConditionCode((uint4)data, register);
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd(registers<type>(data), register);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionLoad(registers<type>(data), register);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry(registers<type>(data), register);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionLoad(register, registers<type>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSubtract(registers<type>(data), register);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionLoad(register, Immediate<type>{(uint3)data});
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionSubtractCarry(registers<type>(data), register);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    if constexpr(R::bits == 32) return (void)Undefined;
    return instructionExchange(registers<type>(data), register);
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionAnd(registers<type>(data), register);
  case 0xc8: return instructionAdd(register, Immediate<type>{fetch<type>()});
  case 0xc9: return instructionAddCarry(register, Immediate<type>{fetch<type>()});
  case 0xca: return instructionSubtract(register, Immediate<type>{fetch<type>()});
  case 0xcb: return instructionSubtractCarry(register, Immediate<type>{fetch<type>()});
  case 0xcc: return instructionAnd(register, Immediate<type>{fetch<type>()});
  case 0xcd: return instructionXor(register, Immediate<type>{fetch<type>()});
  case 0xce: return instructionOr(register, Immediate<type>{fetch<type>()});
  case 0xcf: return instructionCompare(register, Immediate<type>{fetch<type>()});
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    return instructionXor(registers<type>(data), register);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionCompare(register, Immediate<type>{(uint3)data});
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    return instructionOr(registers<type>(data), register);
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    return instructionCompare(registers<type>(data), register);
  }
}

template<typename M>
auto TLCS900H::instructionSourceMemory(M memory) -> void {
  using type = typename M::type;
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02: case 0x03: return (void)Undefined;
  case 0x04:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionPush(memory);
  case 0x05: return (void)Undefined;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: return (void)Undefined;
  case 0x18: return (void)Undefined;
  case 0x19:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionLoad(Memory<type>{fetch<type>()}, memory);
  case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad(registers<type>(data), memory);
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: return (void)Undefined;
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionExchange(memory, registers<type>(data));
  case 0x38:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionAdd(memory, Immediate<type>{fetch<type>()});
  case 0x39:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionAddCarry(memory, Immediate<type>{fetch<type>()});
  case 0x3a:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionSubtract(memory, Immediate<type>{fetch<type>()});
  case 0x3b:
    if constexpr(M::bits == 32) return (void)Undefined;
    return instructionSubtractCarry(memory, Immediate<type>{fetch<type>()});
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd(registers<type>(data), memory);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionAdd(memory, registers<type>(data));
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry(registers<type>(data), memory);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionAddCarry(memory, registers<type>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSubtract(registers<type>(data), memory);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionSubtract(memory, registers<type>(data));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionSubtractCarry(registers<type>(data), memory);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    return instructionSubtractCarry(memory, registers<type>(data));
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionAnd(registers<type>(data), memory);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionAnd(memory, registers<type>(data));
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    return instructionXor(registers<type>(data), memory);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionXor(memory, registers<type>(data));
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    return instructionOr(registers<type>(data), memory);
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionOr(memory, registers<type>(data));
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    return instructionCompare(registers<type>(data), memory);
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionCompare(memory, registers<type>(data));
  }
}

auto TLCS900H::instructionTargetMemory(uint32 address) -> void {
  auto data = fetch();

  switch(data) {
  case 0x01: return (void)Undefined;
  case 0x03: return (void)Undefined;
  case 0x04: return instructionPop(Memory<uint8>{address});
  case 0x05: return (void)Undefined;
  case 0x06: return instructionPop(Memory<uint16>{address});
  case 0x07: return (void)Undefined;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: return (void)Undefined;
  case 0x10: case 0x11: case 0x12: case 0x13: return (void)Undefined;
  case 0x15: return (void)Undefined;
  case 0x17: return (void)Undefined;
  case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad(registers<uint16>(data), Memory<uint16>{address});
  case 0x2d: case 0x2e: case 0x2f: return (void)Undefined;
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    return instructionLoad(registers<uint32>(data), Memory<uint32>{address});
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: return (void)Undefined;
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    return instructionLoad(Memory<uint8>{address}, registers<uint8>(data));
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f: return (void)Undefined;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    return instructionLoad(Memory<uint16>{address}, registers<uint16>(data));
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f: return (void)Undefined;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    return instructionLoad(Memory<uint32>{address}, registers<uint32>(data));
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: return (void)Undefined;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: return (void)Undefined;
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: return (void)Undefined;
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionJump((uint4)data, Memory<uint32>{address});
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionCall((uint4)data, Memory<uint32>{address});
  }
}
