//80  grp1 memb,immb
//81  grp1 memw,immw
//82  grp1 memb,immbs
//83  grp1 memw,immbs
auto V30MZ::opGroup1MemImm(Size size, bool sign) {
  modRM();
  auto mem = getMem(size);
  uint16 imm = 0;
  if(sign) imm = (int8)fetch();
  else if(size == Byte) imm = fetch();
  else imm = fetch(Word);
  switch(modrm.reg) {
  case 0: setMem(size, alAdd(size, mem, imm)); break;
  case 1: setMem(size, alOr (size, mem, imm)); break;
  case 2: setMem(size, alAdc(size, mem, imm)); break;
  case 3: setMem(size, alSbb(size, mem, imm)); break;
  case 4: setMem(size, alAnd(size, mem, imm)); break;
  case 5: setMem(size, alSub(size, mem, imm)); break;
  case 6: setMem(size, alXor(size, mem, imm)); break;
  case 7:              alSub(size, mem, imm);  break;
  }
}

//c0  grp2 memb,imm8
//c1  grp2 memw,imm8
//d0  grp2 memb,1
//d1  grp2 memw,1
//d2  grp2 memb,cl
//d3  grp2 memw,cl
auto V30MZ::opGroup2MemImm(Size size, maybe<uint8> imm) {
  modRM();
  auto mem = getMem(size);
  if(!imm) {
    wait(2);
    imm = fetch();
  }
  switch(modrm.reg) {
  case 0: setMem(size, alRol(size, mem, *imm)); break;
  case 1: setMem(size, alRor(size, mem, *imm)); break;
  case 2: setMem(size, alRcl(size, mem, *imm)); break;
  case 3: setMem(size, alRcr(size, mem, *imm)); break;
  case 4: setMem(size, alShl(size, mem, *imm)); break;
  case 5: setMem(size, alShr(size, mem, *imm)); break;
  case 6: setMem(size, alSal(size, mem, *imm)); break;
  case 7: setMem(size, alSar(size, mem, *imm)); break;
  }
}

//f6  grp3 memb
//f7  grp3 memw
auto V30MZ::opGroup3MemImm(Size size) {
  modRM();
  auto mem = getMem(size);
  switch(modrm.reg) {
  case 0: alAnd(size, mem, fetch(size)); break;
  case 1: warning("[V30MZ] GRP3.1"); break;
  case 2: wait(2); setMem(size, alNot(size, mem)); break;
  case 3: wait(2); setMem(size, alNeg(size, mem)); break;
  case 4: wait(2); setAcc(size * 2, alMul(size, getAcc(size), mem)); break;
  case 5: wait(2); setAcc(size * 2, alMuli(size, getAcc(size), mem)); break; break;
  case 6: wait(size == Byte ? 15 : 23); setAcc(size * 2, alDiv(size, getAcc(size * 2), mem)); break;
  case 7: wait(size == Byte ? 17 : 24); setAcc(size * 2, alDivi(size, getAcc(size * 2), mem)); break;
  }
}

//fe  grp4 memb
//ff  grp4 memw
auto V30MZ::opGroup4MemImm(Size size) {
  modRM();
  switch(modrm.reg) {
  case 0:
    wait(2);
    setMem(size, alInc(size, getMem(size)));
    break;
  case 1:
    wait(2);
    setMem(size, alDec(size, getMem(size)));
    break;
  case 2:
    if(size == Byte) { warning("[V30MZ] GRP4.2"); break; }
    wait(5);
    push(r.ip);
    r.ip = getMem(Word);
    break;
  case 3:
    if(size == Byte) { warning("[V30MZ] GRP4.3"); break; }
    wait(11);
    push(r.cs);
    push(r.ip);
    r.ip = getMem(Word, 0);
    r.cs = getMem(Word, 2);
    break;
  case 4:
    if(size == Byte) { warning("[V30MZ] GRP4.4"); break; }
    wait(4);
    r.ip = getMem(Word);
    break;
  case 5:
    if(size == Byte) { warning("[V30MZ] GRP4.5"); break; }
    wait(9);
    r.ip = getMem(Word, 0);
    r.cs = getMem(Word, 2);
    break;
  case 6:
    if(size == Byte) { warning("[V30MZ] GRP4.6"); break; }
    wait(1);
    push(getMem(Word));
    break;
  case 7:
    warning("[V30MZ] GRP4.7");
    break;
  }
}
