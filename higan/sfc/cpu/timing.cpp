auto CPU::dmaCounter() const -> uint {
  return (status.dmaCounter + hcounter()) & 7;
}

auto CPU::addClocks(uint clocks) -> void {
  status.irqLock = false;
  uint ticks = clocks >> 1;
  while(ticks--) {
    tick();
    if(hcounter() & 2) pollInterrupts();
  }

  step(clocks);

  status.autoJoypadClock += clocks;
  if(status.autoJoypadClock >= 256) {
    status.autoJoypadClock -= 256;
    stepAutoJoypadPoll();
  }

  if(!status.dramRefreshed && hcounter() >= status.dramRefreshPosition) {
    status.dramRefreshed = true;
    addClocks(40);
  }

  #if defined(DEBUGGER)
  synchronizeSMP();
  synchronizePPU();
  synchronizeCoprocessors();
  #endif
}

//called by ppu.tick() when Hcounter=0
auto CPU::scanline() -> void {
  status.dmaCounter = (status.dmaCounter + status.lineClocks) & 7;
  status.lineClocks = lineclocks();

  //forcefully sync S-CPU to other processors, in case chips are not communicating
  synchronizeSMP();
  synchronizePPU();
  synchronizeCoprocessors();

  if(vcounter() == 0) {
    //HDMA init triggers once every frame
    status.hdmaInitPosition = (version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
    status.hdmaInitTriggered = false;

    status.autoJoypadCounter = 0;
  }

  //DRAM refresh occurs once every scanline
  if(version == 2) status.dramRefreshPosition = 530 + 8 - dmaCounter();
  status.dramRefreshed = false;

  //HDMA triggers once every visible scanline
  if(vcounter() < ppu.vdisp()) {
    status.hdmaPosition = 1104;
    status.hdmaTriggered = false;
  }
}

auto CPU::aluEdge() -> void {
  if(alu.mpyctr) {
    alu.mpyctr--;
    if(status.rddiv & 1) status.rdmpy += alu.shift;
    status.rddiv >>= 1;
    alu.shift <<= 1;
  }

  if(alu.divctr) {
    alu.divctr--;
    status.rddiv <<= 1;
    alu.shift >>= 1;
    if(status.rdmpy >= alu.shift) {
      status.rdmpy -= alu.shift;
      status.rddiv |= 1;
    }
  }
}

auto CPU::dmaEdge() -> void {
  //H/DMA pending && DMA inactive?
  //.. Run one full CPU cycle
  //.. HDMA pending && HDMA enabled ? DMA sync + HDMA run
  //.. DMA pending && DMA enabled ? DMA sync + DMA run
  //.... HDMA during DMA && HDMA enabled ? DMA sync + HDMA run
  //.. Run one bus CPU cycle
  //.. CPU sync

  if(status.dmaActive) {
    if(status.hdmaPending) {
      status.hdmaPending = false;
      if(hdmaEnabledChannels()) {
        if(!dmaEnabledChannels()) {
          dmaAddClocks(8 - dmaCounter());
        }
        status.hdmaMode == 0 ? hdmaInit() : hdmaRun();
        if(!dmaEnabledChannels()) {
          addClocks(status.clockCount - (status.dmaClocks % status.clockCount));
          status.dmaActive = false;
        }
      }
    }

    if(status.dmaPending) {
      status.dmaPending = false;
      if(dmaEnabledChannels()) {
        dmaAddClocks(8 - dmaCounter());
        dmaRun();
        addClocks(status.clockCount - (status.dmaClocks % status.clockCount));
        status.dmaActive = false;
      }
    }
  }

  if(!status.hdmaInitTriggered && hcounter() >= status.hdmaInitPosition) {
    status.hdmaInitTriggered = true;
    hdmaInitReset();
    if(hdmaEnabledChannels()) {
      status.hdmaPending = true;
      status.hdmaMode = 0;
    }
  }

  if(!status.hdmaTriggered && hcounter() >= status.hdmaPosition) {
    status.hdmaTriggered = true;
    if(hdmaActiveChannels()) {
      status.hdmaPending = true;
      status.hdmaMode = 1;
    }
  }

  if(!status.dmaActive) {
    if(status.dmaPending || status.hdmaPending) {
      status.dmaClocks = 0;
      status.dmaActive = true;
    }
  }
}

//used to test for NMI/IRQ, which can trigger on the edge of every opcode.
//test one cycle early to simulate two-stage pipeline of x816 CPU.
//
//status.irq_lock is used to simulate hardware delay before interrupts can
//trigger during certain events (immediately after DMA, writes to $4200, etc)
auto CPU::lastCycle() -> void {
  if(!status.irqLock) {
    status.nmiPending |= nmiTest();
    status.irqPending |= irqTest();
    status.interruptPending = (status.nmiPending || status.irqPending);
  }
}
