#ifdef SCPU_CPP

#include "irq.cpp"
#include "joypad.cpp"

unsigned sCPU::dma_counter() {
  return (status.dma_counter + ppucounter.hcounter()) & 7;
}

void sCPU::add_clocks(unsigned clocks) {
  if(status.dram_refreshed == false) {
    if(ppucounter.hcounter() + clocks >= status.dram_refresh_position) {
      status.dram_refreshed = true;
      clocks += 40;
    }
  }

  counter.sub(counter.irq_delay, clocks);
  unsigned ticks = clocks >> 1;
  while(ticks--) {
    ppucounter.tick();
    snes.input.tick();
    poll_interrupts();
  }

  scheduler.addclocks_cpu(clocks);
}

void sCPU::scanline() {
  status.dma_counter = (status.dma_counter + status.line_clocks) & 7;
  status.line_clocks = ppucounter.lineclocks();
  if(ppucounter.vcounter() == 0) frame();

  //dram refresh occurs once every scanline
  status.dram_refreshed = false;
  if(cpu_version == 2) status.dram_refresh_position = 530 + 8 - dma_counter();

  //hdma triggers once every visible scanline
  status.hdma_triggered = (ppucounter.vcounter() <= (ppu.overscan() == false ? 224 : 239)) ? false : true;

  update_interrupts();

  if(status.auto_joypad_poll == true && ppucounter.vcounter() == (ppu.overscan() == false ? 227 : 242)) {
    snes.input.poll();
    run_auto_joypad_poll();
  }
}

void sCPU::frame() {
  status.hdmainit_triggered = false;
  if(cpu_version == 1) {
    status.hdmainit_trigger_position = 12 + 8 - dma_counter();
  } else {
    status.hdmainit_trigger_position = 12 + dma_counter();
  }
}

//used for H/DMA bus synchronization
void sCPU::precycle_edge() {
  if(status.dma_state == DMA_CPUsync) {
    add_clocks(status.clock_count - (status.dma_clocks % status.clock_count));
    status.dma_state = DMA_Inactive;
  }
}

//used to test for H/DMA, which can trigger on the edge of every opcode cycle.
void sCPU::cycle_edge() {
  if(status.hdmainit_triggered == false) {
    if(ppucounter.hcounter() >= status.hdmainit_trigger_position || ppucounter.vcounter()) {
      status.hdmainit_triggered = true;
      hdma_init_reset();
      if(hdma_enabled_channels()) {
        status.hdma_pending = true;
        status.hdma_mode = 0;
      }
    }
  }

  if(status.hdma_triggered == false) {
    if(ppucounter.hcounter() >= 1104) {
      status.hdma_triggered = true;
      if(hdma_active_channels()) {
        status.hdma_pending = true;
        status.hdma_mode = 1;
      }
    }
  }

  //H/DMA pending && DMA inactive?
  //.. Run one full CPU cycle
  //.. HDMA pending && HDMA enabled ? DMA sync + HDMA run
  //.. DMA pending && DMA enabled ? DMA sync + DMA run
  //.... HDMA during DMA && HDMA enabled ? DMA sync + HDMA run
  //.. Run one bus CPU cycle
  //.. CPU sync

  if(status.dma_state == DMA_Run) {
    if(status.hdma_pending) {
      status.hdma_pending = false;
      if(hdma_enabled_channels()) {
        dma_add_clocks(8 - dma_counter()); //DMA sync
        status.hdma_mode == 0 ? hdma_init() : hdma_run();
        if(!dma_enabled_channels()) status.dma_state = DMA_CPUsync;
      }
    }

    if(status.dma_pending) {
      status.dma_pending = false;
      if(dma_enabled_channels()) {
        dma_add_clocks(8 - dma_counter()); //DMA sync
        dma_run();
        status.dma_state = DMA_CPUsync;
      }
    }
  }

  if(status.dma_state == DMA_Inactive) {
    if(status.dma_pending || status.hdma_pending) {
      status.dma_clocks = 0;
      status.dma_state = DMA_Run;
    }
  }
}

//used to test for NMI/IRQ, which can trigger on the edge of every opcode.
//test one cycle early to simulate two-stage pipeline of x816 CPU.
//
//status.irq_delay is used to simulate hardware delay before interrupts can
//trigger during certain events (immediately after DMA, writes to $4200, etc)
void sCPU::last_cycle() {
  if(counter.irq_delay) return;

  status.nmi_pending |= nmi_test();
  status.irq_pending |= irq_test();

  event.irq = (status.nmi_pending || status.irq_pending);
}

void sCPU::timing_power() {
}

void sCPU::timing_reset() {
  counter.nmi_hold  = 0;
  counter.irq_hold  = 0;

  counter.nmi_fire  = 0;
  counter.irq_fire  = 0;
  counter.irq_delay = 0;
  counter.hw_math   = 0;

  status.clock_count = 0;
  status.line_clocks = ppucounter.lineclocks();

  status.dram_refreshed = false;
  status.dram_refresh_position = (cpu_version == 1) ? 530 : 538;

  status.hdmainit_triggered = false;
  status.hdmainit_trigger_position = 0;

  status.hdma_triggered = false;

  status.irq_delay = 0;

  status.nmi_valid      = false;
  status.nmi_line       = false;
  status.nmi_transition = false;
  status.nmi_pending    = false;

  status.irq_valid      = false;
  status.irq_line       = false;
  status.irq_transition = false;
  status.irq_pending    = false;

  update_interrupts();

  status.dma_counter  = 0;
  status.dma_clocks   = 0;
  status.dma_pending  = false;
  status.hdma_pending = false;
  status.hdma_mode    = 0;
  status.dma_state    = DMA_Inactive;
}

#undef ntsc_color_burst_phase_shift_scanline

#endif  //ifdef SCPU_CPP
