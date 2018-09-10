//the number of clock cycles that have elapsed since (H)DMA began
auto CPU::dmaClocks() const -> uint {
  if(counter.cpu >= counter.dma) {
    return counter.cpu - counter.dma;
  } else {
    return 0 - counter.cpu + counter.dma;
  }
}

//DMA clock divider
auto CPU::dmaCounter() const -> uint {
  return counter.cpu & 7;
}

//joypad auto-poll clock divider
auto CPU::joypadCounter() const -> uint {
  return counter.cpu & 255;
}

auto CPU::step(uint clocks) -> void {
  status.irqLock = false;
  uint ticks = clocks >> 1;
  while(ticks--) {
    counter.cpu += 2;
    tick();
    if(hcounter() & 2) pollInterrupts();
    if(joypadCounter() == 0) joypadEdge();
  }

  Thread::step(clocks);
  for(auto peripheral : peripherals) synchronize(*peripheral);

  if(!status.dramRefresh && hcounter() >= status.dramRefreshPosition) {
    //note: pattern should technically be 5-3, 5-3, 5-3, 5-3, 5-3 per logic analyzer
    //result averages out the same as no coprocessor polls refresh() at > frequency()/2
    status.dramRefresh = 1; step(6); status.dramRefresh = 2; step(2); aluEdge();
    status.dramRefresh = 1; step(6); status.dramRefresh = 2; step(2); aluEdge();
    status.dramRefresh = 1; step(6); status.dramRefresh = 2; step(2); aluEdge();
    status.dramRefresh = 1; step(6); status.dramRefresh = 2; step(2); aluEdge();
    status.dramRefresh = 1; step(6); status.dramRefresh = 2; step(2); aluEdge();
  }

  if(configuration.hacks.coprocessors.delayedSync) return;
  for(auto coprocessor : coprocessors) synchronize(*coprocessor);
}

//called by ppu.tick() when Hcounter=0
auto CPU::scanline() -> void {
  status.lineClocks = lineclocks();

  //forcefully sync S-CPU to other processors, in case chips are not communicating
  synchronize(smp);
  synchronize(ppu);
  for(auto coprocessor : coprocessors) synchronize(*coprocessor);

  if(vcounter() == 0) {
    //HDMA setup triggers once every frame
    status.hdmaSetupPosition = (version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
    status.hdmaSetupTriggered = false;

    status.autoJoypadCounter = 0;
  }

  //DRAM refresh occurs once every scanline
  if(version == 2) status.dramRefreshPosition = 530 + 8 - dmaCounter();
  status.dramRefresh = 0;

  //HDMA triggers once every visible scanline
  if(vcounter() < ppu.vdisp()) {
    status.hdmaPosition = 1104;
    status.hdmaTriggered = false;
  }
}

auto CPU::aluEdge() -> void {
  if(alu.mpyctr) {
    alu.mpyctr--;
    if(io.rddiv & 1) io.rdmpy += alu.shift;
    io.rddiv >>= 1;
    alu.shift <<= 1;
  }

  if(alu.divctr) {
    alu.divctr--;
    io.rddiv <<= 1;
    alu.shift >>= 1;
    if(io.rdmpy >= alu.shift) {
      io.rdmpy -= alu.shift;
      io.rddiv |= 1;
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
      if(hdmaEnable()) {
        if(!dmaEnable()) {
          counter.dma = counter.cpu;
          step(8 - dmaCounter());
        }
        status.hdmaMode == 0 ? hdmaSetup() : hdmaRun();
        if(!dmaEnable()) {
          step(status.clockCount - dmaClocks() % status.clockCount);
          status.dmaActive = false;
        }
      }
    }

    if(status.dmaPending) {
      status.dmaPending = false;
      if(dmaEnable()) {
        counter.dma = counter.cpu;
        step(8 - dmaCounter());
        dmaRun();
        step(status.clockCount - dmaClocks() % status.clockCount);
        status.dmaActive = false;
      }
    }
  }

  if(!status.hdmaSetupTriggered && hcounter() >= status.hdmaSetupPosition) {
    status.hdmaSetupTriggered = true;
    hdmaReset();
    if(hdmaEnable()) {
      status.hdmaPending = true;
      status.hdmaMode = 0;
    }
  }

  if(!status.hdmaTriggered && hcounter() >= status.hdmaPosition) {
    status.hdmaTriggered = true;
    if(hdmaActive()) {
      status.hdmaPending = true;
      status.hdmaMode = 1;
    }
  }

  if(!status.dmaActive) {
    if(status.dmaPending || status.hdmaPending) {
      status.dmaActive = true;
    }
  }
}

//called every 256 clocks; see CPU::step()
auto CPU::joypadEdge() -> void {
  if(vcounter() >= ppu.vdisp()) {
    //cache enable state at first iteration
    if(status.autoJoypadCounter == 0) status.autoJoypadLatch = io.autoJoypadPoll;
    status.autoJoypadActive = status.autoJoypadCounter <= 15;

    if(status.autoJoypadActive && status.autoJoypadLatch) {
      if(status.autoJoypadCounter == 0) {
        controllerPort1.device->latch(1);
        controllerPort2.device->latch(1);
        controllerPort1.device->latch(0);
        controllerPort2.device->latch(0);

        //shift registers are cleared at start of auto joypad polling
        io.joy1 = 0;
        io.joy2 = 0;
        io.joy3 = 0;
        io.joy4 = 0;
      }

      uint2 port0 = controllerPort1.device->data();
      uint2 port1 = controllerPort2.device->data();

      io.joy1 = io.joy1 << 1 | port0.bit(0);
      io.joy2 = io.joy2 << 1 | port1.bit(0);
      io.joy3 = io.joy3 << 1 | port0.bit(1);
      io.joy4 = io.joy4 << 1 | port1.bit(1);
    }

    status.autoJoypadCounter++;
  }
}

//used to test for NMI/IRQ, which can trigger on the edge of every opcode.
//test one cycle early to simulate two-stage pipeline of x816 CPU.
//
//status.irq_lock is used to simulate hardware delay before interrupts can
//trigger during certain events (immediately after DMA, writes to $4200, etc)
auto CPU::lastCycle() -> void {
  if(!status.irqLock) {
    if(nmiTest()) status.nmiPending = true;
    if(irqTest()) status.irqPending = true;
    status.interruptPending = (status.nmiPending || status.irqPending);
  }
}
