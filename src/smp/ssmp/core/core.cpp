#include "opfn.cpp"

void sSMP::enter() {
  for(;;) {
    tracer.trace_smpop(); //traces SMP opcode (only if tracer is enabled)

    status.in_opcode = true;

    switch(op_readpc()) {
      #include "op_mov.cpp"
      #include "op_pc.cpp"
      #include "op_read.cpp"
      #include "op_rmw.cpp"
      #include "op_misc.cpp"
    }

    status.in_opcode = false;
  }
}
