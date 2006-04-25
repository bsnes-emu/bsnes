uint8 bCPU::port_read(uint8 port) {
  return apu_port[port & 3];
}

void bCPU::port_write(uint8 port, uint8 value) {
  apu_port[port & 3] = value;
}

/* The next 3 functions control bus timing for the CPU.
 * cpu_io is an I/O cycle, and always 6 clock cycles long.
 * mem_read / mem_write indicate memory access bus cycle,
 * they are either 6, 8, or 12 bus cycles long, depending
 * both on location and the $420d.d0 FastROM enable bit.
 */

void bCPU::cpu_io() {
  status.cycle_count = 6;
  pre_exec_cycle();
  add_cycles(6);
  cycle_edge();
}

uint8 bCPU::mem_read(uint32 addr) {
  status.cycle_count = r_mem->speed(addr);
  pre_exec_cycle();
  add_cycles(status.cycle_count - 4);
  regs.mdr = r_mem->read(addr);
  add_cycles(4);
  cycle_edge();
  return regs.mdr;
}

void bCPU::mem_write(uint32 addr, uint8 value) {
  status.cycle_count = r_mem->speed(addr);
  pre_exec_cycle();
  add_cycles(status.cycle_count);
  regs.mdr = value;
  r_mem->write(addr, value);
  cycle_edge();
}
