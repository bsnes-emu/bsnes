#ifdef SSMP_CPP

#include "opfn.cpp"

void sSMP::enter() {
  while(true) {
    tracer.trace_smpop();  //traces SMP opcode (only if tracer is enabled)

    switch(op_readpc()) {
      #include "op_mov.cpp"
      #include "op_pc.cpp"
      #include "op_read.cpp"
      #include "op_rmw.cpp"
      #include "op_misc.cpp"
    }

    //forcefully sync S-CPU and S-SMP, in case chips are not communicating
    static unsigned counter = 0;
    if(++counter >= 128) {
      counter = 0;
      scheduler.sync_smpcpu();
    }
  }
}

#endif
