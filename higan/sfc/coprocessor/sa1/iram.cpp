auto SA1::IRAM::conflict() const -> bool {
  if(configuration.hacks.coprocessors.delayedSync) return false;

  if((cpu.r.mar & 0x40f800) == 0x003000) return cpu.refresh() == 0;  //00-3f,80-bf:3000-37ff
  return false;
}

auto SA1::IRAM::read(uint24 address, uint8 data) -> uint8 {
  if(!size()) return data;
  address = bus.mirror(address, size());
  return WritableMemory::read(address, data);
}

auto SA1::IRAM::write(uint24 address, uint8 data) -> void {
  if(!size()) return;
  address = bus.mirror(address, size());
  return WritableMemory::write(address, data);
}

auto SA1::IRAM::readCPU(uint24 address, uint8 data) -> uint8 {
  cpu.synchronize(sa1);
  return read(address, data);
}

auto SA1::IRAM::writeCPU(uint24 address, uint8 data) -> void {
  cpu.synchronize(sa1);
  return write(address, data);
}

auto SA1::IRAM::readSA1(uint24 address, uint8 data) -> uint8 {
  return read(address, data);
}

auto SA1::IRAM::writeSA1(uint24 address, uint8 data) -> void {
  return write(address, data);
}
