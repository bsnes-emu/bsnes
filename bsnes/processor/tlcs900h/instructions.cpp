template<typename Target, typename Source>
auto TLCS900H::instructionAdd(Target target, Source source) -> void {
  store(target, algorithmAdd(load(target), load(source)));
}

template<typename Target, typename Source>
auto TLCS900H::instructionAddCarry(Target target, Source source) -> void {
  store(target, algorithmAdd(load(target), load(source), CF));
}

template<typename Target, typename Source>
auto TLCS900H::instructionAnd(Target target, Source source) -> void {
  store(target, algorithmAnd(load(target), load(source)));
}

template<typename Source, typename Offset>
auto TLCS900H::instructionAndCarry(Source source, Offset offset) -> void {
  if constexpr(Source::bits == 8 && is_same_v<Offset, Register<uint8>>) { if(load(offset).bit(3)) return (void)Undefined; }
  CF &= load(source).bit(load(offset) & Source::bits - 1);
}

template<typename Source, typename Offset>
auto TLCS900H::instructionBit(Source source, Offset offset) -> void {
  NF = 0;
  VF = Undefined;
  HF = 1;
  ZF = !load(source).bit(load(offset) & Source::bits - 1);
  SF = Undefined;
}

auto TLCS900H::instructionBitSearch1Backward(Register<uint16> register) -> void {
  auto value = load(register);
  for(uint index : reverse(range(16))) {
    if(value.bit(index)) return VF = 1, store(A, index);
  }
  VF = 0;
}

auto TLCS900H::instructionBitSearch1Forward(Register<uint16> register) -> void {
  auto value = load(register);
  for(uint index : range(16)) {
    if(value.bit(index)) return VF = 1, store(A, index);
  }
  VF = 0;
}

template<typename Source>
auto TLCS900H::instructionCall(uint4 code, Source source) -> void {
  auto address = load(source);
  if(condition(code)) push(PC), store(PC, address);
}

template<typename Source>
auto TLCS900H::instructionCallRelative(Source displacement) -> void {
  push(PC);
  store(PC, load(PC) + load(displacement));
}

template<typename Target, typename Offset>
auto TLCS900H::instructionChange(Target target, Offset offset) -> void {
  auto result = load(target);
  result.bit(load(offset) & Target::bits - 1) ^= 1;
  store(target, result);
}

template<typename Size, int Adjust, typename Target>
auto TLCS900H::instructionCompare(Target target) -> void {
  auto source = toRegister3<uint32>(r.prefix);
  auto cf = CF;  //CF is not modified; but algorithmSubtract will modify it
  algorithmSubtract(load(target), load(toMemory<Size>(load(source))));
  store(source, load(source) + Adjust);
  store(BC, load(BC) - 1);
  CF = cf;
  VF = load(BC) == 0;
}

template<typename Size, int Adjust, typename Target>
auto TLCS900H::instructionCompareRepeat(Target target) -> void {
  do { instructionCompare<Size, Adjust>(target); } while(VF && !ZF);
}

template<typename Target, typename Source>
auto TLCS900H::instructionCompare(Target target, Source source) -> void {
  algorithmSubtract(load(target), load(source));
}

template<typename Target>
auto TLCS900H::instructionComplement(Target target) -> void {
  store(target, ~load(target));
  NF = 1;
  HF = 1;
}

auto TLCS900H::instructionDecimalAdjustAccumulator(Register<uint8> register) -> void {
  auto value = load(register);
  if(CF || (uint8)value > 0x99) value += NF ? -0x60 : 0x60, CF = 1;
  if(HF || (uint4)value > 0x09) value += NF ? -0x06 : 0x06;
  PF = parity(value);
  HF = uint8(value ^ load(register)).bit(4);
  ZF = value == 0;
  SF = value.bit(-1);
  store(register, value);
}

template<typename Target, typename Source>
auto TLCS900H::instructionDecrement(Target target, Source source) -> void {
  auto immediate = load(source);
  if(!immediate) immediate = 8;
  store(target, algorithmDecrement(load(target), immediate));
}

template<typename Target, typename Offset>
auto TLCS900H::instructionDecrementJumpNotZero(Target target, Offset offset) -> void {
  auto result = load(target);
  store(target, --result);
  if(result) store(PC, load(PC) + load(offset));
}

