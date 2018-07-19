auto CPU::dmaEnable() -> bool {
  for(auto& channel : channels) if(channel.dmaEnable) return true;
  return false;
}

auto CPU::hdmaEnable() -> bool {
  for(auto& channel : channels) if(channel.hdmaEnable) return true;
  return false;
}

auto CPU::hdmaActive() -> bool {
  for(auto& channel : channels) if(channel.hdmaActive()) return true;
  return false;
}

auto CPU::dmaStep(uint clocks) -> void {
  status.dmaClocks += clocks;
  step(clocks);
}

auto CPU::dmaFlush() -> void {
  if(!pipe.valid) return;
  pipe.valid = false;
  bus.write(pipe.address, pipe.data);
}

auto CPU::dmaRun() -> void {
  dmaStep(8);
  dmaFlush();
  dmaEdge();
  for(auto& channel : channels) channel.dmaRun();
  dmaFlush();
  status.irqLock = true;
}

auto CPU::hdmaReset() -> void {
  for(auto& channel : channels) channel.hdmaReset();
}

auto CPU::hdmaSetup() -> void {
  dmaStep(8);
  dmaFlush();
  for(auto& channel : channels) channel.hdmaSetup();
  dmaFlush();
  status.irqLock = true;
}

auto CPU::hdmaRun() -> void {
  dmaStep(8);
  dmaFlush();
  for(auto& channel : channels) channel.hdmaTransfer();
  for(auto& channel : channels) channel.hdmaAdvance();
  dmaFlush();
  status.irqLock = true;
}

//

auto CPU::Channel::step(uint clocks) -> void {
  return cpu.dmaStep(clocks);
}

auto CPU::Channel::edge() -> void {
  return cpu.dmaEdge();
}

auto CPU::Channel::valid(uint24 address) -> bool {
  //A-bus cannot access the B-bus or CPU I/O registers
  if((address & 0x40ff00) == 0x2100) return false;  //00-3f,80-bf:2100-21ff
  if((address & 0x40fe00) == 0x4000) return false;  //00-3f,80-bf:4000-41ff
  if((address & 0x40ffe0) == 0x4200) return false;  //00-3f,80-bf:4200-421f
  if((address & 0x40ff80) == 0x4300) return false;  //00-3f,80-bf:4300-437f
  return true;
}

auto CPU::Channel::read(uint24 address, bool valid) -> uint8 {
  step(4);
  cpu.r.mdr = valid ? bus.read(address, cpu.r.mdr) : (uint8)0x00;
  step(4);
  flush();
  return cpu.r.mdr;
}

auto CPU::Channel::read(uint24 address) -> uint8 {
  return read(address, valid(address));
}

auto CPU::Channel::flush() -> void {
  return cpu.dmaFlush();
}

auto CPU::Channel::write(uint24 address, uint8 data, bool valid) -> void {
  cpu.pipe.valid = valid;
  cpu.pipe.address = address;
  cpu.pipe.data = data;
}

auto CPU::Channel::write(uint24 address, uint8 data) -> void {
  return write(address, data, valid(address));
}

auto CPU::Channel::transfer(uint24 aAddress, uint2 index) -> void {
  uint24 bAddress = 0x2100 | targetAddress;
  switch(transferMode) {
  case 1: case 5: bAddress += index.bit(0); break;
  case 3: case 7: bAddress += index.bit(1); break;
  case 4: bAddress += index; break;
  }

  //transfers from WRAM to WRAM are invalid
  bool valid = bAddress != 0x2180 || ((aAddress & 0xfe0000) != 0x7e0000 && (aAddress & 0x40e000) != 0x0000);

  if(direction == 0) {
    auto data = read(aAddress);
    write(bAddress, data, valid);
  } else {
    auto data = read(bAddress, valid);
    write(aAddress, data);
  }
}

auto CPU::Channel::dmaRun() -> void {
  if(!dmaEnable) return;

  uint2 index = 0;
  do {
    transfer(sourceBank << 16 | sourceAddress, index++);
    if(!fixedTransfer) !reverseTransfer ? sourceAddress++ : sourceAddress--;
    edge();
  } while(dmaEnable && --transferSize);

  step(8);
  flush();
  edge();
  dmaEnable = false;
}

auto CPU::Channel::hdmaActive() -> bool {
  return hdmaEnable && !hdmaCompleted;
}

auto CPU::Channel::hdmaFinished() -> bool {
  auto channel = next;
  while(channel) {
    if(channel->hdmaActive()) return false;
    channel = channel->next;
  }
  return true;
}

auto CPU::Channel::hdmaReset() -> void {
  hdmaCompleted = false;
  hdmaDoTransfer = false;
}

auto CPU::Channel::hdmaSetup() -> void {
  hdmaDoTransfer = true;  //note: needs hardware verification
  if(!hdmaEnable) return;

  dmaEnable = false;  //HDMA will stop active DMA mid-transfer
  hdmaAddress = sourceAddress;
  lineCounter = 0;
  hdmaReload();
}

auto CPU::Channel::hdmaReload() -> void {
  auto data = read(sourceBank << 16 | hdmaAddress);

  if((uint7)lineCounter == 0) {
    lineCounter = data;
    hdmaAddress++;

    hdmaCompleted = lineCounter == 0;
    hdmaDoTransfer = !hdmaCompleted;

    if(indirect) {
      data = read(sourceBank << 16 | hdmaAddress++);
      indirectAddress = data << 8 | 0x00;  //todo: should 0x00 be indirectAddress >> 8 ?
      if(hdmaCompleted && hdmaFinished()) return;

      data = read(sourceBank << 16 | hdmaAddress++);
      indirectAddress = data << 8 | indirectAddress >> 8;
    }
  }
}

auto CPU::Channel::hdmaTransfer() -> void {
  if(!hdmaActive()) return;
  dmaEnable = false;  //HDMA will stop active DMA mid-transfer
  if(!hdmaDoTransfer) return;

  static const uint lengths[8] = {1, 2, 2, 4, 4, 4, 2, 4};
  for(uint2 index : range(lengths[transferMode])) {
    uint24 address = !indirect ? sourceBank << 16 | hdmaAddress++ : indirectBank << 16 | indirectAddress++;
    transfer(address, index);
  }
}

auto CPU::Channel::hdmaAdvance() -> void {
  if(!hdmaActive()) return;
  lineCounter--;
  hdmaDoTransfer = lineCounter.bit(7);
  hdmaReload();
}
