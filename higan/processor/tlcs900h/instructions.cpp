template<typename Target, typename Source>
auto TLCS900H::instructionAdd(Target target, Source source) -> void {
  store(target, algorithmAdd(load(target), load(source)));
}

template<typename Target, typename Source>
auto TLCS900H::instructionAddCarry(Target target, Source source) -> void {
  store(target, algorithmAdd(load(target), load(source), carry()));
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

template<typename Target, typename Source>
auto TLCS900H::instructionCompare(Target target, Source source) -> void {
  algorithmSubtract(load(target), load(source));
}

auto TLCS900H::instructionComplementCarry() -> void {
  setCarry(!carry());
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

auto TLCS900H::instructionReturnInterrupt() -> void {
  pop(SR);
  pop(PC);
  //TODO: decrement INTNEST here
}

template<typename Target>
auto TLCS900H::instructionSetConditionCode(uint4 code, Target target) -> void {
  store(target, condition(code));
}

auto TLCS900H::instructionSetInterruptFlags(uint3 flags) -> void {
  setIFF(flags);
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
  store(target, algorithmSubtract(load(target), load(source), carry()));
}

template<typename Target, typename Source>
auto TLCS900H::instructionXor(Target target, Source source) -> void {
  store(target, algorithmXor(load(target), load(source)));
}
