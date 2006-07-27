/*****
 * These 3 functions control bus timing for the CPU.
 * cpu_io is an I/O cycle, and always 6 clock cycles long.
 * mem_read / mem_write indicate memory access bus cycles.
 * they are either 6, 8, or 12 bus cycles long, depending
 * both on location and the $420d.d0 FastROM enable bit.
 *****/

void sCPU::op_io() {
  status.clock_count = 6;
  add_clocks(6);
//co_return();
  cycle_edge();
}

uint8 sCPU::op_read(uint32 addr) {
  status.clock_count = r_mem->speed(addr);
  add_clocks(status.clock_count - 4);
#ifdef FAVOR_ACCURACY
  co_return();
#endif
  regs.mdr = r_mem->read(addr);
  add_clocks(4);
  cycle_edge();
  return regs.mdr;
}

void sCPU::op_write(uint32 addr, uint8 data) {
  status.clock_count = r_mem->speed(addr);
  add_clocks(status.clock_count);
#ifdef FAVOR_ACCURACY
  co_return();
#endif
  regs.mdr = data;
  r_mem->write(addr, regs.mdr);
  cycle_edge();
}
