auto CPU::dmaAddClocks(uint clocks) -> void {
  status.dmaClocks += clocks;
  addClocks(clocks);
}

//=============
//memory access
//=============

auto CPU::dmaTransferValid(uint8 bbus, uint24 abus) -> bool {
  //transfers from WRAM to WRAM are invalid; chip only has one address bus
  if(bbus == 0x80 && ((abus & 0xfe0000) == 0x7e0000 || (abus & 0x40e000) == 0x0000)) return false;
  return true;
}

auto CPU::dmaAddressValid(uint24 abus) -> bool {
  //A-bus access to B-bus or S-CPU registers are invalid
  if((abus & 0x40ff00) == 0x2100) return false;  //$00-3f,80-bf:2100-21ff
  if((abus & 0x40fe00) == 0x4000) return false;  //$00-3f,80-bf:4000-41ff
  if((abus & 0x40ffe0) == 0x4200) return false;  //$00-3f,80-bf:4200-421f
  if((abus & 0x40ff80) == 0x4300) return false;  //$00-3f,80-bf:4300-437f
  return true;
}

auto CPU::dmaRead(uint24 abus) -> uint8 {
  if(!dmaAddressValid(abus)) return 0x00;
  return bus.read(abus, r.mdr);
}

//simulate two-stage pipeline for DMA transfers; example:
//cycle 0: read N+0
//cycle 1: write N+0 & read N+1 (parallel; one on A-bus, one on B-bus)
//cycle 2: write N+1 & read N+2 (parallel)
//cycle 3: write N+2
auto CPU::dmaWrite(bool valid, uint addr, uint8 data) -> void {
  if(pipe.valid) bus.write(pipe.addr, pipe.data);
  pipe.valid = valid;
  pipe.addr = addr;
  pipe.data = data;
}

auto CPU::dmaTransfer(bool direction, uint8 bbus, uint24 abus) -> void {
  if(direction == 0) {
    dmaAddClocks(4);
    r.mdr = dmaRead(abus);
    dmaAddClocks(4);
    dmaWrite(dmaTransferValid(bbus, abus), 0x2100 | bbus, r.mdr);
  } else {
    dmaAddClocks(4);
    r.mdr = dmaTransferValid(bbus, abus) ? bus.read(0x2100 | bbus, r.mdr) : (uint8)0x00;
    dmaAddClocks(4);
    dmaWrite(dmaAddressValid(abus), abus, r.mdr);
  }
}

//===================
//address calculation
//===================

auto CPU::dmaAddressB(uint n, uint index) -> uint8 {
  switch(channel[n].transferMode) {
  case 0: return (channel[n].targetAddress);                       //0
  case 1: return (channel[n].targetAddress + (index & 1));         //0,1
  case 2: return (channel[n].targetAddress);                       //0,0
  case 3: return (channel[n].targetAddress + ((index >> 1) & 1));  //0,0,1,1
  case 4: return (channel[n].targetAddress + (index & 3));         //0,1,2,3
  case 5: return (channel[n].targetAddress + (index & 1));         //0,1,0,1
  case 6: return (channel[n].targetAddress);                       //0,0     [2]
  case 7: return (channel[n].targetAddress + ((index >> 1) & 1));  //0,0,1,1 [3]
  }
  unreachable;
}

auto CPU::dmaAddress(uint n) -> uint24 {
  uint24 addr = channel[n].sourceBank << 16 | channel[n].sourceAddress;

  if(!channel[n].fixedTransfer) {
    if(!channel[n].reverseTransfer) {
      channel[n].sourceAddress++;
    } else {
      channel[n].sourceAddress--;
    }
  }

  return addr;
}

auto CPU::hdmaAddress(uint n) -> uint24 {
  return channel[n].sourceBank << 16 | channel[n].hdmaAddress++;
}

auto CPU::hdmaIndirectAddress(uint n) -> uint24 {
  return channel[n].indirectBank << 16 | channel[n].indirectAddress++;
}

//==============
//channel status
//==============

auto CPU::dmaEnabledChannels() -> uint {
  uint count = 0;
  for(auto n : range(8)) count += channel[n].dmaEnabled;
  return count;
}

