#include "opfn.cpp"
#include "op_mov.cpp"
#include "op_pc.cpp"
#include "op_read.cpp"
#include "op_rmw.cpp"
#include "op_misc.cpp"

void bSMP::exec() {
  if(status.cycle_pos) {
    (this->*optbl[status.opcode])();
    add_cycles(1);
    return;
  }

//on first cycle?
  status.opcode = op_read();
  status.cycle_pos = 1;
  add_cycles(1);
}

//only return true when we are on an opcode edge
bool bSMP::in_opcode() {
  return (status.cycle_pos != 0);
}

void bSMP::init_op_table() {
  #include "optable.cpp"
}
