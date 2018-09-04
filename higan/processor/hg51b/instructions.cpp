auto HG51B::push() -> void {
  stack[7] = stack[6];
  stack[6] = stack[5];
  stack[5] = stack[4];
  stack[4] = stack[3];
  stack[3] = stack[2];
  stack[2] = stack[1];
  stack[1] = stack[0];
  stack[0] = r.pb << 8 | r.pc << 0;
}

auto HG51B::pull() -> void {
  auto pc  = stack[0];
  stack[0] = stack[1];
  stack[1] = stack[2];
  stack[2] = stack[3];
  stack[3] = stack[4];
  stack[4] = stack[5];
  stack[5] = stack[6];
  stack[6] = stack[7];
  stack[7] = 0x0000;

  r.pb = pc >> 8;
  r.pc = pc >> 0;
}

//Shift-A: math opcodes can shift A register prior to ALU operation
auto HG51B::sa() -> uint {
  static const uint shift[] = {0, 1, 8, 16};
  return r.a << shift[opcode.bits(8,9)];
}

//Register-or-Immediate: most opcodes can load from a register or immediate
auto HG51B::ri() -> uint {
  if(opcode.bit(10)) return opcode.bits(0,7);
  return readRegister(opcode.bits(0,7));
}

//New-PC: determine jump target address; opcode.d9 = long jump flag (1 = yes)
auto HG51B::np() -> void {
  if(opcode.bit(9)) r.pb = r.p;
  r.pc = opcode.bits(0,7);
}

