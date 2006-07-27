#define ntsc_color_burst_phase_shift_scanline() \
  (status.region == SNES::NTSC && status.vcounter == 240 && \
   status.interlace == false && status.interlace_field == 1)

#include "irq.cpp"
#include "joypad.cpp"

uint16 sCPU::vcounter() { return status.vcounter; }
uint16 sCPU::hcycles()  { return status.hclock;   }

bool   sCPU::interlace()        { return status.interlace;        }
bool   sCPU::interlace_field()  { return status.interlace_field;  }
bool   sCPU::overscan()         { return status.overscan;         }
uint16 sCPU::region_scanlines() { return status.region_scanlines; }

void   sCPU::set_interlace(bool r) { status.interlace = r; update_interrupts(); }
void   sCPU::set_overscan (bool r) { status.overscan  = r; update_interrupts(); }

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
  if(status.irq_delay) {
    if(status.irq_delay >= clocks) {
      status.irq_delay -= clocks;
    } else {
      status.irq_delay  = 0;
    }
  }

  status.clocks_executed += clocks;
  poll_interrupts(clocks);

  if(status.hclock + clocks >= status.line_clocks) {
    clocks = (status.hclock + clocks) - status.line_clocks;
    status.hclock = 0;
    scanline();
    poll_interrupts(clocks);
  }

  status.hclock += clocks;

  if(status.dram_refreshed == false) {
    if(status.hclock >= 530) {
      status.dram_refreshed = true;
      add_clocks(40);
      return;
    }
  }
}

void sCPU::scanline() {
  if(++status.vcounter >= status.field_lines) {
    frame();
  }

  status.line_clocks = (ntsc_color_burst_phase_shift_scanline() == false) ? 1364 : 1360;

//dram refresh occurs once every scanline
  status.dram_refreshed = false;

//hdma triggers once every visible scanline
  status.line_rendered  =
  status.hdma_triggered = (status.vcounter <= (!overscan() ? 224 : 239)) ? false : true;

  r_ppu->scanline();
  snes->scanline();

  update_interrupts();

  if(status.auto_joypad_poll == true && status.vcounter == (!overscan() ? 227 : 242)) {
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

  r_ppu->frame();
  snes->frame();
}

/*****
 * precycle_edge()
 *
 * Used for DMA/HDMA bus synchronization
 *****/
void sCPU::precycle_edge() {
}

/*****
 * cycle_edge()
 *
 * Used to test for HDMA, which can trigger on the edge of every opcode cycle.
 *****/
void sCPU::cycle_edge() {
  if(status.line_rendered == false) {
    if(status.hclock >= 128) {
      status.line_rendered = true;
      r_ppu->render_scanline();
    }
  }

  if(status.hdmainit_triggered == false) {
    if(status.hclock >= 12 || status.vcounter) {
      status.hdmainit_triggered = true;
      hdma_init();
    }
  }

  if(status.hdma_triggered == false) {
    if(status.hclock >= 1106) {
      status.hdma_triggered = true;
      hdma_run();
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
  if(status.irq_delay)return;

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
  status.clock_count     = 0;
  status.clocks_executed = 0;

  status.vcounter = 0;
  status.hcounter = 0;
  status.hclock   = 0;

  status.interlace        = 0;
  status.interlace_field  = 0;
  status.overscan         = false;
  status.region_scanlines = (status.region == SNES::NTSC) ? 525 : 625;

  status.field_lines = status.region_scanlines >> 1;
  status.line_clocks = 1364;

  status.line_rendered      = false;
  status.dram_refreshed     = false;
  status.hdmainit_triggered = false;
  status.hdma_triggered     = false;

  status.irq_delay = 0;

  status.nmi_read       = 1;
  status.nmi_line       = 1;
  status.nmi_transition = 0;
  status.nmi_pending    = false;

  status.irq_read       = 1;
  status.irq_line       = 1;
  status.irq_transition = 0;
  status.irq_pending    = false;

  update_interrupts();

//initial latch values for $213c/$213d
//[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
//[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  add_clocks(186);
}

#undef ntsc_color_burst_phase_shift_scanline