template<typename Target, typename Source>
auto TLCS900H::instructionDivide(Target target, Source source) -> void {
  using T = typename Target::type;
  using E = Natural<2 * T::bits()>;
  auto dividend  = load(expand(target));
  auto divisor   = load(source);
  auto quotient  = divisor ? E(dividend / divisor) : E(T(~(dividend >> T::bits())));
  auto remainder = divisor ? E(dividend % divisor) : E(T(dividend));
  store(expand(target), T(remainder) << T::bits() | T(quotient));
  VF = !divisor || remainder >> T::bits();
}

template<typename Target, typename Source>
auto TLCS900H::instructionDivideSigned(Target target, Source source) -> void {
  using T = typename Target::type;
  using E = Natural<2 * T::bits()>;
  auto dividend  = load(expand(target)).integer();
  auto divisor   = load(source).integer();
  auto quotient  = divisor ? E(dividend / divisor) : E(T(~(dividend >> T::bits())));
  auto remainder = divisor ? E(dividend % divisor) : E(T(dividend));
  store(expand(target), T(remainder) << T::bits() | T(quotient));
  VF = !divisor || remainder >> T::bits();
}

template<typename Target, typename Source>
auto TLCS900H::instructionExchange(Target target, Source source) -> void {
  auto data = load(target);
  store(target, load(source));
  store(source, data);
}

template<typename Target>
auto TLCS900H::instructionExtendSign(Target target) -> void {
  store(target, load(shrink(target)).integer());
}

template<typename Target>
auto TLCS900H::instructionExtendZero(Target target) -> void {
  store(target, load(shrink(target)));
}

auto TLCS900H::instructionHalt() -> void {
  r.halted = true;
}

template<typename Target, typename Source>
auto TLCS900H::instructionIncrement(Target target, Source source) -> void {
  auto immediate = load(source);
  if(!immediate) immediate = 8;
  store(target, algorithmIncrement(load(target), immediate));
}

template<typename Source>
auto TLCS900H::instructionJump(uint4 code, Source source) -> void {
  auto address = load(source);
  if(condition(code)) store(PC, address);
}

template<typename Source>
auto TLCS900H::instructionJumpRelative(uint4 code, Source displacement) -> void {
  if(condition(code)) store(PC, load(PC) + load(displacement));
}

template<typename Target, typename Offset>
auto TLCS900H::instructionLink(Target target, Offset offset) -> void {
  push(target);
  store(target, load(XSP));
  store(XSP, load(XSP) + load(offset));
}

template<typename Target, typename Source>
auto TLCS900H::instructionLoad(Target target, Source source) -> void {
  store(target, load(source));
}

template<typename Source, typename Offset>
auto TLCS900H::instructionLoadCarry(Source source, Offset offset) -> void {
  if constexpr(Source::bits == 8 && is_same_v<Offset, Register<uint8>>) { if(load(offset).bit(3)) return (void)Undefined; }
  CF = load(source).bit(load(offset) & Source::bits - 1);
}

template<typename Size, int Adjust> auto TLCS900H::instructionLoad() -> void {
  auto target = (uint3)r.prefix == 5 ? XIX : XDE;
  auto source = (uint3)r.prefix == 5 ? XIY : XHL;
  store(toMemory<Size>(load(target)), load(toMemory<Size>(load(source))));
  store(target, load(target) + Adjust);
  store(source, load(source) + Adjust);
  store(BC, load(BC) - 1);
  NF = 0;
  VF = load(BC) == 0;
  HF = 0;
}

template<typename Size, int Adjust> auto TLCS900H::instructionLoadRepeat() -> void {
  do { instructionLoad<Size, Adjust>(); } while(VF);
}

//reverse all bits in a 16-bit register
//note: an 8-bit lookup table is faster (when in L1/L2 cache), but much more code
auto TLCS900H::instructionMirror(Register<uint16> register) -> void {
  auto data = load(register);
  data = data << 1 & 0xaaaa | data >> 1 & 0x5555;
  data = data << 2 & 0xcccc | data >> 2 & 0x3333;
  data = data << 4 & 0xf0f0 | data >> 4 & 0x0f0f;
  store(register, data << 8 | data >> 8);
}

template<uint Modulo, typename Target, typename Source>
auto TLCS900H::instructionModuloDecrement(Target target, Source source) -> void {
  auto result = load(target);
  auto number = load(source);
  if(bit::count(number) != 1) return (void)Undefined;  //must be power of two (what about number==1?)
  if(result % number == 0) {
    store(target, result + (number - Modulo));
  } else {
    store(target, result - Modulo);
  }
}

