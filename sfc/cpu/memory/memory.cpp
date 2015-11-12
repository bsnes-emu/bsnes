auto CPU::portRead(uint2 port) const -> uint8 {
  return status.port[port];
}

auto CPU::portWrite(uint2 port, uint8 data) -> void {
  status.port[port] = data;
}

auto CPU::op_io() -> void {
  status.clock_count = 6;
  dma_edge();
  add_clocks(6);
  alu_edge();
}

auto CPU::op_read(uint32 addr) -> uint8 {
  status.clock_count = speed(addr);
  dma_edge();
  add_clocks(status.clock_count - 4);
  regs.mdr = bus.read(addr);
  add_clocks(4);
  alu_edge();
  debugger.op_read(addr, regs.mdr);
  return regs.mdr;
}

auto CPU::op_write(uint32 addr, uint8 data) -> void {
  alu_edge();
  status.clock_count = speed(addr);
  dma_edge();
  add_clocks(status.clock_count);
  bus.write(addr, regs.mdr = data);
  debugger.op_write(addr, regs.mdr);
}

auto CPU::speed(uint addr) const -> uint {
  if(addr & 0x408000) {
    if(addr & 0x800000) return status.rom_speed;
    return 8;
  }
  if((addr + 0x6000) & 0x4000) return 8;
  if((addr - 0x4000) & 0x7e00) return 6;
  return 12;
}

auto CPU::disassembler_read(uint32 addr) -> uint8 {
  return bus.read(addr);
}
