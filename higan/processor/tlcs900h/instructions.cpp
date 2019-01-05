#define read read<Size>
#define write write<Size>
#define push push<Size>
#define pop pop<Size>
#define algorithm(name, ...) algorithm##name<Size>(__VA_ARGS__)

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionAdd(Target target, Source source) -> void {
  write(target, algorithm(Add, read(target), read(source)));
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionAddCarry(Target target, Source source) -> void {
  write(target, algorithm(Add, read(target), read(source), CF));
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionAnd(Target target, Source source) -> void {
  write(target, algorithm(And, read(target), read(source)));
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionCompare(Target target, Source source) -> void {
  algorithm(Subtract, read(target), read(source));
}

auto TLCS900H::instructionComplementCarry() -> void {
  CF = !CF;
}

auto TLCS900H::instructionNoOperation() -> void {
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionOr(Target target, Source source) -> void {
  write(target, algorithm(Or, read(target), read(source)));
}

template<typename Size, typename Target>
auto TLCS900H::instructionPop(Target target) -> void {
  write(target, pop());
}

template<typename Size, typename Source>
auto TLCS900H::instructionPush(Source source) -> void {
  push(read(source));
}

auto TLCS900H::instructionSoftwareInterrupt(Immediate interrupt) -> void {
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionSubtract(Target target, Source source) -> void {
  write(target, algorithm(Subtract, read(target), read(source)));
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionSubtractCarry(Target target, Source source) -> void {
  write(target, algorithm(Subtract, read(target), read(source), CF));
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionXor(Target target, Source source) -> void {
  write(target, algorithm(Xor, read(target), read(source)));
}

#undef read
#undef write
#undef push
#undef pop
#undef algorithm