template<uint Modulo, typename Target, typename Source>
auto TLCS900H::instructionModuloIncrement(Target target, Source source) -> void {
  auto result = load(target);
  auto number = load(source);
  if(bit::count(number) != 1) return (void)Undefined;  //must be power of two (what about number==1?)
  if(result % number == number - Modulo) {
    store(target, result - (number - Modulo));
  } else {
    store(target, result + Modulo);
  }
}

template<typename Target, typename Source>
auto TLCS900H::instructionMultiply(Target target, Source source) -> void {
  store(expand(target), load(target) * load(source));
}

auto TLCS900H::instructionMultiplyAdd(Register<uint16> register) -> void {
  auto xde = toMemory<int16>(load(XDE));
  auto xhl = toMemory<int16>(load(XHL));

  auto source = load(expand(register));
  auto target = load(xde) * load(xhl);
  store(expand(register), source + target);
  store(XHL, load(XHL) - 2);

  auto result = load(expand(register));
  VF = uint32((target ^ result) & (source ^ result)).bit(-1);
  ZF = result == 0;
  SF = result.bit(-1);
}

template<typename Target, typename Source>
auto TLCS900H::instructionMultiplySigned(Target target, Source source) -> void {
  store(expand(target), load(target).integer() * load(source).integer());
}

template<typename Target>
auto TLCS900H::instructionNegate(Target target) -> void {
  store(target, algorithmSubtract(typename Target::type{0}, load(target)));
}

auto TLCS900H::instructionNoOperation() -> void {
}

template<typename Target, typename Source>
auto TLCS900H::instructionOr(Target target, Source source) -> void {
  store(target, algorithmOr(load(target), load(source)));
}

template<typename Source, typename Offset>
auto TLCS900H::instructionOrCarry(Source source, Offset offset) -> void {
  if constexpr(Source::bits == 8 && is_same_v<Offset, Register<uint8>>) { if(load(offset).bit(3)) return (void)Undefined; }
  CF |= load(source).bit(load(offset) & Source::bits - 1);
}

//increments odd addresses only to ensure they are even (16-bit aligned)
template<typename Target>
auto TLCS900H::instructionPointerAdjustAccumulator(Target target) -> void {
  auto result = load(target);
  store(target, result + result.bit(0));
}

template<typename Target>
auto TLCS900H::instructionPop(Target target) -> void {
  pop(target);
}

template<typename Source>
auto TLCS900H::instructionPush(Source source) -> void {
  push(source);
}

template<typename Target, typename Offset>
auto TLCS900H::instructionReset(Target target, Offset offset) -> void {
  auto result = load(target);
  result.bit(load(offset) & Target::bits - 1) = 0;
  store(target, result);
}

auto TLCS900H::instructionReturn(uint4 code) -> void {
  if(condition(code)) pop(PC);
}

template<typename Source>
auto TLCS900H::instructionReturnDeallocate(Source displacement) -> void {
  pop(PC);
  store(XSP, load(XSP) + load(displacement));
}

auto TLCS900H::instructionReturnInterrupt() -> void {
  pop(SR);
  pop(PC);
  store(INTNEST, load(INTNEST) - 1);
}

template<typename LHS, typename RHS>
auto TLCS900H::instructionRotateLeftDigit(LHS lhs, RHS rhs) -> void {
  auto lvalue = load(lhs);
  auto rvalue = load(rhs);
  auto Lvalue = lvalue;
  lvalue.bits(0,3) = rvalue.bits(4,7);
  rvalue.bits(4,7) = rvalue.bits(0,3);
  rvalue.bits(0,3) = Lvalue.bits(0,3);
  store(lhs, lvalue);
  store(rhs, rvalue);
  NF = 0;
  PF = parity(lvalue);
  HF = 0;
  ZF = lvalue == 0;
  SF = lvalue.bit(-1);
}

template<typename Target, typename Amount>
auto TLCS900H::instructionRotateLeft(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    uint cf = result.bit(-1);
    result = result << 1 | CF;
    CF = cf;
  }
  store(target, algorithmRotated(result));
}

template<typename Target, typename Amount>
auto TLCS900H::instructionRotateLeftWithoutCarry(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    CF = result.bit(-1);
    result = result << 1 | CF;
  }
  store(target, algorithmRotated(result));
}

template<typename LHS, typename RHS>
auto TLCS900H::instructionRotateRightDigit(LHS lhs, RHS rhs) -> void {
  auto lvalue = load(lhs);
  auto rvalue = load(rhs);
  auto Rvalue = rvalue;
  rvalue.bits(0,3) = rvalue.bits(4,7);
  rvalue.bits(4,7) = lvalue.bits(0,3);
  lvalue.bits(0,3) = Rvalue.bits(0,3);
  store(lhs, lvalue);
  store(rhs, rvalue);
  NF = 0;
  PF = parity(lvalue);
  HF = 0;
  ZF = lvalue == 0;
  SF = lvalue.bit(-1);
}

