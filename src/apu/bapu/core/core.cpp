#include "opfn.cpp"
#include "op_mov.cpp"
#include "op_pc.cpp"
#include "op_read.cpp"
#include "op_rmw.cpp"
#include "op_misc.cpp"

void bAPU::exec() {
  if(status.cycle_pos) {
    (this->*optbl[status.opcode])();
    add_cycles(1);
    if(status.cycle_pos == 0) {
    #ifdef DEBUGGER
      snes->notify(SNES::APU_EXEC_OPCODE_END);
    #endif
    }
    return;
  }

//on first cycle?
#ifdef DEBUGGER
  snes->notify(SNES::APU_EXEC_OPCODE_BEGIN);
#endif
  status.opcode = op_read();
  status.cycle_pos = 1;
  add_cycles(1);
}

//only return true when we are on an opcode edge
bool bAPU::in_opcode() {
  return (status.cycle_pos != 0);
}

void bAPU::init_op_table() {
  #include "optable.cpp"
}
