void bAPU::exec_cycle() {
uint8 op;
  if(status.cycle_pos == 0) {
    op = spcram_read(regs.pc);
    snes->notify(SNES::APU_EXEC_OPCODE_BEGIN);
    status.opcode = op_read();
    status.cycle_pos = 1;
    add_cycles(1);
  } else {
    (this->*optbl[status.opcode])();
    add_cycles(1);
    if(status.cycle_pos == 0) {
      snes->notify(SNES::APU_EXEC_OPCODE_END);
    }
  }
}

void bAPU::init_op_table() {
#include "bapu_optable.cpp"
}