template<typename Target, typename Amount>
auto TLCS900H::instructionRotateRight(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    uint cf = result.bit(0);
    result = CF << Target::bits - 1 | result >> 1;
    CF = cf;
  }
  store(target, algorithmRotated(result));
}

template<typename Target, typename Amount>
auto TLCS900H::instructionRotateRightWithoutCarry(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    CF = result.bit(0);
    result = CF << Target::bits - 1 | result >> 1;
  }
  store(target, algorithmRotated(result));
}

template<typename Target, typename Offset>
auto TLCS900H::instructionSet(Target target, Offset offset) -> void {
  auto result = load(target);
  result.bit(load(offset) & Target::bits - 1) = 1;
  store(target, result);
}

template<typename Target>
auto TLCS900H::instructionSetConditionCode(uint4 code, Target target) -> void {
  store(target, condition(code));
}

auto TLCS900H::instructionSetFlag(uint1& flag, uint1 value) -> void {
  flag = value;
}

auto TLCS900H::instructionSetInterruptFlipFlop(uint3 value) -> void {
  IFF = value;
}

auto TLCS900H::instructionSetRegisterFilePointer(uint2 value) -> void {
  RFP = value;
}

template<typename Target, typename Amount>
auto TLCS900H::instructionShiftLeftArithmetic(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    CF = result.bit(-1);
    result = result << 1;
  }
  store(target, algorithmShifted(result));
}

template<typename Target, typename Amount>
auto TLCS900H::instructionShiftLeftLogical(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    CF = result.bit(-1);
    result = result << 1;
  }
  store(target, algorithmShifted(result));
}

template<typename Target, typename Amount>
auto TLCS900H::instructionShiftRightArithmetic(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    CF = result.bit(0);
    result = result >> 1;
    result.bit(-1) = result.bit(-2);
  }
  store(target, algorithmShifted(result));
}

template<typename Target, typename Amount>
auto TLCS900H::instructionShiftRightLogical(Target target, Amount amount) -> void {
  auto result = load(target);
  auto length = load(amount).clip(4);
  for(uint n : range(length ? length : 16)) {
    CF = result.bit(0);
    result = result >> 1;
  }
  store(target, algorithmShifted(result));
}

template<typename Target, typename Offset>
auto TLCS900H::instructionStoreCarry(Target target, Offset offset) -> void {
  if constexpr(Target::bits == 8) { if(load(offset).bit(3)) return; }  //unlike other *CF instructions, STCF behavior is defined
  auto result = load(target);
  result.bit(load(offset)) = CF;
  store(target, result);
}

auto TLCS900H::instructionSoftwareInterrupt(uint3 interrupt) -> void {
  push(PC);
  push(SR);
  store(PC, load(Memory<uint24>{0xffff00 + interrupt * 4}));
  store(INTNEST, load(INTNEST) + 1);  //note: not confirmed behavior
}

template<typename Target, typename Source>
auto TLCS900H::instructionSubtract(Target target, Source source) -> void {
  store(target, algorithmSubtract(load(target), load(source)));
}

//note: the TLCS900/H manual states this is subtract-with-carry, but it isn't
template<typename Target, typename Source>
auto TLCS900H::instructionSubtractBorrow(Target target, Source source) -> void {
  store(target, algorithmSubtract(load(target), load(source), CF));
}

template<typename Target, typename Offset>
auto TLCS900H::instructionTestSet(Target target, Offset offset) -> void {
  auto result = load(target);
  NF = 0;
  VF = Undefined;
  HF = 1;
  ZF = result.bit(load(offset) & Target::bits - 1);
  SF = Undefined;
  result.bit(load(offset) & Target::bits - 1) = 1;
  store(target, result);
}

template<typename Target>
auto TLCS900H::instructionUnlink(Target target) -> void {
  store(XSP, load(target));
  pop(target);
}

template<typename Target, typename Source>
auto TLCS900H::instructionXor(Target target, Source source) -> void {
  store(target, algorithmXor(load(target), load(source)));
}

template<typename Source, typename Offset>
auto TLCS900H::instructionXorCarry(Source source, Offset offset) -> void {
  if constexpr(Source::bits == 8 && is_same_v<Offset, Register<uint8>>) { if(load(offset).bit(3)) return (void)Undefined; }
  CF ^= load(source).bit(load(offset) & Source::bits - 1);
}
