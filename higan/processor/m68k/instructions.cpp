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

auto M68K::instructionANDI(uint size, EA modify) -> void {
  auto data = readPC(size);
  write(size, modify, data = read(size, modify) & data);

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = sign(size, data) < 0;
}

auto M68K::instructionBCC(uint condition, uint displacement) -> void {
  auto word = readPC();
  if(displacement) displacement = (int8)displacement, r.pc -= 2;
  else displacement = (int16)word;
  if(condition == 1) {
    condition = 0;
  //pushLong(r.pc);
  }
  if(testCondition(condition)) r.pc += displacement;
}

auto M68K::instructionLEA(uint target, EA source) -> void {
  r.a(target) = address(Long, source);
}

auto M68K::instructionMOVE(uint size, EA target, EA source) -> void {
  auto data = read(size, source);
  write(size, target, data);

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = sign(size, data) < 0;
}

auto M68K::instructionMOVEA(uint size, uint target, EA source) -> void {
  r.d(target) = read(size, source);
}

auto M68K::instructionMOVEM(uint direction, uint size, EA source) -> void {
  auto list = readPC();

  for(uint n : range(8)) {
    if(list.bit(0 + n)) r.d(n) = read(size, source);
  }

  for(uint n : range(8)) {
    if(list.bit(8 + n)) r.a(n) = read(size, source);
  }
}

auto M68K::instructionMOVEQ(uint target, uint immediate) -> void {
  r.d(target) = immediate;

  r.c = 0;
  r.v = 0;
  r.z = immediate == 0;
  r.v = sign(Byte, immediate) < 0;
}

auto M68K::instructionMOVE_USP(uint direction, uint reg) -> void {
  if(!r.s) trap();
  if(direction == 0) {
    r.usp = r.a(reg);
  } else {
    r.a(reg) = r.usp;
  }
}

auto M68K::instructionNOP() -> void {
}

auto M68K::instructionTST(uint size, EA source) -> void {
  auto data = read(size, source);

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = sign(size, data) < 0;
}
