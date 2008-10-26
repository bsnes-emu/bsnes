#ifdef SCPU_CPP

#include "irq.cpp"
#include "joypad.cpp"

unsigned sCPU::dma_counter() {
  return (status.dma_counter + status.hcounter) & 7;
}

/*****
 * One PPU dot = 4 CPU clocks
 *
 * PPU dots 323 and 327 are 6 CPU clocks long.
 * This does not apply to NTSC non-interlace scanline 240 on odd fields. This is
 * because the PPU skips one dot to alter the color burst phase of the video signal.
 *
 * Dot 323 range = { 1292, 1294, 1296 }
 * Dot 327 range = { 1310, 1312, 1314 }
 *****/

#define ntsc_color_burst_phase_shift_scanline() ( \
  snes.region() == SNES::NTSC && status.vcounter == 240 && \
  ppu.interlace() == false && ppu.field() == 1 \
)

uint16 sCPU::hdot() {
  if(ntsc_color_burst_phase_shift_scanline() == true) return (status.hcounter >> 2);
  return (status.hcounter - ((status.hcounter > 1292) << 1) - ((status.hcounter > 1310) << 1)) >> 2;
}

void sCPU::add_clocks(unsigned clocks) {
  if(status.dram_refreshed == false) {
    if(status.hcounter + clocks >= status.dram_refresh_position) {
      status.dram_refreshed = true;
      clocks += 40;
    }
  }

  counter.sub(counter.irq_delay, clocks);
  scheduler.addclocks_cpu(clocks);

  clocks >>= 1;
  while(clocks--) {
    history.enqueue(status.vcounter, status.hcounter);
    status.hcounter += 2;
    if(status.hcounter >= status.line_clocks) scanline();
    poll_interrupts();
    snes.input.tick();
  }
}

void sCPU::scanline() {
  status.hcounter = 0;
  status.dma_counter = (status.dma_counter + status.line_clocks) & 7;
  if(++status.vcounter >= status.field_lines) frame();
  status.line_clocks = (ntsc_color_burst_phase_shift_scanline() == false) ? 1364 : 1360;

  //dram refresh occurs once every scanline
  status.dram_refreshed = false;
  if(cpu_version == 2) status.dram_refresh_position = 530 + 8 - dma_counter();

  //hdma triggers once every visible scanline
  status.line_rendered  = false;
  status.hdma_triggered = (status.vcounter <= (ppu.overscan() == false ? 224 : 239)) ? false : true;

  ppu.scanline();
  snes.scanline();

  update_interrupts();

  if(status.auto_joypad_poll == true && status.vcounter == (ppu.overscan() == false ? 227 : 242)) {
    snes.input.poll();
    run_auto_joypad_poll();
  }
}

void sCPU::frame() {
  ppu.frame();
  snes.frame();

  status.vcounter = 0;
  status.field_lines = (snes.region() == SNES::NTSC ? 525 : 625) >> 1;
  //interlaced even fields have one extra scanline (263+262=525 NTSC, 313+312=625 PAL)
  if(ppu.interlace() == true && ppu.field() == 0) status.field_lines++;

  status.hdmainit_triggered = false;
  if(cpu_version == 1) {
    status.hdmainit_trigger_position = 12 + 8 - dma_counter();
  } else {
    status.hdmainit_trigger_position = 12 + dma_counter();
  }
}

/*****
 * precycle_edge()
 *
 * Used for H/DMA bus synchronization
 *****/
void sCPU::precycle_edge() {
  if(status.dma_state == DMA_CPUsync) {
    add_clocks(status.clock_count - (status.dma_clocks % status.clock_count));
    status.dma_state = DMA_Inactive;
  }
}

/*****
 * cycle_edge()
 *
 * Used to test for H/DMA, which can trigger on the edge of every opcode cycle.
 *****/
void sCPU::cycle_edge() {
  if(status.line_rendered == false) {
    if(status.hcounter >= status.line_render_position) {
      status.line_rendered = true;
      ppu.render_scanline();
    }
  }

  if(status.hdmainit_triggered == false) {
    if(status.hcounter >= status.hdmainit_trigger_position || status.vcounter) {
      status.hdmainit_triggered = true;
      hdma_init_reset();
      if(hdma_enabled_channels()) {
        status.hdma_pending = true;
        status.hdma_mode = 0;
      }
    }
  }

  if(status.hdma_triggered == false) {
    if(status.hcounter >= 1104) {
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

/*****
 * last_cycle()
 *
 * Used to test for NMI/IRQ, which can trigger on the edge of every opcode.
 * Test one cycle early to simulate two-stage pipeline of x816 CPU.
 *
 * status.irq_delay is used to simulate hardware delay before interrupts can
 * trigger during certain events (immediately after DMA, writes to $4200, etc)
 *****/
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

  status.vcounter = 0;
  status.hcounter = 0;

  status.field_lines = (snes.region() == SNES::NTSC ? 525 : 625) >> 1;
  status.line_clocks = 1364;

  status.line_rendered = false;
  status.line_render_position = min(1112U, (unsigned)config::ppu.hack.render_scanline_position);

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

  history.reset();

  //initial latch values for $213c/$213d
  //[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
  //[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  add_clocks(186);
}

#undef ntsc_color_burst_phase_shift_scanline

#endif //ifdef SCPU_CPP
