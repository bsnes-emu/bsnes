auto TLCS900H::disassemble() -> string {
  string output;
  output.append(hex(r.pc.l.l0, 6L), "  ");
  output.append(disassembleInstruction());
  output.size(-48);
  output.append("I", r.iff);
  output.append("R", r.rfp);
  output.append(r.s ? "S" : "s");
  output.append(r.z ? "Z" : "z");
  output.append(r.h ? "H" : "h");
  output.append(r.v ? "V" : "v");
  output.append(r.n ? "N" : "n");
  output.append(r.c ? "C" : "c");
  return output;
}

auto TLCS900H::disassembleInstruction() -> string {
  uint8 opcode[8] = {};
  string opRegister;
  string opSourceMemory;
  string opTargetMemory;
  auto pc = r.pc.l.l0;

  auto fetch8n = [&]() -> uint8 {
    return disassembleRead(pc++);
  };
  auto fetch16n = [&]() -> uint16 {
    uint16 data = fetch8n() << 0;
    return data | fetch8n() << 8;
  };
  auto fetch24n = [&]() -> uint24 {
    uint24 data  = fetch8n() <<  0;
           data |= fetch8n() <<  8;
    return data |= fetch8n() << 16;
  };
  auto fetch32n = [&]() -> uint32 {
    uint32 data  = fetch8n() <<  0;
           data |= fetch8n() <<  8;
           data |= fetch8n() << 16;
    return data |= fetch8n() << 24;
  };
  auto fetch8i = [&]() -> int8 { return (int8)fetch8n(); };
  auto fetch16i = [&]() -> int16 { return (int16)fetch16n(); };

  auto direct16n = [&]() -> string {
    return {"0x", hex(fetch16n(), 4L)};
  };
  auto direct24n = [&]() -> string {
    return {"0x", hex(fetch24n(), 6L)};
  };
  auto displacement16i = [&]() -> string {
    auto displacement = fetch16i();
    if(displacement < 0) return {"-0x", hex(abs(displacement), 4L)};
    return {"+0x", hex(displacement, 4L)};
  };
  auto displacementPC16i = [&]() -> string {
    auto displacement = fetch16i();
    return {"0x", hex(pc + displacement, 6L)};
  };
  auto immediate8n = [&]() -> string {
    return {"#0x", hex(fetch8n(), 2L)};
  };
  auto immediate16n = [&]() -> string {
    return {"#0x", hex(fetch16n(), 4L)};
  };
  auto indirect8n = [&]() -> string {
    return {"(0x", hex(fetch8n(), 2L), ")"};
  };

  #define op(name, ...) return {pad(name, -6), vector<string>{__VA_ARGS__}.merge(",")}
  #define bad() return {pad("???", -6), hex(opcode[0], 2L)}
  switch(opcode[0] = fetch8n()) {
  case 0x00: op("nop");
  case 0x01: bad();
  case 0x02: op("push", "sr");
  case 0x03: op("pop", "sr");
  case 0x04: bad();
  case 0x05: op("halt");
  case 0x06: {
    uint3 immediate = fetch8n();
    if(immediate == 7) op("di");
    op("ei", {"#", immediate});
  }
  case 0x07: op("reti");
  case 0x08: op("ld", indirect8n(), immediate8n());
  case 0x09: op("push", immediate8n());
  case 0x0a: op("ldw", indirect8n(), immediate16n());
  case 0x0b: op("push", immediate16n());
  case 0x0c: op("incf");
  case 0x0d: op("decf");
  case 0x0e: op("ret");
  case 0x0f: op("retd", displacement16i());
  case 0x10: op("rcf");
  case 0x11: op("scf");
  case 0x12: op("ccf");
  case 0x13: op("zcf");
  case 0x14: op("push", "a");
  case 0x15: op("pop", "a");
  case 0x16: op("ex", "f", "f'");
  case 0x17: {
    uint2 immediate = fetch8n();
    op("ldf", {"#", immediate});
  }
  case 0x18: op("push", "f");
  case 0x19: op("pop", "f");
  case 0x1a: op("jp", direct16n());
  case 0x1b: op("jp", direct24n());
  case 0x1c: op("call", direct16n());
  case 0x1d: op("call", direct24n());
  case 0x1e: op("calr", displacementPC16i());
  case 0x1f: bad();
  default: bad();
  }
  #undef bad

  #define bad() return {pad("???", -6), hex(opcode[0], 2L), " ", hex(opcode[1], 2L)}
  if(opRegister) switch(opcode[1] = fetch8n()) {
  default: bad();
  }
  #undef bad

  #define bad() return {pad("???", -6), hex(opcode[0], 2L), " ", hex(opcode[1], 2L)}
  if(opSourceMemory) switch(opcode[1] = fetch8n()) {
  default: bad();
  }
  #undef bad

  #define bad() return {pad("???", -6), hex(opcode[0], 2L), " ", hex(opcode[1], 2L)}
  if(opTargetMemory) switch(opcode[1] = fetch8n()) {
  default: bad();
  }
  #undef bad
  #undef op

  return {};
}
