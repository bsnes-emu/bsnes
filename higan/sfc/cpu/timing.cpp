auto CPU::dmaCounter() const -> uint {
  return (status.dma_counter + hcounter()) & 7;
}

auto CPU::addClocks(uint clocks) -> void {
  status.irq_lock = false;
  uint ticks = clocks >> 1;
  while(ticks--) {
    tick();
    if(hcounter() & 2) pollInterrupts();
  }

  step(clocks);

  status.auto_joypad_clock += clocks;
  if(status.auto_joypad_clock >= 256) {
    status.auto_joypad_clock -= 256;
    stepAutoJoypadPoll();
  }

  if(!status.dram_refreshed && hcounter() >= status.dram_refresh_position) {
    status.dram_refreshed = true;
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
  status.dma_counter = (status.dma_counter + status.line_clocks) & 7;
  status.line_clocks = lineclocks();

  //forcefully sync S-CPU to other processors, in case chips are not communicating
  synchronizeSMP();
  synchronizePPU();
  synchronizeCoprocessors();

  if(vcounter() == 0) {
    //HDMA init triggers once every frame
    status.hdma_init_position = (cpu_version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
    status.hdma_init_triggered = false;

    status.auto_joypad_counter = 0;
  }

  //DRAM refresh occurs once every scanline
  if(cpu_version == 2) status.dram_refresh_position = 530 + 8 - dmaCounter();
  status.dram_refreshed = false;

  //HDMA triggers once every visible scanline
  if(vcounter() < ppu.vdisp()) {
    status.hdma_position = 1104;
    status.hdma_triggered = false;
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

  if(status.dma_active == true) {
    if(status.hdma_pending) {
      status.hdma_pending = false;
      if(hdmaEnabledChannels()) {
        if(!dmaEnabledChannels()) {
          dmaAddClocks(8 - dmaCounter());
        }
        status.hdma_mode == 0 ? hdmaInit() : hdmaRun();
        if(!dmaEnabledChannels()) {
          addClocks(status.clock_count - (status.dma_clocks % status.clock_count));
          status.dma_active = false;
        }
      }
    }

    if(status.dma_pending) {
      status.dma_pending = false;
      if(dmaEnabledChannels()) {
        dmaAddClocks(8 - dmaCounter());
        dmaRun();
        addClocks(status.clock_count - (status.dma_clocks % status.clock_count));
        status.dma_active = false;
      }
    }
  }

  if(status.hdma_init_triggered == false && hcounter() >= status.hdma_init_position) {
    status.hdma_init_triggered = true;
    hdmaInitReset();
    if(hdmaEnabledChannels()) {
      status.hdma_pending = true;
      status.hdma_mode = 0;
    }
  }

  if(status.hdma_triggered == false && hcounter() >= status.hdma_position) {
    status.hdma_triggered = true;
    if(hdmaActiveChannels()) {
      status.hdma_pending = true;
      status.hdma_mode = 1;
    }
  }

  if(status.dma_active == false) {
    if(status.dma_pending || status.hdma_pending) {
      status.dma_clocks = 0;
      status.dma_active = true;
    }
  }
}

//used to test for NMI/IRQ, which can trigger on the edge of every opcode.
//test one cycle early to simulate two-stage pipeline of x816 CPU.
//
//status.irq_lock is used to simulate hardware delay before interrupts can
//trigger during certain events (immediately after DMA, writes to $4200, etc)
auto CPU::lastCycle() -> void {
  if(!status.irq_lock) {
    status.nmi_pending |= nmiTest();
    status.irq_pending |= irqTest();
    status.interrupt_pending = (status.nmi_pending || status.irq_pending);
  }
}
