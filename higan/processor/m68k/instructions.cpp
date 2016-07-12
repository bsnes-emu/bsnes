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

auto M68K::instructionBCC(uint4 condition, uint8 displacementByte) -> void {
  uint16 displacementWord = readWordPC();
  if(displacementByte) r.pc -= 2;
  if(condition == 1) {
    condition = 0;
  //pushLong(r.pc);
  }
  if(testCondition(condition)) {
    r.pc += displacementByte ? (int8_t)displacementByte : ((int16_t)displacementWord - 2);
  }
}

auto M68K::instructionLEA(uint3 wr, EA ea) -> void {
  r.a(wr) = address(ea);
}

auto M68K::instructionMOVEM(uint1 direction, EA ea) -> void {
  auto list = readWordPC();
}

auto M68K::instructionNOP() -> void {
}

auto M68K::instructionTST(EA ea) -> void {
  auto data = read(ea);

  r.c = 0;
  r.v = 0;
  r.z = data == 0;
  r.n = signExtend(ea.size, data) < 0;
}
