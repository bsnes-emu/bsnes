template<> auto TLCS900H::toRegister3<uint8>(uint3 code) const -> Register<uint8> {
  static const Register< uint8> lookup[] = {W, A, B, C, D, E, H, L};
  return lookup[code];
}

template<> auto TLCS900H::toRegister3<uint16>(uint3 code) const -> Register<uint16> {
  static const Register<uint16> lookup[] = {WA, BC, DE, HL, IX, IY, IZ, SP};
  return lookup[code];
}

template<> auto TLCS900H::toRegister3<uint32>(uint3 code) const -> Register<uint32> {
  static const Register<uint32> lookup[] = {XWA, XBC, XDE, XHL, XIX, XIY, XIZ, XSP};
  return lookup[code];
}

template<typename T> auto TLCS900H::toRegister8(uint8 code) const -> Register<T> { return {code}; }
template<typename T> auto TLCS900H::toControlRegister(uint8 code) const -> ControlRegister<T> { return {code}; }
template<typename T> auto TLCS900H::toMemory(uint32 address) const -> Memory<T> { return {address}; }
template<typename T> auto TLCS900H::toImmediate(uint32 constant) const -> Immediate<T> { return {constant}; }
template<typename T> auto TLCS900H::toImmediate3(natural constant) const -> Immediate<T> { return {constant.clip(3) ? constant.clip(3) : 8}; }

//note: much of this code is split to multiple statements due to C++ not guaranteeing
//the order of evaluations of function arguments. fetch() ordering is critical.

