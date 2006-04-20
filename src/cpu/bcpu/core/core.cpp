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

uint32 bCPU::op_addr(uint8 mode, uint32 addr) {
  switch(mode) {
  case OPMODE_ADDR:
    addr &= 0xffff;
    break;
  case OPMODE_LONG:
    addr &= 0xffffff;
    break;
  case OPMODE_DBR:
    addr = ((regs.db << 16) + addr) & 0xffffff;
    break;
  case OPMODE_PBR:
    addr &= 0xffff;
    addr = (regs.pc.b << 16) + addr;
    break;
  case OPMODE_DP:
    addr &= 0xffff;
    addr = (regs.d + addr) & 0xffff;
    break;
  case OPMODE_SP:
    addr &= 0xffff;
    addr = (regs.s + addr) & 0xffff;
    break;
  }
  return addr;
}

uint8 bCPU::op_read() {
uint8 r;
  r = mem_read(regs.pc.d);
  regs.pc.w++;
  return r;
}

uint8 bCPU::op_read(uint8 mode, uint32 addr) {
  addr = op_addr(mode, addr);
  return mem_read(addr);
}

void bCPU::op_write(uint8 mode, uint32 addr, uint8 value) {
  addr = op_addr(mode, addr);
  mem_write(addr, value);
}

uint8 bCPU::stack_read() {
  if(regs.e) {
    regs.s.l++;
  } else {
    regs.s.w++;
  }
  return mem_read(regs.s);
}

void bCPU::stack_write(uint8 value) {
  mem_write(regs.s, value);
  if(regs.e) {
    regs.s.l--;
  } else {
    regs.s.w--;
  }
}

void bCPU::init_op_tables() {
  #include "optable.cpp"
}
