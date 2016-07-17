auto M68K::testCondition(uint4 condition) -> bool {
  switch(condition) {
  case  0: return true;   //RA
  case  1: return false;  //NV,SR
  case  2: return !r.c && !r.z;  //HI
  case  3: return  r.c ||  r.z;  //LS
  case  4: return !r.c;  //CC,HS
  case  5: return  r.c;  //CS,LO
  case  6: return !r.z;  //NE
  case  7: return  r.z;  //EQ
  case  8: return !r.v;  //VC
  case  9: return  r.v;  //VS
  case 10: return !r.n;  //PL
  case 11: return  r.n;  //MI
  case 12: return  r.n ==  r.v;  //GE
  case 13: return  r.n !=  r.v;  //LT
  case 14: return  r.n ==  r.v && !r.z;  //GT
  case 15: return  r.n !=  r.v ||  r.z;  //LE
  }
  unreachable;
}

//

auto M68K::instructionANDI(uint2 size, uint3 mode, uint3 reg) -> void {
  auto data = readPC(size);
  EA modify{this, size, mode, reg};
  modify.write(data = modify.read() & data);

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = sign(size, data) < 0;
}

auto M68K::instructionBCC(uint4 condition, uint8 displacement) -> void {
  auto word = readPC();
  if(displacement) displacement = (int8)displacement, r.pc -= 2;
  else displacement = (int16)word;
  if(condition == 1) {
    condition = 0;
  //pushLong(r.pc);
  }
  if(testCondition(condition)) r.pc += displacement;
}

auto M68K::instructionLEA(uint3 target, uint3 mode, uint3 reg) -> void {
  EA source{this, Long, mode, reg};
  r.a(target) = source.address;
}

auto M68K::instructionMOVE(uint2 size, uint3 targetReg, uint3 targetMode, uint3 sourceMode, uint3 sourceReg) -> void {
  EA source{this, size, sourceMode, sourceReg};
  EA target{this, size, targetMode, targetReg};

  auto data = source.read();
  source.flush();
  target.write(data);

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = sign(size, data) < 0;
}

auto M68K::instructionMOVEA(uint2 size, uint3 target, uint3 mode, uint3 reg) -> void {
  EA source{this, size, mode, reg};
  r.a(target) = source.read();
}

auto M68K::instructionMOVEM(uint1 direction, uint2 size, uint3 mode, uint3 reg) -> void {
  auto list = readPC();
  EA source{this, size, mode, reg};

  for(uint n : range(8)) {
    if(list.bit(0 + n)) r.d(n) = source.read();
  }

  for(uint n : range(8)) {
    if(list.bit(8 + n)) r.a(n) = source.read();
  }
}

auto M68K::instructionMOVEQ(uint3 target, uint8 immediate) -> void {
  r.d(target) = immediate;

  r.c = 0;
  r.v = 0;
  r.z = immediate == 0;
  r.v = sign(Byte, immediate) < 0;
}

auto M68K::instructionMOVE_USP(uint1 direction, uint3 reg) -> void {
  if(!r.s) trap();  //todo: proper trap
  if(direction == 0) {
    r.usp = r.a(reg);
  } else {
    r.a(reg) = r.usp;
  }
}

auto M68K::instructionNOP() -> void {
}

auto M68K::instructionTST(uint2 size, uint3 mode, uint3 reg) -> void {
  EA source{this, size, mode, reg};
  auto data = source.read();

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = sign(size, data) < 0;
}