auto TLCS900H::instruction() -> void {
  auto data = fetch();

  switch(r.prefix = data) {
  case 0x00: return instructionNoOperation();
  case 0x01: return (void)Undefined;  //NORMAL (not present on 900/H)
  case 0x02: return instructionPush(SR);
  case 0x03: return instructionPop(SR);
  case 0x04: return (void)Undefined;  //MAX or MIN (not present on 900/H)
  case 0x05: return instructionHalt();
  case 0x06: return instructionSetInterruptFlipFlop((uint3)fetch());
  case 0x07: return instructionReturnInterrupt();
  case 0x08: {
    auto memory = fetchMemory<uint8, uint8>();
    return instructionLoad(memory, fetchImmediate<uint8>()); }
  case 0x09: return instructionPush(fetchImmediate<uint8>());
  case 0x0a: {
    auto memory = fetchMemory<uint16, uint8>();
    return instructionLoad(memory, fetchImmediate<uint16>()); }
  case 0x0b: return instructionPush(fetchImmediate<uint16>());
  case 0x0c: return instructionSetRegisterFilePointer(RFP + 1);
  case 0x0d: return instructionSetRegisterFilePointer(RFP - 1);
  case 0x0e: return instructionReturn(True);
  case 0x0f: return instructionReturnDeallocate(fetchImmediate<int16>());
  case 0x10: return instructionSetFlag(CF, 0);
  case 0x11: return instructionSetFlag(CF, 1);
  case 0x12: return instructionSetFlag(CF, !CF);
  case 0x13: return instructionSetFlag(CF, ZF);
  case 0x14: return instructionPush(A);
  case 0x15: return instructionPop(A);
  case 0x16: return instructionExchange(F, FP);
  case 0x17: return instructionSetRegisterFilePointer((uint2)fetch());
  case 0x18: return instructionPush(F);
  case 0x19: return instructionPop(F);
  case 0x1a: return instructionJump(True, fetchImmediate<uint16>());
  case 0x1b: return instructionJump(True, fetchImmediate<uint24>());
  case 0x1c: return instructionCall(True, fetchImmediate<uint16>());
  case 0x1d: return instructionCall(True, fetchImmediate<uint24>());
  case 0x1e: return instructionCallRelative(fetchImmediate<int16>());
  case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad(toRegister3<uint8>(data), fetchImmediate<uint8>());
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
    return instructionPush(toRegister3<uint16>(data));
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    return instructionLoad(toRegister3<uint16>(data), fetchImmediate<uint16>());
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
    return instructionPush(toRegister3<uint32>(data));
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    return instructionLoad(toRegister3<uint32>(data), fetchImmediate<uint32>());
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    return instructionPop(toRegister3<uint16>(data));
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: return (void)Undefined;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    return instructionPop(toRegister3<uint32>(data));
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    return instructionJumpRelative((uint4)data, fetchImmediate<int8>());
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    return instructionJumpRelative((uint4)data, fetchImmediate<int16>());
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionSourceMemory(toMemory<uint8>(load(toRegister3<uint32>(data))));
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionSourceMemory(toMemory<uint8>(load(toRegister3<uint32>(data)) + fetch<int8>()));
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionSourceMemory(toMemory<uint16>(load(toRegister3<uint32>(data))));
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionSourceMemory(toMemory<uint16>(load(toRegister3<uint32>(data)) + fetch<int8>()));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSourceMemory(toMemory<uint32>(load(toRegister3<uint32>(data))));
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionSourceMemory(toMemory<uint32>(load(toRegister3<uint32>(data)) + fetch<int8>()));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionTargetMemory(load(toRegister3<uint32>(data)));
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    return instructionTargetMemory(load(toRegister3<uint32>(data)) + fetch<int8>());
  case 0xc0: return instructionSourceMemory(fetchMemory<uint8,  uint8>());
  case 0xc1: return instructionSourceMemory(fetchMemory<uint8, uint16>());
  case 0xc2: return instructionSourceMemory(fetchMemory<uint8, uint24>());
  case 0xc3: {
    data = fetch();
    if((data & 3) == 0) return instructionSourceMemory(toMemory<uint8>(load(toRegister8<uint32>(data))));
    if((data & 3) == 1) return instructionSourceMemory(toMemory<uint8>(load(toRegister8<uint32>(data)) + fetch<int16>()));
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      return instructionSourceMemory(Memory<uint8>{r32 + (int8)r8});
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      return instructionSourceMemory(Memory<uint8>{r32 + (int16)r16});
    }
    return (void)Undefined; }
  case 0xc4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionSourceMemory(toMemory<uint8>(load(register))); }
  case 0xc5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    instructionSourceMemory(toMemory<uint8>(load(register)));
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xc6: return (void)Undefined;
  case 0xc7: return instructionRegister(fetchRegister<uint8>());
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionRegister(toRegister3<uint8>(data));
  case 0xd0: return instructionSourceMemory(fetchMemory<uint16,  uint8>());
  case 0xd1: return instructionSourceMemory(fetchMemory<uint16, uint16>());
  case 0xd2: return instructionSourceMemory(fetchMemory<uint16, uint24>());
  case 0xd3: {
    data = fetch();
    if((data & 3) == 0) return instructionSourceMemory(toMemory<uint16>(load(toRegister8<uint32>(data))));
    if((data & 3) == 1) return instructionSourceMemory(toMemory<uint16>(load(toRegister8<uint32>(data)) + fetch<int16>()));
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      return instructionSourceMemory(toMemory<uint16>(r32 + (int8)r8));
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      return instructionSourceMemory(toMemory<uint16>(r32 + (int16)r16));
    }
    return (void)Undefined; }
  case 0xd4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionSourceMemory(toMemory<uint16>(load(register))); }
  case 0xd5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    instructionSourceMemory(toMemory<uint16>(load(register)));
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xd6: return (void)Undefined;
  case 0xd7: return instructionRegister(fetchRegister<uint16>());
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionRegister(toRegister3<uint16>(data));
  case 0xe0: return instructionSourceMemory(fetchMemory<uint32,  uint8>());
  case 0xe1: return instructionSourceMemory(fetchMemory<uint32, uint16>());
  case 0xe2: return instructionSourceMemory(fetchMemory<uint32, uint24>());
  case 0xe3: {
    data = fetch();
    if((data & 3) == 0) return instructionSourceMemory(toMemory<uint32>(load(toRegister8<uint32>(data))));
    if((data & 3) == 1) return instructionSourceMemory(toMemory<uint32>(load(toRegister8<uint32>(data)) + fetch<int16>()));
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      return instructionSourceMemory(toMemory<uint32>(r32 + (int8)r8));
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      return instructionSourceMemory(toMemory<uint32>(r32 + (int16)r16));
    }
    return (void)Undefined; }
  case 0xe4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionSourceMemory(toMemory<uint32>(load(register))); }
  case 0xe5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    instructionSourceMemory(toMemory<uint32>(load(register)));
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xe6: return (void)Undefined;
  case 0xe7: return instructionRegister(fetchRegister<uint32>());
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionRegister(toRegister3<uint32>(data));
  case 0xf0: return instructionTargetMemory(fetch< uint8>());
  case 0xf1: return instructionTargetMemory(fetch<uint16>());
  case 0xf2: return instructionTargetMemory(fetch<uint24>());
  case 0xf3: {
    data = fetch();
    if((data & 3) == 0) return instructionTargetMemory(load(toRegister8<uint32>(data)));
    if((data & 3) == 1) return instructionTargetMemory(load(toRegister8<uint32>(data)) + fetch<int16>());
    if(data == 0x03) {
      auto r32 = load(fetchRegister<uint32>());
      auto  r8 = load(fetchRegister< uint8>());
      return instructionTargetMemory(r32 + (int8)r8);
    }
    if(data == 0x07) {
      auto r32 = load(fetchRegister<uint32>());
      auto r16 = load(fetchRegister<uint16>());
      return instructionTargetMemory(r32 + (int16)r16);
    }
    return (void)Undefined; }
  case 0xf4: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    if((data & 3) == 0) store(register, load(register) - 1);
    if((data & 3) == 1) store(register, load(register) - 2);
    if((data & 3) == 2) store(register, load(register) - 4);
    if((data & 3) == 3) Undefined;
    return instructionTargetMemory(load(register)); }
  case 0xf5: {
    data = fetch();
    auto register = toRegister8<uint32>(data);
    instructionTargetMemory(load(register));
    if((data & 3) == 0) store(register, load(register) + 1);
    if((data & 3) == 1) store(register, load(register) + 2);
    if((data & 3) == 2) store(register, load(register) + 4);
    if((data & 3) == 3) Undefined;
    return; }
  case 0xf6: return (void)Undefined;
  case 0xf7: {
    if(fetch()) Undefined;
    auto memory = fetchMemory<uint8, uint8>();
    if(fetch()) Undefined;
    auto immediate = fetchImmediate<uint8>();
    if(fetch()) Undefined;
    return instructionLoad(memory, immediate); }
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionSoftwareInterrupt((uint3)data);
  }
}

