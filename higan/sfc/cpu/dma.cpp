auto CPU::dmaAddClocks(uint clocks) -> void {
  status.dma_clocks += clocks;
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
  switch(channel[n].transfer_mode) { default:
  case 0: return (channel[n].dest_addr);                       //0
  case 1: return (channel[n].dest_addr + (index & 1));         //0,1
  case 2: return (channel[n].dest_addr);                       //0,0
  case 3: return (channel[n].dest_addr + ((index >> 1) & 1));  //0,0,1,1
  case 4: return (channel[n].dest_addr + (index & 3));         //0,1,2,3
  case 5: return (channel[n].dest_addr + (index & 1));         //0,1,0,1
  case 6: return (channel[n].dest_addr);                       //0,0     [2]
  case 7: return (channel[n].dest_addr + ((index >> 1) & 1));  //0,0,1,1 [3]
  }
}

inline auto CPU::dmaAddress(uint n) -> uint24 {
  uint24 addr = channel[n].source_bank << 16 | channel[n].source_addr;

  if(!channel[n].fixed_transfer) {
    if(!channel[n].reverse_transfer) {
      channel[n].source_addr++;
    } else {
      channel[n].source_addr--;
    }
  }

  return addr;
}

inline auto CPU::hdmaAddress(uint n) -> uint24 {
  return channel[n].source_bank << 16 | channel[n].hdma_addr++;
}

inline auto CPU::hdmaIndirectAddress(uint n) -> uint24 {
  return channel[n].indirect_bank << 16 | channel[n].indirect_addr++;
}

//==============
//channel status
//==============

auto CPU::dmaEnabledChannels() -> uint {
  uint count = 0;
  for(auto n : range(8)) count += channel[n].dma_enabled;
  return count;
}

inline auto CPU::hdmaActive(uint n) -> bool {
  return channel[n].hdma_enabled && !channel[n].hdma_completed;
}

inline auto CPU::hdmaActiveAfter(uint s) -> bool {
  for(uint n = s + 1; n < 8; n++) {
    if(hdmaActive(n)) return true;
  }
  return false;
}

inline auto CPU::hdmaEnabledChannels() -> uint {
  uint count = 0;
  for(auto n : range(8)) count += channel[n].hdma_enabled;
  return count;
}

inline auto CPU::hdmaActiveChannels() -> uint {
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
    if(!channel[n].dma_enabled) continue;

    uint index = 0;
    do {
      dmaTransfer(channel[n].direction, dmaAddressB(n, index++), dmaAddress(n));
      dmaEdge();
    } while(channel[n].dma_enabled && --channel[n].transfer_size);

    dmaAddClocks(8);
    dmaWrite(false);
    dmaEdge();

    channel[n].dma_enabled = false;
  }

  status.irq_lock = true;
}

auto CPU::hdmaUpdate(uint n) -> void {
  dmaAddClocks(4);
  r.mdr = dmaRead(channel[n].source_bank << 16 | channel[n].hdma_addr);
  dmaAddClocks(4);
  dmaWrite(false);

  if((channel[n].line_counter & 0x7f) == 0) {
    channel[n].line_counter = r.mdr;
    channel[n].hdma_addr++;

    channel[n].hdma_completed = channel[n].line_counter == 0;
    channel[n].hdma_do_transfer = !channel[n].hdma_completed;

    if(channel[n].indirect) {
      dmaAddClocks(4);
      r.mdr = dmaRead(hdmaAddress(n));
      channel[n].indirect_addr = r.mdr << 8;
      dmaAddClocks(4);
      dmaWrite(false);

      if(!channel[n].hdma_completed || hdmaActiveAfter(n)) {
        dmaAddClocks(4);
        r.mdr = dmaRead(hdmaAddress(n));
        channel[n].indirect_addr >>= 8;
        channel[n].indirect_addr |= r.mdr << 8;
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
    channel[n].dma_enabled = false;  //HDMA run during DMA will stop DMA mid-transfer

    if(channel[n].hdma_do_transfer) {
      static const uint transferLength[8] = {1, 2, 2, 4, 4, 4, 2, 4};
      uint length = transferLength[channel[n].transfer_mode];
      for(auto index : range(length)) {
        uint addr = !channel[n].indirect ? hdmaAddress(n) : hdmaIndirectAddress(n);
        dmaTransfer(channel[n].direction, dmaAddressB(n, index), addr);
      }
    }
  }

  for(auto n : range(8)) {
    if(!hdmaActive(n)) continue;

    channel[n].line_counter--;
    channel[n].hdma_do_transfer = channel[n].line_counter & 0x80;
    hdmaUpdate(n);
  }

  status.irq_lock = true;
}

auto CPU::hdmaInitReset() -> void {
  for(auto n : range(8)) {
    channel[n].hdma_completed = false;
    channel[n].hdma_do_transfer = false;
  }
}

auto CPU::hdmaInit() -> void {
  dmaAddClocks(8);
  dmaWrite(false);

  for(auto n : range(8)) {
    if(!channel[n].hdma_enabled) continue;
    channel[n].dma_enabled = false;  //HDMA init during DMA will stop DMA mid-transfer

    channel[n].hdma_addr = channel[n].source_addr;
    channel[n].line_counter = 0;
    hdmaUpdate(n);
  }

  status.irq_lock = true;
}
