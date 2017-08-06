auto ARM7TDMI::fetch() -> void {
  pipeline.execute = pipeline.decode;
  pipeline.decode = pipeline.fetch;

  uint sequential = Sequential;
  if(pipeline.nonsequential) {
    pipeline.nonsequential = false;
    sequential = Nonsequential;
  }

  uint mask = !cpsr().t ? 3 : 1;
  uint size = !cpsr().t ? Word : Half;

  r(15).data += size >> 3;
  pipeline.fetch.address = r(15) & ~mask;
  pipeline.fetch.instruction = read(Prefetch | size | sequential, pipeline.fetch.address);
}

auto ARM7TDMI::instruction() -> void {
  uint mask = !cpsr().t ? 3 : 1;
  uint size = !cpsr().t ? Word : Half;

  if(pipeline.reload) {
    pipeline.reload = false;
    r(15).data &= ~mask;
    pipeline.fetch.address = r(15) & ~mask;
    pipeline.fetch.instruction = read(Prefetch | size | Nonsequential, pipeline.fetch.address);
    fetch();
  }
  fetch();

  if(irq && !cpsr().i) {
    interrupt(PSR::IRQ, 0x18);
    if(cpsr().t) r(14).data += 2;
    return;
  }

  if(!cpsr().t) {
    if(!TST(pipeline.execute.instruction.bits(28,31))) return;
  } else {
  }
}

auto ARM7TDMI::interrupt(uint mode, uint32 address) -> void {
  auto psr = cpsr();
  cpsr().m = 0x10 | mode;
  spsr() = psr;
  cpsr().t = 0;
  if(cpsr().m == PSR::FIQ) cpsr().f = 1;
  cpsr().i = 1;
  r(14) = pipeline.decode.address;
  r(15) = address;
}
