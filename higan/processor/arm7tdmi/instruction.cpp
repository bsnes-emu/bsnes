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

  opcode = pipeline.execute.instruction;
  if(!cpsr().t) {
    if(!TST(opcode.bits(28,31))) return;
    armInstruction[(opcode & 0x0ff00000) >> 16 | (opcode & 0x000000f0) >> 4](opcode);
  } else {
    thumbInstruction[opcode & 0xffff]();
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

auto ARM7TDMI::armInitialize() -> void {
  #define bind(id, name, ...) { \
    uint index = (id & 0x0ff00000) >> 16 | (id & 0x000000f0) >> 4; \
    assert(!armInstruction[index]); \
    armInstruction[index] = [&](uint32 opcode) { return armInstruction##name(arguments); }; \
    armDisassemble[index] = [&](uint32 opcode) { return armDisassemble##name(arguments); }; \
  }

  #define pattern(s) \
    std::integral_constant<uint32_t, bit::test(s)>::value

  #define arguments \
    opcode.bits( 0, 3)   /* m */
  {
    auto opcode = pattern(".... 0001 0010 ---- ---- ---- 0001 ????");
    bind(opcode, BranchExchangeRegister);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3) << 0 | opcode.bits( 8,11) << 4,  /* immediate */ \
    opcode.bit ( 5),     /* half */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint1 half : range(2))
  for(uint1 writeback : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 000? ?1?1 ???? ???? ???? 11?1 ????") | half << 5 | writeback << 21 | up << 23 | pre << 24;
    bind(opcode, LoadImmediate);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bit ( 5),     /* half */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint1 half : range(2))
  for(uint1 writeback : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 000? ?0?1 ???? ???? ---- 11?1 ????") | half << 5 | writeback << 21 | up << 23 | pre << 24;
    bind(opcode, LoadRegister);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (22)      /* byte */
  for(uint1 byte : range(2)) {
    auto opcode = pattern(".... 0001 0?00 ???? ???? ---- 1001 ????") | byte << 22;
    bind(opcode, MemorySwap);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3) << 0 | opcode.bits( 8,11) << 4,  /* immediate */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* mode */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint1 mode : range(2))
  for(uint1 writeback : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 000? ?1?? ???? ???? ???? 1011 ????") | mode << 20 | writeback << 21 | up << 23 | pre << 24;
    bind(opcode, MoveHalfImmediate);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* mode */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint1 mode : range(2))
  for(uint1 writeback : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 000? ?0?? ???? ???? ---- 1011 ????") | mode << 20 | writeback << 21 | up << 23 | pre << 24;
    bind(opcode, MoveHalfRegister);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* d */ \
    opcode.bit (22)      /* mode */
  for(uint1 mode : range(2)) {
    auto opcode = pattern(".... 0001 0?00 ---- ---- ---- 0000 ????") | mode << 22;
    bind(opcode, MoveToRegisterFromStatus);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 7),  /* immediate */ \
    opcode.bits( 8,11),  /* rotate */ \
    opcode.bits(16,19),  /* field */ \
    opcode.bit (22)      /* mode */
  for(uint4 immediateHi : range(16))
  for(uint1 mode : range(2)) {
    auto opcode = pattern(".... 0011 0?10 ???? ---- ???? ???? ????") | immediateHi << 4 | mode << 22;
    bind(opcode, MoveToStatusFromImmediate);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits(16,19),  /* field */ \
    opcode.bit (22)      /* mode */
  for(uint1 mode : range(2)) {
    auto opcode = pattern(".... 0001 0?10 ???? ---- ---- 0000 ????") | mode << 22;
    bind(opcode, MoveToStatusFromRegister);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits( 8,11),  /* s */ \
    opcode.bits(12,15),  /* n */ \
    opcode.bits(16,19),  /* d */ \
    opcode.bit (20),     /* save */ \
    opcode.bit (21)      /* accumulate */
  for(uint1 save : range(2))
  for(uint1 accumulate : range(2)) {
    auto opcode = pattern(".... 0000 00?? ???? ???? ???? 1001 ????") | save << 20 | accumulate << 21;
    bind(opcode, Multiply);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits( 8,11),  /* s */ \
    opcode.bits(12,15),  /* l */ \
    opcode.bits(16,19),  /* h */ \
    opcode.bit (20),     /* save */ \
    opcode.bit (21),     /* accumulate */ \
    opcode.bit (22)      /* sign */
  for(uint1 save : range(2))
  for(uint1 accumulate : range(2))
  for(uint1 sign : range(2)) {
    auto opcode = pattern(".... 0000 1??? ???? ???? ???? 1001 ????") | save << 20 | accumulate << 21 | sign << 22;
    bind(opcode, MultiplyLong);
  }
  #undef arguments

  #undef bind
  #undef pattern
}

auto ARM7TDMI::thumbInitialize() -> void {
  #define bind(id, name, ...) { \
    assert(!thumbInstruction[id]); \
    thumbInstruction[id] = [=] { return thumbInstruction##name(__VA_ARGS__); }; \
    thumbDisassemble[id] = [=] { return thumbDisassemble##name(__VA_ARGS__); }; \
  }

  #define pattern(s) \
    std::integral_constant<uint16_t, bit::test(s)>::value

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint3 m : range(8))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("0001 10?? ???? ????") | d << 0 | n << 3 | m << 6 | mode << 9;
    bind(opcode, AdjustRegister, d, n, m, mode);
  }

  #undef bind
  #undef pattern
}