template<typename R>
auto TLCS900H::instructionRegister(R register) -> void {
  using T = typename R::type;
  enum : uint { bits = R::bits };
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02: return (void)Undefined;
  case 0x03: return instructionLoad(register, fetchImmediate<T>());
  case 0x04: return instructionPush(register);
  case 0x05: return instructionPop(register);
  case 0x06:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionComplement(register);
  case 0x07:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionNegate(register);
  case 0x08:
    if constexpr(bits != 32) return instructionMultiply(register, fetchImmediate<T>());
    return (void)Undefined;
  case 0x09:
    if constexpr(bits != 32) return instructionMultiplySigned(register, fetchImmediate<T>());
    return (void)Undefined;
  case 0x0a:
    if constexpr(bits != 32) return instructionDivide(register, fetchImmediate<T>());
    return (void)Undefined;
  case 0x0b:
    if constexpr(bits != 32) return instructionDivideSigned(register, fetchImmediate<T>());
    return (void)Undefined;
  case 0x0c:
    if constexpr(bits == 32) return instructionLink(register, fetchImmediate<int16>());
    return (void)Undefined;
  case 0x0d:
    if constexpr(bits == 32) return instructionUnlink(register);
    return (void)Undefined;
  case 0x0e:
    if constexpr(bits == 16) return instructionBitSearch1Forward(register);
    return (void)Undefined;
  case 0x0f:
    if constexpr(bits == 16) return instructionBitSearch1Backward(register);
    return (void)Undefined;
  case 0x10:
    if constexpr(bits == 8) return instructionDecimalAdjustAccumulator(register);
    return (void)Undefined;
  case 0x11: return (void)Undefined;
  case 0x12:
    if constexpr(bits != 8) return instructionExtendZero(register);
    return (void)Undefined;
  case 0x13:
    if constexpr(bits != 8) return instructionExtendSign(register);
    return (void)Undefined;
  case 0x14:
    if constexpr(bits != 8) return instructionPointerAdjustAccumulator(register);
    return (void)Undefined;
  case 0x15: return (void)Undefined;
  case 0x16:
    if constexpr(bits == 16) return instructionMirror(register);
    return (void)Undefined;
  case 0x17: case 0x18: return (void)Undefined;
  case 0x19:
    if constexpr(bits == 16) return instructionMultiplyAdd(register);
    return (void)Undefined;
  case 0x1a: case 0x1b: return (void)Undefined;
  case 0x1c:
    if constexpr(bits != 32) return instructionDecrementJumpNotZero(register, fetchImmediate<int8>());
    return (void)Undefined;
  case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20:
    if constexpr(bits != 32) return instructionAndCarry(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x21:
    if constexpr(bits != 32) return instructionOrCarry(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x22:
    if constexpr(bits != 32) return instructionXorCarry(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x23:
    if constexpr(bits != 32) return instructionLoadCarry(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x24:
    if constexpr(bits != 32) return instructionStoreCarry(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x25: case 0x26: case 0x27: return (void)Undefined;
  case 0x28:
    if constexpr(bits != 32) return instructionAndCarry(register, A);
    return (void)Undefined;
  case 0x29:
    if constexpr(bits != 32) return instructionOrCarry(register, A);
    return (void)Undefined;
  case 0x2a:
    if constexpr(bits != 32) return instructionXorCarry(register, A);
    return (void)Undefined;
  case 0x2b:
    if constexpr(bits != 32) return instructionLoadCarry(register, A);
    return (void)Undefined;
  case 0x2c:
    if constexpr(bits != 32) return instructionStoreCarry(register, A);
    return (void)Undefined;
  case 0x2d: return (void)Undefined;
  case 0x2e: return instructionLoad(toControlRegister<T>(data), register);
  case 0x2f: return instructionLoad(register, toControlRegister<T>(data));
  case 0x30:
    if constexpr(bits != 32) return instructionReset(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x31:
    if constexpr(bits != 32) return instructionSet(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x32:
    if constexpr(bits != 32) return instructionChange(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x33:
    if constexpr(bits != 32) return instructionBit(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x34:
    if constexpr(bits != 32) return instructionTestSet(register, fetchImmediate<uint8>());
    return (void)Undefined;
  case 0x35: case 0x36: case 0x37: return (void)Undefined;
  case 0x38:
    if constexpr(bits == 16) return instructionModuloIncrement<1>(register, fetchImmediate<uint16>());
    return (void)Undefined;
  case 0x39:
    if constexpr(bits == 16) return instructionModuloIncrement<2>(register, fetchImmediate<uint16>());
    return (void)Undefined;
  case 0x3a:
    if constexpr(bits == 16) return instructionModuloIncrement<4>(register, fetchImmediate<uint16>());
    return (void)Undefined;
  case 0x3b: return (void)Undefined;
  case 0x3c:
    if constexpr(bits == 16) return instructionModuloDecrement<1>(register, fetchImmediate<uint16>());
    return (void)Undefined;
  case 0x3d:
    if constexpr(bits == 16) return instructionModuloDecrement<2>(register, fetchImmediate<uint16>());
    return (void)Undefined;
  case 0x3e:
    if constexpr(bits == 16) return instructionModuloDecrement<4>(register, fetchImmediate<uint16>());
    return (void)Undefined;
  case 0x3f: return (void)Undefined;
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    if constexpr(bits != 32) return instructionMultiply(toRegister3<T>(data), register);
    return (void)Undefined;
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    if constexpr(bits != 32) return instructionMultiplySigned(toRegister3<T>(data), register);
    return (void)Undefined;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    if constexpr(bits != 32) return instructionDivide(toRegister3<T>(data), register);
    return (void)Undefined;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    if constexpr(bits != 32) return instructionDivideSigned(toRegister3<T>(data), register);
    return (void)Undefined;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    return instructionIncrement(register, toImmediate<T>((uint3)data));
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    return instructionDecrement(register, toImmediate<T>((uint3)data));
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionSetConditionCode((uint4)data, register);
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd(toRegister3<T>(data), register);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionLoad(toRegister3<T>(data), register);
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry(toRegister3<T>(data), register);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionLoad(register, toRegister3<T>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSubtract(toRegister3<T>(data), register);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionLoad(register, toImmediate<T>((uint3)data));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionSubtractBorrow(toRegister3<T>(data), register);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionExchange(toRegister3<T>(data), register);
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionAnd(toRegister3<T>(data), register);
  case 0xc8: return instructionAdd(register, fetchImmediate<T>());
  case 0xc9: return instructionAddCarry(register, fetchImmediate<T>());
  case 0xca: return instructionSubtract(register, fetchImmediate<T>());
  case 0xcb: return instructionSubtractBorrow(register, fetchImmediate<T>());
  case 0xcc: return instructionAnd(register, fetchImmediate<T>());
  case 0xcd: return instructionXor(register, fetchImmediate<T>());
  case 0xce: return instructionOr(register, fetchImmediate<T>());
  case 0xcf: return instructionCompare(register, fetchImmediate<T>());
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    return instructionXor(toRegister3<T>(data), register);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionCompare(register, toImmediate<T>((uint3)data));
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    return instructionOr(toRegister3<T>(data), register);
  case 0xe8: return instructionRotateLeftWithoutCarry(register, fetchImmediate<uint8>());
  case 0xe9: return instructionRotateRightWithoutCarry(register, fetchImmediate<uint8>());
  case 0xea: return instructionRotateLeft(register, fetchImmediate<uint8>());
  case 0xeb: return instructionRotateRight(register, fetchImmediate<uint8>());
  case 0xec: return instructionShiftLeftArithmetic(register, fetchImmediate<uint8>());
  case 0xed: return instructionShiftRightArithmetic(register, fetchImmediate<uint8>());
  case 0xee: return instructionShiftLeftLogical(register, fetchImmediate<uint8>());
  case 0xef: return instructionShiftRightLogical(register, fetchImmediate<uint8>());
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    return instructionCompare(toRegister3<T>(data), register);
  case 0xf8: return instructionRotateLeftWithoutCarry(register, A);
  case 0xf9: return instructionRotateRightWithoutCarry(register, A);
  case 0xfa: return instructionRotateLeft(register, A);
  case 0xfb: return instructionRotateRight(register, A);
  case 0xfc: return instructionShiftLeftArithmetic(register, A);
  case 0xfd: return instructionShiftRightArithmetic(register, A);
  case 0xfe: return instructionShiftLeftLogical(register, A);
  case 0xff: return instructionShiftRightLogical(register, A);
  }
}

template<typename M>
auto TLCS900H::instructionSourceMemory(M memory) -> void {
  using T = typename M::type;
  enum : uint { bits = M::bits };
  auto data = fetch();

  switch(data) {
  case 0x00: case 0x01: case 0x02: case 0x03: return (void)Undefined;
  case 0x04:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionPush(memory);
  case 0x05: return (void)Undefined;
  case 0x06:
    if constexpr(bits == 8) return instructionRotateLeftDigit(A, memory);
    return (void)Undefined;
  case 0x07:
    if constexpr(bits == 8) return instructionRotateRightDigit(A, memory);
    return (void)Undefined;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: return (void)Undefined;
  case 0x10:
    if constexpr(bits ==  8) return instructionLoad<T, +1>();
    if constexpr(bits == 16) return instructionLoad<T, +2>();
    return (void)Undefined;
  case 0x11:
    if constexpr(bits ==  8) return instructionLoadRepeat<T, +1>();
    if constexpr(bits == 16) return instructionLoadRepeat<T, +2>();
    return (void)Undefined;
  case 0x12:
    if constexpr(bits ==  8) return instructionLoad<T, -1>();
    if constexpr(bits == 16) return instructionLoad<T, -2>();
    return (void)Undefined;
  case 0x13:
    if constexpr(bits ==  8) return instructionLoadRepeat<T, -1>();
    if constexpr(bits == 16) return instructionLoadRepeat<T, -2>();
    return (void)Undefined;
  case 0x14:
    if constexpr(bits ==  8) return instructionCompare<T, +1>(A);
    if constexpr(bits == 16) return instructionCompare<T, +2>(WA);
    return (void)Undefined;
  case 0x15:
    if constexpr(bits ==  8) return instructionCompareRepeat<T, +1>(A);
    if constexpr(bits == 16) return instructionCompareRepeat<T, +2>(WA);
    return (void)Undefined;
  case 0x16:
    if constexpr(bits ==  8) return instructionCompare<T, -1>(A);
    if constexpr(bits == 16) return instructionCompare<T, -2>(WA);
    return (void)Undefined;
  case 0x17:
    if constexpr(bits ==  8) return instructionCompareRepeat<T, -1>(A);
    if constexpr(bits == 16) return instructionCompareRepeat<T, -2>(WA);
    return (void)Undefined;
  case 0x18: return (void)Undefined;
  case 0x19:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionLoad(fetchMemory<T, uint16>(), memory);
  case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad(toRegister3<T>(data), memory);
  case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: return (void)Undefined;
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionExchange(memory, toRegister3<T>(data));
  case 0x38:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionAdd(memory, fetchImmediate<T>());
  case 0x39:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionAddCarry(memory, fetchImmediate<T>());
  case 0x3a:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionSubtract(memory, fetchImmediate<T>());
  case 0x3b:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionSubtractBorrow(memory, fetchImmediate<T>());
  case 0x3c:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionAnd(memory, fetchImmediate<T>());
  case 0x3d:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionXor(memory, fetchImmediate<T>());
  case 0x3e:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionOr(memory, fetchImmediate<T>());
  case 0x3f:
    if constexpr(bits == 32) return (void)Undefined;
    return instructionCompare(memory, fetchImmediate<T>());
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    if constexpr(bits != 32) return instructionMultiply(toRegister3<T>(data), memory);
    return (void)Undefined;
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
    if constexpr(bits != 32) return instructionMultiplySigned(toRegister3<T>(data), memory);
    return (void)Undefined;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    if constexpr(bits != 32) return instructionDivide(toRegister3<T>(data), memory);
    return (void)Undefined;
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    if constexpr(bits != 32) return instructionDivideSigned(toRegister3<T>(data), memory);
    return (void)Undefined;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    return instructionIncrement(memory, toImmediate<T>((uint3)data));
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
    return instructionDecrement(memory, toImmediate<T>((uint3)data));
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: return (void)Undefined;
  case 0x78:
    if constexpr(bits != 32) return instructionRotateLeftWithoutCarry(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x79:
    if constexpr(bits != 32) return instructionRotateRightWithoutCarry(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x7a:
    if constexpr(bits != 32) return instructionRotateLeft(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x7b:
    if constexpr(bits != 32) return instructionRotateRight(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x7c:
    if constexpr(bits != 32) return instructionShiftLeftArithmetic(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x7d:
    if constexpr(bits != 32) return instructionShiftRightArithmetic(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x7e:
    if constexpr(bits != 32) return instructionShiftLeftLogical(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x7f:
    if constexpr(bits != 32) return instructionShiftRightLogical(memory, toImmediate<uint4>(1));
    return (void)Undefined;
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAdd(toRegister3<T>(data), memory);
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionAdd(memory, toRegister3<T>(data));
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionAddCarry(toRegister3<T>(data), memory);
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionAddCarry(memory, toRegister3<T>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionSubtract(toRegister3<T>(data), memory);
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionSubtract(memory, toRegister3<T>(data));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionSubtractBorrow(toRegister3<T>(data), memory);
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    return instructionSubtractBorrow(memory, toRegister3<T>(data));
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionAnd(toRegister3<T>(data), memory);
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionAnd(memory, toRegister3<T>(data));
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
    return instructionXor(toRegister3<T>(data), memory);
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionXor(memory, toRegister3<T>(data));
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
    return instructionOr(toRegister3<T>(data), memory);
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionOr(memory, toRegister3<T>(data));
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
    return instructionCompare(toRegister3<T>(data), memory);
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionCompare(memory, toRegister3<T>(data));
  }
}

auto TLCS900H::instructionTargetMemory(uint32 address) -> void {
  auto data = fetch();

  switch(data) {
  case 0x00: return instructionLoad(toMemory<uint8>(address), fetchImmediate<uint8>());
  case 0x01: return (void)Undefined;
  case 0x02: return instructionLoad(toMemory<uint16>(address), fetchImmediate<uint16>());
  case 0x03: return (void)Undefined;
  case 0x04: return instructionPop(toMemory<uint8>(address));
  case 0x05: return (void)Undefined;
  case 0x06: return instructionPop(toMemory<uint16>(address));
  case 0x07: return (void)Undefined;
  case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: return (void)Undefined;
  case 0x10: case 0x11: case 0x12: case 0x13: return (void)Undefined;
  case 0x14: return instructionLoad(toMemory<uint8>(address), fetchMemory<uint8, uint16>());
  case 0x15: return (void)Undefined;
  case 0x16: return instructionLoad(toMemory<uint16>(address), fetchMemory<uint16, uint16>());
  case 0x17: return (void)Undefined;
  case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: return (void)Undefined;
  case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
    return instructionLoad(toRegister3<uint16>(data), toImmediate<uint16>(address));
  case 0x28: return instructionAndCarry(toMemory<uint8>(address), A);
  case 0x29: return instructionOrCarry(toMemory<uint8>(address), A);
  case 0x2a: return instructionXorCarry(toMemory<uint8>(address), A);
  case 0x2b: return instructionLoadCarry(toMemory<uint8>(address), A);
  case 0x2c: return instructionStoreCarry(toMemory<uint8>(address), A);
  case 0x2d: case 0x2e: case 0x2f: return (void)Undefined;
  case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    return instructionLoad(toRegister3<uint32>(data), toImmediate<uint32>(address));
  case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: return (void)Undefined;
  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    return instructionLoad(toMemory<uint8>(address), toRegister3<uint8>(data));
  case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f: return (void)Undefined;
  case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    return instructionLoad(toMemory<uint16>(address), toRegister3<uint16>(data));
  case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f: return (void)Undefined;
  case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    return instructionLoad(toMemory<uint32>(address), toRegister3<uint32>(data));
  case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f: return (void)Undefined;
  case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: return (void)Undefined;
  case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f: return (void)Undefined;
  case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    return instructionAndCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
    return instructionOrCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
    return instructionXorCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
    return instructionLoadCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
    return instructionStoreCarry(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
    return instructionTestSet(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
    return instructionReset(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
    return instructionSet(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
    return instructionChange(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
    return instructionBit(toMemory<uint8>(address), toImmediate<uint3>(data));
  case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
  case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
    return instructionJump((uint4)data, toImmediate<uint32>(address));
  case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
  case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
    return instructionCall((uint4)data, toImmediate<uint32>(address));
  case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
  case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
    return instructionReturn((uint4)data);
  }
}
