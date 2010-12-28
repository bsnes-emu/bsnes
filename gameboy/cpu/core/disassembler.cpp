#ifdef CPU_CPP

string CPU::disassemble(uint16 pc) {
  return { hex<4>(pc), "  ", disassemble_opcode(pc) };
}

string CPU::disassemble_opcode(uint16 pc) {
  uint8 opcode = bus.read(pc);
  uint8 p0 = bus.read(pc + 1);
  uint8 p1 = bus.read(pc + 2);
  uint8 p2 = bus.read(pc + 3);

  switch(opcode) {
    case 0x00: return { "nop" };
    case 0x05: return { "dec  b" };
    case 0x06: return { "ld   b,$", hex<2>(p0) };
    case 0x0d: return { "dec  c" };
    case 0x0e: return { "ld   c,$", hex<2>(p0) };
    case 0x20: return { "jp   nz,$", hex<2>(p0) };
    case 0x21: return { "ld   hl,$", hex<2>(p1), hex<2>(p0) };
    case 0x32: return { "ldd  (hl),a" };
    case 0x3e: return { "ld   a,$", hex<2>(p0) };
    case 0xaf: return { "xor  a" };
    case 0xc3: return { "jp   $", hex<2>(p1), hex<2>(p0) };
  }

  return { "???? [", hex<2>(opcode), ",", hex<2>(p1), ",", hex<2>(p0), "]" };
}

#endif
