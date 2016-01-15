#include "irq.cpp"
#include "joypad.cpp"

auto CPU::dma_counter() -> uint {
  return (status.dma_counter + hcounter()) & 7;
}

auto CPU::add_clocks(uint clocks) -> void {
  status.irq_lock = false;
  uint ticks = clocks >> 1;
  while(ticks--) {
    tick();
    if(hcounter() & 2) poll_interrupts();
  }

  step(clocks);

  status.auto_joypad_clock += clocks;
  if(status.auto_joypad_clock >= 256) {
    status.auto_joypad_clock -= 256;
    step_auto_joypad_poll();
  }

  if(status.dram_refreshed == false && hcounter() >= status.dram_refresh_position) {
    status.dram_refreshed = true;
    add_clocks(40);
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
    status.hdma_init_position = (cpu_version == 1 ? 12 + 8 - dma_counter() : 12 + dma_counter());
    status.hdma_init_triggered = false;

    status.auto_joypad_counter = 0;
  }

  //DRAM refresh occurs once every scanline
  if(cpu_version == 2) status.dram_refresh_position = 530 + 8 - dma_counter();
  status.dram_refreshed = false;

  //HDMA triggers once every visible scanline
  if(vcounter() <= (ppu.overscan() == false ? 224 : 239)) {
    status.hdma_position = 1104;
    status.hdma_triggered = false;
  }
}

auto CPU::alu_edge() -> void {
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

auto CPU::dma_edge() -> void {
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
      if(hdma_enabled_channels()) {
        if(!dma_enabled_channels()) {
          dma_add_clocks(8 - dma_counter());
        }
        status.hdma_mode == 0 ? hdma_init() : hdma_run();
        if(!dma_enabled_channels()) {
          add_clocks(status.clock_count - (status.dma_clocks % status.clock_count));
          status.dma_active = false;
        }
      }
    }

    if(status.dma_pending) {
      status.dma_pending = false;
      if(dma_enabled_channels()) {
        dma_add_clocks(8 - dma_counter());
        dma_run();
        add_clocks(status.clock_count - (status.dma_clocks % status.clock_count));
        status.dma_active = false;
      }
    }
  }

  if(status.hdma_init_triggered == false && hcounter() >= status.hdma_init_position) {
    status.hdma_init_triggered = true;
    hdma_init_reset();
    if(hdma_enabled_channels()) {
      status.hdma_pending = true;
      status.hdma_mode = 0;
    }
  }

  if(status.hdma_triggered == false && hcounter() >= status.hdma_position) {
    status.hdma_triggered = true;
    if(hdma_active_channels()) {
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
auto CPU::last_cycle() -> void {
  if(status.irq_lock == false) {
    status.nmi_pending |= nmi_test();
    status.irq_pending |= irq_test();
    status.interrupt_pending = (status.nmi_pending || status.irq_pending);
  }
}

auto CPU::timing_power() -> void {
}

auto CPU::timing_reset() -> void {
  status.clock_count = 0;
  status.line_clocks = lineclocks();

  status.irq_lock = false;
  status.dram_refresh_position = (cpu_version == 1 ? 530 : 538);
  status.dram_refreshed = false;

  status.hdma_init_position = (cpu_version == 1 ? 12 + 8 - dma_counter() : 12 + dma_counter());
  status.hdma_init_triggered = false;

  status.hdma_position = 1104;
  status.hdma_triggered = false;

  status.nmi_valid      = false;
  status.nmi_line       = false;
  status.nmi_transition = false;
  status.nmi_pending    = false;
  status.nmi_hold       = false;

  status.irq_valid      = false;
  status.irq_line       = false;
  status.irq_transition = false;
  status.irq_pending    = false;
  status.irq_hold       = false;

  status.reset_pending     = true;
  status.interrupt_pending = true;

  status.dma_active   = false;
  status.dma_counter  = 0;
  status.dma_clocks   = 0;
  status.dma_pending  = false;
  status.hdma_pending = false;
  status.hdma_mode    = 0;

  status.auto_joypad_active  = false;
  status.auto_joypad_latch   = false;
  status.auto_joypad_counter = 0;
  status.auto_joypad_clock   = 0;
}
