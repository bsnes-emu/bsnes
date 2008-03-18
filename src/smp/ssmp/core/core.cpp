#ifdef SSMP_CPP

#include "opfn.cpp"

#include "op_mov.cpp"
#include "op_pc.cpp"
#include "op_read.cpp"
#include "op_rmw.cpp"
#include "op_misc.cpp"

void sSMP::enter() { loop:
  tracer.trace_smpop(); //traces SMP opcode (only if tracer is enabled)

  status.in_opcode = true;
  (this->*optbl[op_readpc()])();
  status.in_opcode = false;

  goto loop;
}

#endif //ifdef SSMP_CPP
