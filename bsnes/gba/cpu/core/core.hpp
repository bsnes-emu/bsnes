struct ARM7TDMI {
  #include "registers.hpp"
  GPR r[31];
  PSR cpsr;
  PSR spsr[5];

  void power();
};