auto CPU::hdmaActive(uint n) -> bool {
  return channel[n].hdmaEnabled && !channel[n].hdmaCompleted;
}

auto CPU::hdmaActiveAfter(uint s) -> bool {
  for(uint n = s + 1; n < 8; n++) {
    if(hdmaActive(n)) return true;
  }
  return false;
}

auto CPU::hdmaEnabledChannels() -> uint {
  uint count = 0;
  for(auto n : range(8)) count += channel[n].hdmaEnabled;
  return count;
}

auto CPU::hdmaActiveChannels() -> uint {
  uint count = 0;
  for(auto n : range(8)) count += hdmaActive(n);
  return count;
}

//==============
//core functions
//==============

auto CPU::dmaRun() -> void {
  dmaAddClocks(8);
  dmaWrite(false);
  dmaEdge();

  for(auto n : range(8)) {
    if(!channel[n].dmaEnabled) continue;

    uint index = 0;
    do {
      dmaTransfer(channel[n].direction, dmaAddressB(n, index++), dmaAddress(n));
      dmaEdge();
    } while(channel[n].dmaEnabled && --channel[n].transferSize);

    dmaAddClocks(8);
    dmaWrite(false);
    dmaEdge();

    channel[n].dmaEnabled = false;
  }

  status.irqLock = true;
}

auto CPU::hdmaUpdate(uint n) -> void {
  dmaAddClocks(4);
  r.mdr = dmaRead(channel[n].sourceBank << 16 | channel[n].hdmaAddress);
  dmaAddClocks(4);
  dmaWrite(false);

  if((channel[n].lineCounter & 0x7f) == 0) {
    channel[n].lineCounter = r.mdr;
    channel[n].hdmaAddress++;

    channel[n].hdmaCompleted = channel[n].lineCounter == 0;
    channel[n].hdmaDoTransfer = !channel[n].hdmaCompleted;

    if(channel[n].indirect) {
      dmaAddClocks(4);
      r.mdr = dmaRead(hdmaAddress(n));
      channel[n].indirectAddress = r.mdr << 8;
      dmaAddClocks(4);
      dmaWrite(false);

      if(!channel[n].hdmaCompleted || hdmaActiveAfter(n)) {
        dmaAddClocks(4);
        r.mdr = dmaRead(hdmaAddress(n));
        channel[n].indirectAddress >>= 8;
        channel[n].indirectAddress |= r.mdr << 8;
        dmaAddClocks(4);
        dmaWrite(false);
      }
    }
  }
}

auto CPU::hdmaRun() -> void {
  dmaAddClocks(8);
  dmaWrite(false);

  for(auto n : range(8)) {
    if(!hdmaActive(n)) continue;
    channel[n].dmaEnabled = false;  //HDMA run during DMA will stop DMA mid-transfer

    if(channel[n].hdmaDoTransfer) {
      static const uint transferLength[8] = {1, 2, 2, 4, 4, 4, 2, 4};
      uint length = transferLength[channel[n].transferMode];
      for(auto index : range(length)) {
        uint addr = !channel[n].indirect ? hdmaAddress(n) : hdmaIndirectAddress(n);
        dmaTransfer(channel[n].direction, dmaAddressB(n, index), addr);
      }
    }
  }

  for(auto n : range(8)) {
    if(!hdmaActive(n)) continue;

    channel[n].lineCounter--;
    channel[n].hdmaDoTransfer = channel[n].lineCounter & 0x80;
    hdmaUpdate(n);
  }

  status.irqLock = true;
}

auto CPU::hdmaInitReset() -> void {
  for(auto n : range(8)) {
    channel[n].hdmaCompleted = false;
    channel[n].hdmaDoTransfer = false;
  }
}

auto CPU::hdmaInit() -> void {
  dmaAddClocks(8);
  dmaWrite(false);

  for(auto n : range(8)) {
    if(!channel[n].hdmaEnabled) continue;
    channel[n].dmaEnabled = false;  //HDMA init during DMA will stop DMA mid-transfer

    channel[n].hdmaAddress = channel[n].sourceAddress;
    channel[n].lineCounter = 0;
    hdmaUpdate(n);
  }

  status.irqLock = true;
}
