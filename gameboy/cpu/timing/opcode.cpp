#ifdef CPU_CPP

void CPU::op_io() {
  add_clocks(4);
}

uint8 CPU::op_read(uint16 addr) {
  uint8 r = bus.read(addr);
  add_clocks(4);
  return r;
}

void CPU::op_write(uint16 addr, uint8 data) {
  bus.write(addr, data);
  add_clocks(4);
}

#endif
