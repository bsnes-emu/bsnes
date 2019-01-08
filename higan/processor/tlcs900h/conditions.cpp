auto TLCS900H::condition(uint4 code) -> bool {
  switch(code) {
  case  0: return 0 == 1;                                 //F   (false)
  case  1: return (sign() ^ overflow()) == 1;             //LT  (signed less than)
  case  2: return (zero() | (sign() ^ overflow())) == 1;  //LE  (signed less than or equal)
  case  3: return (carry() | zero()) == 1;                //ULE (unsigned less than or equal)
  case  4: return overflow() == 1;                        //OV  (overflow)
  case  5: return sign() == 1;                            //MI  (minus)
  case  6: return zero() == 1;                            //EQ  (equal)
  case  7: return carry() == 1;                           //ULT (unsigned less than)
  case  8: return 0 == 0;                                 //T   (true)
  case  9: return (sign() ^ overflow()) == 0;             //GE  (signed greater than or equal)
  case 10: return (zero() | (sign() ^ overflow())) == 0;  //GT  (signed greater than)
  case 11: return (carry() | zero()) == 0;                //UGT (unsigned greater than)
  case 12: return overflow() == 0;                        //NOV (no overflow)
  case 13: return sign() == 0;                            //PL  (plus)
  case 14: return zero() == 0;                            //NE  (not equal)
  case 15: return carry() == 0;                           //UGE (unsigned greater than or equal)
  } unreachable;
}
