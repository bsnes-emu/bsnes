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
    bool t = cpsr().t;
    interrupt(PSR::IRQ, 0x18);
    if(t) r(14).data += 2;
    return;
  }

  opcode = pipeline.execute.instruction;
  if(!cpsr().t) {
    if(!TST(opcode.bits(28,31))) return;
    uint12 index = (opcode & 0x0ff00000) >> 16 | (opcode & 0x000000f0) >> 4;
    armInstruction[index](opcode);
  } else {
    thumbInstruction[(uint16)opcode]();
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
    opcode.bits( 0,23),  /* displacement */ \
    opcode.bit (24)      /* link */
  for(uint4 displacementLo : range(16))
  for(uint4 displacementHi : range(16))
  for(uint1 link : range(2)) {
    auto opcode = pattern(".... 101? ???? ???? ???? ???? ???? ????")
                | displacementLo << 4 | displacementHi << 20 | link << 24;
    bind(opcode, Branch);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3)   /* m */
  {
    auto opcode = pattern(".... 0001 0010 ---- ---- ---- 0001 ????");
    bind(opcode, BranchExchangeRegister);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 7),  /* immediate */ \
    opcode.bits( 8,11),  /* shift */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* save */ \
    opcode.bits(21,24)   /* mode */
  for(uint4 shiftHi : range(16))
  for(uint1 save : range(2))
  for(uint4 mode : range(16)) {
    if(mode >= 8 && mode <= 11 && !save) continue;  //TST, TEQ, CMP, CMN
    auto opcode = pattern(".... 001? ???? ???? ???? ???? ???? ????") | shiftHi << 4 | save << 20 | mode << 21;
    bind(opcode, DataImmediate);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits( 5, 6),  /* type */ \
    opcode.bits( 7,11),  /* shift */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* save */ \
    opcode.bits(21,24)   /* mode */
  for(uint2 type : range(4))
  for(uint1 shiftLo : range(2))
  for(uint1 save : range(2))
  for(uint4 mode : range(16)) {
    if(mode >= 8 && mode <= 11 && !save) continue;  //TST, TEQ, CMP, CMN
    auto opcode = pattern(".... 000? ???? ???? ???? ???? ???0 ????") | type << 5 | shiftLo << 7 | save << 20 | mode << 21;
    bind(opcode, DataImmediateShift);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits( 5, 6),  /* type */ \
    opcode.bits( 8,11),  /* s */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* save */ \
    opcode.bits(21,24)   /* mode */
  for(uint2 type : range(4))
  for(uint1 save : range(2))
  for(uint4 mode : range(16)) {
    if(mode >= 8 && mode <= 11 && !save) continue;  //TST, TEQ, CMP, CMN
    auto opcode = pattern(".... 000? ???? ???? ???? ???? 0??1 ????") | type << 5 | save << 20 | mode << 21;
    bind(opcode, DataRegisterShift);
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
    opcode.bits( 0,11),  /* immediate */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* mode */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (22),     /* byte */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint4 immediatePart : range(16))
  for(uint1 mode : range(2))
  for(uint1 writeback : range(2))
  for(uint1 byte : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 010? ???? ???? ???? ???? ???? ????")
                | immediatePart << 4 | mode << 20 | writeback << 21 | byte << 22 | up << 23 | pre << 24;
    bind(opcode, MoveImmediateOffset);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0,15),  /* list */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* mode */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (22),     /* type */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint4 listPart : range(16))
  for(uint1 mode : range(2))
  for(uint1 writeback : range(2))
  for(uint1 type : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 100? ???? ???? ???? ???? ???? ????")
                | listPart << 4 | mode << 20 | writeback << 21 | type << 22 | up << 23 | pre << 24;
    bind(opcode, MoveMultiple);
  }
  #undef arguments

  #define arguments \
    opcode.bits( 0, 3),  /* m */ \
    opcode.bits( 5, 6),  /* type */ \
    opcode.bits( 7,11),  /* shift */ \
    opcode.bits(12,15),  /* d */ \
    opcode.bits(16,19),  /* n */ \
    opcode.bit (20),     /* mode */ \
    opcode.bit (21),     /* writeback */ \
    opcode.bit (22),     /* byte */ \
    opcode.bit (23),     /* up */ \
    opcode.bit (24)      /* pre */
  for(uint2 type : range(4))
  for(uint1 shiftLo : range(2))
  for(uint1 mode : range(2))
  for(uint1 writeback : range(2))
  for(uint1 byte : range(2))
  for(uint1 up : range(2))
  for(uint1 pre : range(2)) {
    auto opcode = pattern(".... 011? ???? ???? ???? ???? ???0 ????")
                | type << 5 | shiftLo << 7 | mode << 20 | writeback << 21 | byte << 22 | up << 23 | pre << 24;
    bind(opcode, MoveRegisterOffset);
  }
  #undef arguments

  #define arguments \
    opcode.bits(12,15),  /* d */ \
    opcode.bit (22)      /* mode */
  for(uint1 mode : range(2)) {
    auto opcode = pattern(".... 0001 0?00 ---- ???? ---- 0000 ----") | mode << 22;
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

  #define arguments \
    opcode.bits( 0,23)  /* immediate */
  for(uint4 immediateLo : range(16))
  for(uint4 immediateHi : range(16)) {
    auto opcode = pattern(".... 1111 ???? ???? ???? ???? ???? ????") | immediateLo << 4 | immediateHi << 20;
    bind(opcode, SoftwareInterrupt);
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
  for(uint3 m : range(8))
  for(uint4 mode : range(16)) {
    auto opcode = pattern("0100 00?? ???? ????") | d << 0 | m << 3 | mode << 6;
    bind(opcode, ALU, d, m, mode);
  }

  for(uint4 d : range(16))
  for(uint4 m : range(16))
  for(uint2 mode : range(4)) {
    if(mode == 3) continue;
    auto opcode = pattern("0100 01?? ???? ????") | d.bits(0,2) << 0 | m << 3 | d.bit(3) << 7 | mode << 8;
    bind(opcode, ALUExtended, d, m, mode);
  }

  for(uint8 immediate : range(256))
  for(uint3 d : range(8))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("1010 ???? ???? ????") | immediate << 0 | d << 8 | mode << 11;
    bind(opcode, AddRegister, immediate, d, mode);
  }

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint3 immediate : range(8))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("0001 11?? ???? ????") | d << 0 | n << 3 | immediate << 6 | mode << 9;
    bind(opcode, AdjustImmediate, d, n, immediate, mode);
  }

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint3 m : range(8))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("0001 10?? ???? ????") | d << 0 | n << 3 | m << 6 | mode << 9;
    bind(opcode, AdjustRegister, d, n, m, mode);
  }

  for(uint7 immediate : range(128))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("1011 0000 ???? ????") | immediate << 0 | mode << 7;
    bind(opcode, AdjustStack, immediate, mode);
  }

  for(uint3 _ : range(8))
  for(uint4 m : range(16)) {
    auto opcode = pattern("0100 0111 0??? ?---") | _ << 0 | m << 3;
    bind(opcode, BranchExchange, m);
  }

  for(uint11 displacement : range(2048)) {
    auto opcode = pattern("1111 0??? ???? ????") | displacement << 0;
    bind(opcode, BranchFarPrefix, displacement);
  }

  for(uint11 displacement : range(2048)) {
    auto opcode = pattern("1111 1??? ???? ????") | displacement << 0;
    bind(opcode, BranchFarSuffix, displacement);
  }

  for(uint11 displacement : range(2048)) {
    auto opcode = pattern("1110 0??? ???? ????") | displacement << 0;
    bind(opcode, BranchNear, displacement);
  }

  for(uint8 displacement : range(256))
  for(uint4 condition : range(16)) {
    if(condition == 15) continue;  //BNV
    auto opcode = pattern("1101 ???? ???? ????") | displacement << 0 | condition << 8;
    bind(opcode, BranchTest, displacement, condition);
  }

  for(uint8 immediate : range(256))
  for(uint3 d : range(8))
  for(uint2 mode : range(4)) {
    auto opcode = pattern("001? ???? ???? ????") | immediate << 0 | d << 8 | mode << 11;
    bind(opcode, Immediate, immediate, d, mode);
  }

  for(uint8 displacement : range(256))
  for(uint3 d : range(8)) {
    auto opcode = pattern("0100 1??? ???? ????") | displacement << 0 | d << 8;
    bind(opcode, LoadLiteral, displacement, d);
  }

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint5 immediate : range(32))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("0111 ???? ???? ????") | d << 0 | n << 3 | immediate << 6 | mode << 11;
    bind(opcode, MoveByteImmediate, d, n, immediate, mode);
  }

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint5 immediate : range(32))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("1000 ???? ???? ????") | d << 0 | n << 3 | immediate << 6 | mode << 11;
    bind(opcode, MoveHalfImmediate, d, n, immediate, mode);
  }

  for(uint8 list : range(256))
  for(uint3 n : range(8))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("1100 ???? ???? ????") | list << 0 | n << 8 | mode << 11;
    bind(opcode, MoveMultiple, list, n, mode);
  }

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint3 m : range(8))
  for(uint3 mode : range(8)) {
    auto opcode = pattern("0101 ???? ???? ????") | d << 0 | n << 3 | m << 6 | mode << 9;
    bind(opcode, MoveRegisterOffset, d, n, m, mode);
  }

  for(uint8 immediate : range(256))
  for(uint3 d : range(8))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("1001 ???? ???? ????") | immediate << 0 | d << 8 | mode << 11;
    bind(opcode, MoveStack, immediate, d, mode);
  }

  for(uint3 d : range(8))
  for(uint3 n : range(8))
  for(uint5 offset : range(32))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("0110 ???? ???? ????") | d << 0 | n << 3 | offset << 6 | mode << 11;
    bind(opcode, MoveWordImmediate, d, n, offset, mode);
  }

  for(uint3 d : range(8))
  for(uint3 m : range(8))
  for(uint5 immediate : range(32))
  for(uint2 mode : range(4)) {
    if(mode == 3) continue;
    auto opcode = pattern("000? ???? ???? ????") | d << 0 | m << 3 | immediate << 6 | mode << 11;
    bind(opcode, ShiftImmediate, d, m, immediate, mode);
  }

  for(uint8 immediate : range(256)) {
    auto opcode = pattern("1101 1111 ???? ????") | immediate << 0;
    bind(opcode, SoftwareInterrupt, immediate);
  }

  for(uint8 list : range(256))
  for(uint1 lrpc : range(2))
  for(uint1 mode : range(2)) {
    auto opcode = pattern("1011 ?10? ???? ????") | list << 0 | lrpc << 8 | mode << 11;
    bind(opcode, StackMultiple, list, lrpc, mode);
  }

  #undef bind
  #undef pattern
}
