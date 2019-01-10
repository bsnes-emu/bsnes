auto TLCS900H::condition(uint4 code) -> bool {
  switch(code) {
  case  0: return 0 == 1;                 //F   (false)
  case  1: return (SF ^ VF) == 1;         //LT  (signed less than)
  case  2: return (ZF | (SF ^ VF)) == 1;  //LE  (signed less than or equal)
  case  3: return (CF | ZF) == 1;         //ULE (unsigned less than or equal)
  case  4: return VF == 1;                //OV  (overflow)
  case  5: return SF == 1;                //MI  (minus)
  case  6: return ZF == 1;                //EQ  (equal)
  case  7: return CF == 1;                //ULT (unsigned less than)
  case  8: return 0 == 0;                 //T   (true)
  case  9: return (SF ^ VF) == 0;         //GE  (signed greater than or equal)
  case 10: return (ZF | (SF ^ VF)) == 0;  //GT  (signed greater than)
  case 11: return (CF | ZF) == 0;         //UGT (unsigned greater than)
  case 12: return VF == 0;                //NOV (no overflow)
  case 13: return SF == 0;                //PL  (plus)
  case 14: return ZF == 0;                //NE  (not equal)
  case 15: return CF == 0;                //UGE (unsigned greater than or equal)
  } unreachable;
}
