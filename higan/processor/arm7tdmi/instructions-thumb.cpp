auto ARM7TDMI::thumbALU(uint4 mode, uint4 target, uint4 source) -> void {
  switch(mode) {
  case  0: r(target) = BIT(r(target) & r(source));          break;  //AND
  case  1: r(target) = BIT(r(target) ^ r(source));          break;  //EOR
  case  2: r(target) = BIT(LSL(r(target), r(source)));      break;  //LSL
  case  3: r(target) = BIT(LSR(r(target), r(source)));      break;  //LSR
  case  4: r(target) = BIT(ASR(r(target), r(source)));      break;  //ASR
  case  5: r(target) = ADD(r(target), r(source), cpsr().c); break;  //ADC
  case  6: r(target) = SUB(r(target), r(source), cpsr().c); break;  //SBC
  case  7: r(target) = BIT(ROR(r(target), r(source)));      break;  //ROR
  case  8:             BIT(r(target) & r(source));          break;  //TST
  case  9: r(target) = SUB(0, r(source), 1);                break;  //NEG
  case 10:             SUB(r(target), r(source), 1);        break;  //CMP
  case 11:             ADD(r(target), r(source), 0);        break;  //CMN
  case 12: r(target) = BIT(r(target) | r(source));          break;  //ORR
  case 13: r(target) = MUL(0, r(source), r(target));        break;  //MUL
  case 14: r(target) = BIT(r(target) & ~r(source));         break;  //BIC
  case 15: r(target) = BIT(~r(source));                     break;  //MVN
  }
}
