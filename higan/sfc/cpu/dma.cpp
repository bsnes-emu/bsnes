/* DMA timing:
   The general idea is that DMA moves values between the A-bus and B-bus in parallel.
   Obviously, a write can't happen at the same time as a read, as the read needs 8 cycles to complete.
   My theory is that the accesses are staggered, like so:

   Cycle 0: read 0
   Cycle 1: read 1, write 0
   Cycle 2: read 2, write 1
   ...
   Cycle n: read n, write n-1
   Cycle n+1: write n

   The staggered writes are implemented below using the pipe/flush concept.
*/

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

auto CPU::dmaWrite() -> void {
  r.rwb = pipe.valid;
  r.mar = pipe.address;
  dmaStep(8);
  dmaFlush();
}

auto CPU::dmaRun() -> void {
  dmaWrite();
  dmaEdge();
  for(auto& channel : channels) channel.dmaRun();
  dmaFlush();
  status.irqLock = true;
}

auto CPU::hdmaReset() -> void {
  for(auto& channel : channels) channel.hdmaReset();
}

auto CPU::hdmaSetup() -> void {
  dmaWrite();
  for(auto& channel : channels) channel.hdmaSetup();
  dmaFlush();
  status.irqLock = true;
}

auto CPU::hdmaRun() -> void {
  dmaWrite();
  for(auto& channel : channels) channel.hdmaTransfer();
  for(auto& channel : channels) channel.hdmaAdvance();
  dmaFlush();
  status.irqLock = true;
}

//

auto CPU::Channel::step(uint clocks) -> void { return cpu.dmaStep(clocks); }
auto CPU::Channel::edge() -> void { return cpu.dmaEdge(); }
auto CPU::Channel::flush() -> void { return cpu.dmaFlush(); }
auto CPU::Channel::write() -> void { return cpu.dmaWrite(); }

auto CPU::Channel::validA(uint24 address) -> bool {
  //A-bus cannot access the B-bus or CPU I/O registers
  if((address & 0x40ff00) == 0x2100) return false;  //00-3f,80-bf:2100-21ff
  if((address & 0x40fe00) == 0x4000) return false;  //00-3f,80-bf:4000-41ff
  if((address & 0x40ffe0) == 0x4200) return false;  //00-3f,80-bf:4200-421f
  if((address & 0x40ff80) == 0x4300) return false;  //00-3f,80-bf:4300-437f
  return true;
}

auto CPU::Channel::readA(uint24 address) -> uint8 {
  return readA(address, validA(address));
}

auto CPU::Channel::readA(uint24 address, bool valid) -> uint8 {
  step(4);
  cpu.r.mdr = valid ? bus.read(address, cpu.r.mdr) : (uint8)0x00;
  step(4);
  flush();
  return cpu.r.mdr;
}

auto CPU::Channel::readB(uint8 address, bool valid) -> uint8 {
  step(4);
  cpu.r.mdr = valid ? bus.read(0x2100 | address, cpu.r.mdr) : (uint8)0x00;
  step(4);
  flush();
  return cpu.r.mdr;
}

auto CPU::Channel::writeA(uint24 address, uint8 data) -> void {
  return writeA(address, data, validA(address));
}

auto CPU::Channel::writeA(uint24 address, uint8 data, bool valid) -> void {
  cpu.pipe.valid = valid;
  cpu.pipe.address = address;
  cpu.pipe.data = data;
}

auto CPU::Channel::writeB(uint8 address, uint8 data, bool valid) -> void {
  cpu.pipe.valid = valid;
  cpu.pipe.address = 0x2100 | address;
  cpu.pipe.data = data;
}

auto CPU::Channel::transfer(uint24 addressA, uint2 index) -> void {
  uint8 addressB = targetAddress;
  switch(transferMode) {
  case 1: case 5: addressB += index.bit(0); break;
  case 3: case 7: addressB += index.bit(1); break;
  case 4: addressB += index; break;
  }

  //transfers from WRAM to WRAM are invalid
  bool valid = addressB != 0x2180 || ((addressA & 0xfe0000) != 0x7e0000 && (addressA & 0x40e000) != 0x0000);

  cpu.r.rwb = 1;
  cpu.r.mar = addressA;
  if(direction == 0) {
    auto data = readA(addressA);
    writeB(addressB, data, valid);
  } else {
    auto data = readB(addressB, valid);
    writeA(addressA, data);
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

  write();
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
  cpu.r.rwb = 1;
  auto data = readA(cpu.r.mar = sourceBank << 16 | hdmaAddress);

  if((uint7)lineCounter == 0) {
    lineCounter = data;
    hdmaAddress++;

    hdmaCompleted = lineCounter == 0;
    hdmaDoTransfer = !hdmaCompleted;

    if(indirect) {
      cpu.r.rwb = 1;
      data = readA(cpu.r.mar = sourceBank << 16 | hdmaAddress++);
      indirectAddress = data << 8 | 0x00;  //todo: should 0x00 be indirectAddress >> 8 ?
      if(hdmaCompleted && hdmaFinished()) return;

      cpu.r.rwb = 1;
      data = readA(cpu.r.mar = sourceBank << 16 | hdmaAddress++);
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
