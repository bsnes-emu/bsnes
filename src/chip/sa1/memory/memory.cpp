#ifdef SA1_CPP

//==========================
//SA-1 opcode core functions
//==========================

void SA1::op_io() {
  tick();
  if(regs.wai) scheduler.sync_copcpu();
}

//ROM, I-RAM and MMIO registers are accessed at ~10.74MHz (2 clock ticks)
//BW-RAM is accessed at ~5.37MHz (4 clock ticks)
//tick() == 2 clock ticks
//note: bus conflict delays are not emulated at this time

#define is_bwram(addr) (\
   ((addr & 0x40e000) == 0x006000) \
|| ((addr & 0xf00000) == 0x400000) \
|| ((addr & 0xf00000) == 0x600000) \
)

uint8_t SA1::op_read(unsigned addr) {
  tick();
  if(is_bwram(addr)) tick();
  scheduler.sync_copcpu();
  return sa1bus.read(addr);
}

void SA1::op_write(unsigned addr, uint8_t data) {
  tick();
  if(is_bwram(addr)) tick();
  scheduler.sync_copcpu();
  sa1bus.write(addr, data);
}

#undef is_bwram

#endif
