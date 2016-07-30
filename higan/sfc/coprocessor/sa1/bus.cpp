//ROM / RAM access from the S-CPU

auto SA1::CPUIRAM::size() const -> uint {
  return sa1.iram.size();
}

auto SA1::CPUIRAM::read(uint24 addr, uint8) -> uint8 {
  cpu.synchronize(sa1);
  return sa1.iram.read(addr & 0x07ff);
}

auto SA1::CPUIRAM::write(uint24 addr, uint8 data) -> void {
  cpu.synchronize(sa1);
  sa1.iram.write(addr & 0x07ff, data);
}

auto SA1::CPUBWRAM::size() const -> uint {
  return sa1.bwram.size();
}

auto SA1::CPUBWRAM::read(uint24 addr, uint8) -> uint8 {
  cpu.synchronize(sa1);
  if(dma) return sa1.dmaCC1Read(addr);
  return sa1.bwram.read(addr);
}

auto SA1::CPUBWRAM::write(uint24 addr, uint8 data) -> void {
  cpu.synchronize(sa1);
  sa1.bwram.write(addr, data);
}
