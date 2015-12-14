//ROM / RAM access from the S-CPU

auto SA1::CPUIRAM::size() const -> uint {
  return sa1.iram.size();
}

auto SA1::CPUIRAM::read(uint addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();
  return sa1.iram.read(addr & 0x07ff);
}

auto SA1::CPUIRAM::write(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  sa1.iram.write(addr & 0x07ff, data);
}

auto SA1::CPUBWRAM::size() const -> uint {
  return sa1.bwram.size();
}

auto SA1::CPUBWRAM::read(uint addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();
  if(dma) return sa1.dma_cc1_read(addr);
  return sa1.bwram.read(addr);
}

auto SA1::CPUBWRAM::write(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();
  sa1.bwram.write(addr, data);
}
