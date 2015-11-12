//ROM / RAM access from the S-CPU

unsigned SA1::CPUIRAM::size() const {
  return sa1.iram.size();
}

uint8 SA1::CPUIRAM::read(unsigned addr) {
  cpu.synchronizeCoprocessors();
  return sa1.iram.read(addr & 0x07ff);
}

void SA1::CPUIRAM::write(unsigned addr, uint8 data) {
  cpu.synchronizeCoprocessors();
  sa1.iram.write(addr & 0x07ff, data);
}

unsigned SA1::CPUBWRAM::size() const {
  return sa1.bwram.size();
}

uint8 SA1::CPUBWRAM::read(unsigned addr) {
  cpu.synchronizeCoprocessors();
  if(dma) return sa1.dma_cc1_read(addr);
  return sa1.bwram.read(addr);
}

void SA1::CPUBWRAM::write(unsigned addr, uint8 data) {
  cpu.synchronizeCoprocessors();
  sa1.bwram.write(addr, data);
}
