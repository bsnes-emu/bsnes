#include "opfn.cpp"
#include "op_read.cpp"
#include "op_rmw.cpp"
#include "op_write.cpp"
#include "op_pc.cpp"
#include "op_misc.cpp"

void bCPU::cpu_c2() {
  if(regs.d.l != 0x00) {
    cpu_io();
  }
}

void bCPU::cpu_c4(uint16 x, uint16 y) {
  if(!regs.p.x && (x & 0xff00) != (y & 0xff00)) {
    cpu_io();
  }
}

void bCPU::cpu_c6(uint16 addr) {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    cpu_io();
  }
}

void bCPU::init_op_tables() {
  #include "optable.cpp"
}
