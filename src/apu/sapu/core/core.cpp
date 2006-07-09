#include "opfn.cpp"
//#include "op_mov.cpp"
//#include "op_pc.cpp"
//#include "op_read.cpp"
//#include "op_rmw.cpp"
//#include "op_misc.cpp"

void sAPU::main() {
  for(;;) {
    status.in_opcode = true;

//  (this->*optbl[op_readpc()])();
    switch(op_readpc()) {
      #include "op_mov.cpp"
      #include "op_pc.cpp"
      #include "op_read.cpp"
      #include "op_rmw.cpp"
      #include "op_misc.cpp"
    }

    status.in_opcode = false;

  #ifdef FAVOR_SPEED
    co_return();
  #endif
  }
}
