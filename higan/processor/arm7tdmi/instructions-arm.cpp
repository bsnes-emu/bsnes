auto ARM7TDMI::armALU(uint4 mode, uint4 target, uint4 source, uint32 data, bool save) -> void {
  switch(mode) {
  case  0: r(target) = BIT(r(source) & data, save);          break;  //AND
  case  1: r(target) = BIT(r(source) ^ data, save);          break;  //EOR
  case  2: r(target) = SUB(r(source), data, 1, save);        break;  //SUB
  case  3: r(target) = SUB(data, r(source), 1, save);        break;  //RSB
  case  4: r(target) = ADD(r(source), data, 0, save);        break;  //ADD
  case  5: r(target) = ADD(r(source), data, cpsr().c, save); break;  //ADC
  case  6: r(target) = SUB(r(source), data, cpsr().c, save); break;  //SBC
  case  7: r(target) = SUB(data, r(source), cpsr().c, save); break;  //RSC
  case  8:             BIT(r(source) & data, save);          break;  //TST
  case  9:             BIT(r(source) ^ data, save);          break;  //TEQ
  case 10:             SUB(r(source), data, 1, save);        break;  //CMP
  case 11:             ADD(r(source), data, 0, save);        break;  //CMN
  case 12: r(target) = BIT(r(source) | data, save);          break;  //ORR
  case 13: r(target) = BIT(data, save);                      break;  //MOV
  case 14: r(target) = BIT(r(source) & ~data, save);         break;  //BIC
  case 15: r(target) = BIT(~data, save);                     break;  //MVN
  }

  if(exception() && target == 15 && save) {
    cpsr() = spsr();
  }
}
