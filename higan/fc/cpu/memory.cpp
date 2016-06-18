auto CPU::readRAM(uint11 addr) -> uint8 {
  return ram[addr];
}

auto CPU::writeRAM(uint11 addr, uint8 data) -> void {
  ram[addr] = data;
}

auto CPU::readIO(uint16 addr) -> uint8 {
  switch(addr) {

  case 0x4016: {
    auto data = Famicom::peripherals.controllerPort1->data();
    return (mdr() & 0xc0) | data.bit(2) << 4 | data.bit(1) << 3 | data.bit(0) << 0;
  }

  case 0x4017: {
    auto data = Famicom::peripherals.controllerPort2->data();
    return (mdr() & 0xc0) | data.bit(2) << 4 | data.bit(1) << 3 | data.bit(0) << 0;
  }

  }

  return apu.readIO(addr);
}

auto CPU::writeIO(uint16 addr, uint8 data) -> void {
  switch(addr) {

  case 0x4014: {
    io.oamdmaPage = data;
    io.oamdmaPending = true;
    return;
  }

  case 0x4016: {
    Famicom::peripherals.controllerPort1->latch(data.bit(0));
    Famicom::peripherals.controllerPort2->latch(data.bit(0));
    return;
  }

  }

  return apu.writeIO(addr, data);
}

auto CPU::readDebugger(uint16 addr) -> uint8 {
  return bus.read(addr);
}
