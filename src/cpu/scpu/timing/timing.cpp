#define ntsc_color_burst_phase_shift_scanline() \
  (status.region == SNES::NTSC && status.vcounter == 240 && \
   status.interlace == false && status.interlace_field == 1)

#include "timeshift.cpp"
#include "irq.cpp"
#include "joypad.cpp"

uint16 sCPU::vcounter() { return status.vcounter; }
uint16 sCPU::hclock()   { return status.hclock;   }

bool   sCPU::interlace()        { return status.interlace;        }
bool   sCPU::interlace_field()  { return status.interlace_field;  }
bool   sCPU::overscan()         { return status.overscan;         }
uint16 sCPU::region_scanlines() { return status.region_scanlines; }

uint   sCPU::dma_counter() { return (status.dma_counter + status.hclock) & 7; }

void sCPU::set_interlace(bool r) {
  status.interlace = r;
  update_interrupts();
}

void sCPU::set_overscan (bool r) {
  status.overscan = r;
  status.vblstart = (status.overscan == false) ? 225 : 240;
  update_interrupts();
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
uint16 sCPU::hcounter() {
  if(ntsc_color_burst_phase_shift_scanline() == true) {
    return (status.hclock >> 2);
  }
  return (status.hclock - ((status.hclock > 1292) << 1) - ((status.hclock > 1310) << 1)) >> 2;
}

void sCPU::add_clocks(uint clocks) {
  status.clocks_executed += clocks;

  if(counter.irq_delay) {
    counter.irq_delay = (counter.irq_delay > clocks) ? (counter.irq_delay - clocks) : 0;
  }

  if(status.hclock + clocks >= status.line_clocks) {
    clocks = (status.hclock + clocks) - status.line_clocks;
    while(status.hclock < status.line_clocks - 2) { tick(); }
    scanline();
  }

  clocks >>= 1;
  while(clocks--) { tick(); }
}

alwaysinline void sCPU::tick() {
  status.hclock += 2;

  if(counter.nmi_fire) { nmi_tick(); }
  if(counter.irq_fire) { irq_tick(); }

  if(status.dram_refreshed == false && status.hclock >= status.dram_refresh_position) {
    status.dram_refreshed = true;
    add_clocks(40);
    return;
  }

  poll_interrupts();
}

void sCPU::scanline() {
  status.hclock = 0;
  status.dma_counter = (status.dma_counter + status.line_clocks) & 7;

  if(++status.vcounter >= status.field_lines) {
    frame();
  }

  status.line_clocks = (ntsc_color_burst_phase_shift_scanline() == false) ? 1364 : 1360;

//dram refresh occurs once every scanline
  status.dram_refreshed = false;
  if(cpu_version == 2) {
    if(ntsc_color_burst_phase_shift_scanline() == false) {
      if(status.dram_refresh_position == 534) {
        status.dram_refresh_position = 538;
      } else {
        status.dram_refresh_position = 534;
      }
    }
  }

//hdma triggers once every visible scanline
  status.line_rendered  = false;
  status.hdma_triggered = (status.vcounter <= (!overscan() ? 224 : 239)) ? false : true;

  r_ppu->scanline();
  snes->scanline();

  update_interrupts();

  if(status.auto_joypad_poll == true && status.vcounter == (!overscan() ? 227 : 242)) {
    snes->poll_input();
    run_auto_joypad_poll();
  }
}

void sCPU::frame() {
  status.nmi_read = 1;
  status.nmi_line = 1;
  status.nmi_transition = 0;

  status.vcounter = 0;
  status.interlace_field ^= 1;
  status.field_lines = (status.region_scanlines >> 1);
//interlaced even fields have one extra scanline
//(263+262=525 NTSC, 313+312=625 PAL)
  if(status.interlace == true && status.interlace_field == 0)status.field_lines++;

  status.hdmainit_triggered = false;
  if(cpu_version == 1) {
    status.hdmainit_trigger_position = 12 + 8 - dma_counter();
  } else {
    status.hdmainit_trigger_position = 12 + dma_counter();
  }

  r_ppu->frame();
  snes->frame();
}

/*****
 * precycle_edge()
 *
 * Used for DMA/HDMA bus synchronization
 *****/
alwaysinline void sCPU::precycle_edge() {
  if(status.dma_state == DMASTATE_CPUSYNC) {
    status.dma_state = DMASTATE_INACTIVE;
  uint n = status.clock_count - (status.dma_clocks % status.clock_count);
    add_clocks(n ? n : status.clock_count);
  }
}

/*****
 * cycle_edge()
 *
 * Used to test for HDMA, which can trigger on the edge of every opcode cycle.
 *****/
void sCPU::cycle_edge() {
  if(status.line_rendered == false) {
    if(status.hclock >= status.line_render_position) {
      status.line_rendered = true;
      r_ppu->render_scanline();
    }
  }

  switch(status.dma_state) {
  case DMASTATE_INACTIVE:
    break;

  case DMASTATE_DMASYNC:
    status.dma_state = DMASTATE_RUN;
    break;

  case DMASTATE_RUN:
    status.dma_state = DMASTATE_CPUSYNC;
    status.dma_clocks = 8 - dma_counter() + 8;
    add_clocks(status.dma_clocks);

    if(status.hdmainit_pending) { hdma_init(); status.hdmainit_pending = false; }
    if(status.hdma_pending)     { hdma_run();  status.hdma_pending     = false; }
    if(status.dma_pending)      { dma_run();   status.dma_pending      = false; }

    break;
  }

  if(status.hdmainit_triggered == false) {
    if(status.hclock >= status.hdmainit_trigger_position || status.vcounter) {
      status.hdmainit_triggered = true;

      hdma_init_reset();
      if(hdma_enabled_channels()) {
        if(status.dma_state == DMASTATE_INACTIVE) {
          status.dma_state = DMASTATE_DMASYNC;
          status.hdmainit_pending = true;
        } else {
          hdma_init();
        }
      }
    }
  }

  if(status.hdma_triggered == false) {
    if(status.hclock >= 1106) {
      status.hdma_triggered = true;
      if(hdma_active_channels()) {
        if(status.dma_state == DMASTATE_INACTIVE) {
          status.dma_state = DMASTATE_DMASYNC;
          status.hdma_pending = true;
        } else {
          hdma_run();
        }
      }
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
  if(counter.irq_delay) { return; }

  status.nmi_pending |= nmi_test();
  status.irq_pending |= irq_test();

  event.irq = (status.nmi_pending || status.irq_pending);
}

/*****
 * clocks_executed()
 *
 * Return number of clocks executed since last call to this function.
 * Used by class SNES to control CPU<>APU synchronization.
 *****/
uint32 sCPU::clocks_executed() {
uint32 r = status.clocks_executed;
  status.clocks_executed = 0;
  return r;
}

void sCPU::timing_power() {
}

void sCPU::timing_reset() {
  counter.enabled   = false;
  counter.irq_delay = 0;
  counter.hw_math   = 0;
  counter.irq_fire  = 0;
  counter.nmi_fire  = 0;

  status.clock_count     = 0;
  status.clocks_executed = 0;

  status.vcounter = 0;
  status.hcounter = 0;
  status.hclock   = 0;

  status.interlace        = 0;
  status.interlace_field  = 0;
  status.overscan         = false;
  status.region_scanlines = (status.region == SNES::NTSC) ? 525 : 625;
  status.vblstart         = 225;

  status.field_lines = status.region_scanlines >> 1;
  status.line_clocks = 1364;

  status.line_rendered        = false;
  status.line_render_position = minmax<0, 1112>((uint16)config::ppu.render_scanline_position);

  status.dram_refreshed        = false;
  status.dram_refresh_position = (cpu_version == 1) ? 530 : 538;

  status.hdmainit_triggered        = false;
  status.hdmainit_trigger_position = 0;

  status.hdma_triggered = false;

  status.irq_delay = 0;

  status.nmi_read       = 1;
  status.nmi_line       = 1;
  status.nmi_transition = 0;
  status.nmi_pending    = false;

  status.irq_read       = 1;
  status.irq_line       = 1;
  status.irq_transition = 0;
  status.irq_lock       = false;
  status.irq_pending    = false;

  update_interrupts();

  status.dma_counter      = 0;
  status.dma_state        = DMASTATE_INACTIVE;
  status.dma_pending      = false;
  status.hdma_pending     = false;
  status.hdmainit_pending = false;

//initial latch values for $213c/$213d
//[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
//[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  add_clocks(186);
}

#undef ntsc_color_burst_phase_shift_scanline