auto HG51B::instruction() -> void {
  if((opcode & 0xffff) == 0x0000) {
    //0000 0000 0000 0000
    //nop
  }

  else if((opcode & 0xdd00) == 0x0800) {
    //00.0 10.0 .... ....
    //jump i
    if(opcode & 0x2000) push();
    np();
    step(2);
  }

  else if((opcode & 0xdd00) == 0x0c00) {
    //00.0 11.0 .... ....
    //jumpeq i
    if(r.z) {
      if(opcode & 0x2000) push();
      np();
      step(2);
    }
  }

  else if((opcode & 0xdd00) == 0x1000) {
    //00.1 00.0 .... ....
    //jumpge i
    if(r.c) {
      if(opcode & 0x2000) push();
      np();
      step(2);
    }
  }

  else if((opcode & 0xdd00) == 0x1400) {
    //00.1 01.0 .... ....
    //jumpmi i
    if(r.n) {
      if(opcode & 0x2000) push();
      np();
      step(2);
    }
  }

  else if((opcode & 0xffff) == 0x1c00) {
    //0001 1100 0000 0000
    //wait
    if(io.bus.enable) step(io.bus.pending);
  }

  else if((opcode & 0xfffe) == 0x2500) {
    //0010 0101 0000 000.
    //skiplt/skipge
    if(r.c == (opcode & 1)) {
      advance();
      step(1);
    }
  }

  else if((opcode & 0xfffe) == 0x2600) {
    //0010 0110 0000 000.
    //skipne/skipeq
    if(r.z == (opcode & 1)) {
      advance();
      step(1);
    }
  }

  else if((opcode & 0xfffe) == 0x2700) {
    //0010 0111 0000 000.
    //skipmi/skippl
    if(r.n == (opcode & 1)) {
      advance();
      step(1);
    }
  }

  else if((opcode & 0xffff) == 0x3c00) {
    //0011 1100 0000 0000
    //ret
    pull();
    step(2);
  }

  else if((opcode & 0xffff) == 0x4000) {
    //0100 0000 0000 0000
    //???
    r.busAddress++;
  }

  else if((opcode & 0xf800) == 0x4800) {
    //0100 1... .... ....
    //cmpr a<<n,ri
    int result = ri() - sa();
    r.n = result & 0x800000;
    r.z = (uint24)result == 0;
    r.c = result >= 0;
  }

  else if((opcode & 0xf800) == 0x5000) {
    //0101 0... .... ....
    //cmp a<<n,ri
    int result = sa() - ri();
    r.n = result & 0x800000;
    r.z = (uint24)result == 0;
    r.c = result >= 0;
  }

  else if((opcode & 0xfb00) == 0x5900) {
    //0101 1.01 .... ....
    //sxb
    r.a = (int8)ri();
  }

  else if((opcode & 0xfb00) == 0x5a00) {
    //0101 1.10 .... ....
    //sxw
    r.a = (int16)ri();
  }

  else if((opcode & 0xfb00) == 0x6000) {
    //0110 0.00 .... ....
    //ld a,ri
    r.a = ri();
  }

  else if((opcode & 0xfb00) == 0x6100) {
    //0110 0.01 .... ....
    //ld bus,ri
    r.busData = ri();
  }

  else if((opcode & 0xfb00) == 0x6300) {
    //0110 0.11 .... ....
    //ld p,ri
    r.p = ri();
  }

  else if((opcode & 0xfb00) == 0x6800) {
    //0110 1.00 .... ....
    //rdraml
    uint24 target = ri() + (opcode.bit(10) ? r.ramAddress : (uint24)0);
    if(target < 0xc00) r.ramData.byte(0) = dataRAM[target];
  }

  else if((opcode & 0xfb00) == 0x6900) {
    //0110 1.01 .... ....
    //rdramh
    uint24 target = ri() + (opcode.bit(10) ? r.ramAddress : (uint24)0);
    if(target < 0xc00) r.ramData.byte(1) = dataRAM[target];
  }

  else if((opcode & 0xfb00) == 0x6a00) {
    //0110 1.10 .... ....
    //rdramb
    uint24 target = ri() + (opcode.bit(10) ? r.ramAddress : (uint24)0);
    if(target < 0xc00) r.ramData.byte(2) = dataRAM[target];
  }

  else if((opcode & 0xffff) == 0x7000) {
    //0111 0000 0000 0000
    //rdrom
    r.romData = dataROM[(uint10)r.a];
  }

  else if((opcode & 0xff00) == 0x7c00) {
    //0111 1100 .... ....
    //ld pl,i
    r.p.byte(0) = opcode.bits(0,7);
  }

  else if((opcode & 0xff00) == 0x7d00) {
    //0111 1101 .... ....
    //ld ph,i
    r.p.byte(1) = opcode.bits(0,7);
  }

  else if((opcode & 0xf800) == 0x8000) {
    //1000 0... .... ....
    //add a<<n,ri
    int result = sa() + ri();
    r.a = result;
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
    r.c = result > 0xffffff;
  }

  else if((opcode & 0xf800) == 0x8800) {
    //1000 1... .... ....
    //subr a<<n,ri
    int result = ri() - sa();
    r.a = result;
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
    r.c = result >= 0;
  }

  else if((opcode & 0xf800) == 0x9000) {
    //1001 0... .... ....
    //sub a<<n,ri
    int result = sa() - ri();
    r.a = result;
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
    r.c = result >= 0;
  }

  else if((opcode & 0xfb00) == 0x9800) {
    //1001 1.00 .... ....
    //mul a,ri
    int64 x = (int24)r.a;
    int64 y = (int24)ri();
    x *= y;
    r.accl = x >>  0ull;
    r.acch = x >> 24ull;
    r.n = r.acch & 0x800000;
    r.z = x == 0;
  }

  else if((opcode & 0xf800) == 0xa800) {
    //1010 1... .... ....
    //xor a<<n,ri
    r.a = sa() ^ ri();
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xf800) == 0xb000) {
    //1011 0... .... ....
    //and a<<n,ri
    r.a = sa() & ri();
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xf800) == 0xb800) {
    //1011 1... .... ....
    //or a<<n,ri
    r.a = sa() | ri();
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xfb00) == 0xc000) {
    //1100 0.00 .... ....
    //shr a,ri
    r.a = r.a >> ri();
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xfb00) == 0xc800) {
    //1100 1.00 .... ....
    //asr a,ri
    r.a = (int24)r.a >> ri();
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xfb00) == 0xd000) {
    //1101 0.00 .... ....
    //ror a,ri
    uint24 length = ri();
    r.a = (r.a >> length) | (r.a << (24 - length));
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xfb00) == 0xd800) {
    //1101 1.00 .... ....
    //shl a,ri
    r.a = r.a << ri();
    r.n = r.a & 0x800000;
    r.z = r.a == 0;
  }

  else if((opcode & 0xff00) == 0xe000) {
    //1110 0000 .... ....
    //st r,a
    writeRegister(opcode & 0xff, r.a);
  }

  else if((opcode & 0xff00) == 0xe100) {
    //1110 0001 .... ....
    //st r,bus
    writeRegister(opcode & 0xff, r.busData);
  }

  else if((opcode & 0xfb00) == 0xe800) {
    //1110 1.00 .... ....
    //wrraml
    uint24 target = ri() + (opcode.bit(10) ? r.ramAddress : (uint24)0);
    if(target < 0xc00) dataRAM[target] = r.ramData.byte(0);
  }

  else if((opcode & 0xfb00) == 0xe900) {
    //1110 1.01 .... ....
    //wrramh
    uint24 target = ri() + (opcode.bit(10) ? r.ramAddress : (uint24)0);
    if(target < 0xc00) dataRAM[target] = r.ramData.byte(1);
  }

  else if((opcode & 0xfb00) == 0xea00) {
    //1110 1.10 .... ....
    //wrramb
    uint24 target = ri() + (opcode.bit(10) ? r.ramAddress : (uint24)0);
    if(target < 0xc00) dataRAM[target] = r.ramData.byte(2);
  }

  else if((opcode & 0xff00) == 0xf000) {
    //1111 0000 .... ....
    //swap a,r
    uint24 source = readRegister(opcode & 0xff);
    uint24 target = r.a;
    r.a = source;
    writeRegister(opcode & 0xff, target);
  }

  else if((opcode & 0xffff) == 0xfc00) {
    //1111 1100 0000 0000
    //halt
    halt();
  }

  else {
    print("Hitachi DSP: unknown opcode @ ", hex((r.pb << 8 | r.pc << 0) - 1, 6L), " = ", hex(opcode, 4L), "\n");
    halt();
  }
}
