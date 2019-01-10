template<typename Target>
auto TLCS900H::toSigned(Target target) -> int32 {
  if constexpr(Target::bits() ==  8) return  (int8)target;
  if constexpr(Target::bits() == 16) return (int16)target;
  if constexpr(Target::bits() == 32) return (int32)target;
  return Undefined;
}

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

template<typename Target, typename Source>
auto TLCS900H::instructionDivide(Target target, Source source) -> void {
  //TODO: division by zero
  store(expand(target), load(target) / load(source));
}

template<typename Target, typename Source>
auto TLCS900H::instructionDivideSigned(Target target, Source source) -> void {
  //TODO: division by zero
  store(expand(target), toSigned(load(target)) / toSigned(load(source)));
}

template<typename Target, typename Source>
auto TLCS900H::instructionExchange(Target target, Source source) -> void {
  auto data = load(target);
  store(target, load(source));
  store(source, data);
}

auto TLCS900H::instructionHalt() -> void {
  setHalted(true);
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

template<typename Target, typename Source>
auto TLCS900H::instructionLoad(Target target, Source source) -> void {
  store(target, load(source));
}

//reverse all bits in a 16-bit register
auto TLCS900H::instructionMirror(Register<uint16> register) -> void {
  auto data = load(register);
  uint8 lo = (data.byte(0) * 0x80200802ull & 0x884422110ull) * 0x101010101ull >> 32;
  uint8 hi = (data.byte(1) * 0x80200802ull & 0x884422110ull) * 0x101010101ull >> 32;
  store(register, lo << 8 | hi << 0);
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
  VF = uint32(~(target ^ source) & (target ^ result)).negative();
  ZF = result.zero();
  SF = result.negative();
}

template<typename Target, typename Source>
auto TLCS900H::instructionMultiplySigned(Target target, Source source) -> void {
  store(expand(target), toSigned(load(target)) * toSigned(load(source)));
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

template<typename Target>
auto TLCS900H::instructionPop(Target target) -> void {
  pop(target);
}

template<typename Source>
auto TLCS900H::instructionPush(Source source) -> void {
  push(source);
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

auto TLCS900H::instructionSoftwareInterrupt(uint3 interrupt) -> void {
  //TODO
}

template<typename Target, typename Source>
auto TLCS900H::instructionSubtract(Target target, Source source) -> void {
  store(target, algorithmSubtract(load(target), load(source)));
}

template<typename Target, typename Source>
auto TLCS900H::instructionSubtractCarry(Target target, Source source) -> void {
  store(target, algorithmSubtract(load(target), load(source), CF));
}

template<typename Target, typename Source>
auto TLCS900H::instructionXor(Target target, Source source) -> void {
  store(target, algorithmXor(load(target), load(source)));
}
