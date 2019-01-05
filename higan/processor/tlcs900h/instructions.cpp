template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionAdd(Target target, Source source) -> void {
  write<Size>(target, algorithmAdd<Size>(read<Size>(target), read<Size>(source)));
}

template<typename Size, typename Target, typename Source>
auto TLCS900H::instructionAddCarry(Target target, Source source) -> void {
  write<Size>(target, algorithmAdd<Size>(read<Size>(target), read<Size>(source), CF));
}

auto TLCS900H::instructionComplementCarry() -> void {
  CF = !CF;
}

auto TLCS900H::instructionNoOperation() -> void {
}

auto TLCS900H::instructionSoftwareInterrupt(Immediate interrupt) -> void {
}
