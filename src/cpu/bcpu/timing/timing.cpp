/* Notes about PAL timing:
 * As I do not have PAL hardware to run timing tests on, I've
 * had to guess on a lot of things. Below is how I've arrived
 * at various calculations:
 *
 * NTSC timing crystal: ~21477272hz
 * PAL  timing crystal: ~21281370hz
 * NTSC ~60fps, PAL ~50fps
 * NTSC ~262 lines/frame, PAL ~312 lines/frame
 * NTSC 21477272 / (262 * 60) = ~1366 cycles/line
 * PAL  21281370 / (312 * 50) = ~1364 cycles/line
 *
 * As the cycles/line are very close between the two systems,
 * I have left the known NTSC anomalies intact for PAL timing.
 * In reality, some of these may not exist, and some may be
 * slightly different.
 *
 * [known]
 * - DRAM refresh occurs at about the same time every
 * scanline on PAL units (per Overload).
 * [unknown]
 * - Are dots 323/327 still 2 cycles longer than the
 * other dots?
 * - Is scanline 240 on non-interlace odd frames still
 * 4 cycles short?
 */

uint16 bCPU::vcounter() { return time.v; }
uint16 bCPU::hcounter() { return get_hcounter(); }
uint16 bCPU::hcycles()  { return time.hc; }

bool   bCPU::interlace()        { return time.interlace; }
bool   bCPU::interlace_field()  { return time.interlace_field; }
bool   bCPU::overscan()         { return time.overscan; }
uint16 bCPU::region_scanlines() { return time.region_scanlines; }

void   bCPU::set_interlace(bool r) { time.interlace = r; update_interrupts(); }
void   bCPU::set_overscan (bool r) { time.overscan  = r; update_interrupts(); }

uint8  bCPU::dma_counter() { return (time.dma_counter + time.hc) & 6; }

bool bCPU::nmi_trigger_pos_match(uint32 offset) {
uint16 v  = overscan() ? 240 : 225;
uint16 hc = 2 + offset;
  return (time.v == v && time.hc == hc);
}

bool bCPU::irq_trigger_pos_match(uint32 offset) {
uint16 v  = status.virq_pos;
uint16 hc = (status.hirq_enabled) ? status.hirq_pos : 0;

//positions that can never be latched
//region_scanlines() = 262/NTSC, 312/PAL
//PAL results are unverified on hardware
  if(v == 240 && hc == 339 && interlace() == false && interlace_field() == 1)return false;
  if(v == (region_scanlines() - 1) && hc == 339 && interlace() == false)return false;
  if(v == region_scanlines() && interlace() == false)return false;
  if(v == region_scanlines() && hc == 339)return false;
  if(v  > region_scanlines())return false;
  if(hc > 339)return false;

  hc  = (hc != 0) ? ((hc << 2) + 14) : 10;
  hc += offset;
  if(hc >= time.line_cycles) {
    hc -= time.line_cycles;
    if(++v >= time.frame_lines) {
      v = 0;
    }
  }

  if((status.virq_enabled == true && time.v == v) || status.virq_enabled == false) {
    return (time.hc == hc);
  }

  return false;
}

void bCPU::update_nmi() {
  if(time.v == (overscan() ? 240 : 225)) {
    time.nmi_read_trigger_pos = 2;
    time.nmi_line_trigger_pos = 6;
  } else {
    time.nmi_read_trigger_pos = -64;
    time.nmi_line_trigger_pos = -64;
  }
}

void bCPU::update_irq() {
int vpos = status.virq_pos;
int hpos = (status.hirq_enabled) ? status.hirq_pos : 0;

//positions that can never be latched
//region_scanlines() = 262/NTSC, 312/PAL
//PAL results are unverified on hardware
  if(vpos == 240 && hpos == 339 && interlace() == false && interlace_field() == 1)goto _nolatch;
  if(vpos == (region_scanlines() - 1) && hpos == 339 && interlace() == false)goto _nolatch;
  if(vpos == region_scanlines() && interlace() == false)goto _nolatch;
  if(vpos == region_scanlines() && hpos == 339)goto _nolatch;
  if(vpos  > region_scanlines())goto _nolatch;
  if(hpos  > 339)goto _nolatch;

  hpos = (hpos != 0) ? ((hpos << 2) + 14) : 10;
  if(hpos >= time.line_cycles) {
    hpos -= time.line_cycles;
    if(++vpos >= time.frame_lines) {
      vpos = 0;
    }
  }

  if((status.virq_enabled == true && time.v == vpos) || status.virq_enabled == false) {
    time.irq_read_trigger_pos = hpos;
  } else {
    time.irq_read_trigger_pos = -64;
  }

  hpos += 4;
  if(hpos >= time.line_cycles) {
    hpos -= time.line_cycles;
    if(++vpos >= time.frame_lines) {
      vpos = 0;
    }
  }

  if((status.virq_enabled == true && time.v == vpos) || status.virq_enabled == false) {
    time.irq_line_trigger_pos = hpos;
  } else {
    time.irq_line_trigger_pos = -64;
  }

  return;

_nolatch:
  time.irq_read_trigger_pos = -64;
  time.irq_line_trigger_pos = -64;
}

void bCPU::update_interrupts() {
  update_nmi();
  update_irq();
}

void bCPU::poll_interrupts(int cycles) {
int16 hc, hc_end;
  if(time.hc == 0) {
    hc     = -1;
    hc_end = cycles;
  } else {
    hc     = time.hc;
    hc_end = time.hc + cycles;
  }

  if(hc < time.nmi_read_trigger_pos && time.nmi_read_trigger_pos <= hc_end) {
  //nmi_read can go low even with NMI interrupts disabled in $4200.d7
    time.nmi_read = 0;
  }

  if(hc < time.nmi_line_trigger_pos && time.nmi_line_trigger_pos <= hc_end) {
    if(status.nmi_enabled == true) {
      if(time.nmi_line == 1) {
        time.nmi_transition = 1;
      }
      time.nmi_line = 0;
    }
  }

  if(hc < time.irq_read_trigger_pos && time.irq_read_trigger_pos <= hc_end) {
    if(status.virq_enabled == true || status.hirq_enabled == true) {
      time.irq_read = 0;
    }
  }

  if(hc < time.irq_line_trigger_pos && time.irq_line_trigger_pos <= hc_end) {
    if(status.virq_enabled == true || status.hirq_enabled == true) {
      time.irq_line = 0;
      time.irq_transition = 1;
    }
  }
}

//all scanlines are 1364 cycles long, except scanline 240
//on non-interlace odd-frames, which is 1360 cycles long.
//[NTSC]
//interlace mode has 525 scanlines: 263 on the even frame,
//and 262 on the odd.
//non-interlace mode has 524 scanlines: 262 scanlines on
//both even and odd frames.
//[PAL] <PAL info is unverified on hardware>
//interlace mode has 625 scanlines: 313 on the even frame,
//and 312 on the odd.
//non-interlace mode has 624 scanlines: 312 scanlines on
//both even and odd frames.
//
//cycles per frame:
//  263 * 1364     = 358732
//  262 * 1364     = 357368
//  262 * 1364 - 4 = 357364
void bCPU::inc_vcounter() {
  time.v++;
  if(time.v >= time.frame_lines) {
    time.v = 0;
    time.interlace_field ^= 1;

    if(interlace() == true && interlace_field() == 0) {
      time.frame_lines = time.region_scanlines + 1;
    } else {
      time.frame_lines = time.region_scanlines;
    }
  }

  time.dma_counter += time.line_cycles;
  if(time.v == 240 && time.interlace == false && time.interlace_field == 1) {
    time.line_cycles = 1360;
  } else {
    time.line_cycles = 1364;
  }
  time.dram_refreshed = false;

  update_interrupts();
}

//all dots are 4 cycles long, except dots 323 and 327. dots 323 and 327
//are 6 cycles long. this holds true for all scanlines except scanline
//240 on non-interlace odd frames. the reason for this is because this
//scanline is only 1360 cycles long, instead of 1364 like all other
//scanlines.
//this makes the effective range of hscan_pos 0-339 at all times.
//dot 323 range = { 1292, 1294, 1296 }
//dot 327 range = { 1310, 1312, 1314 }
uint16 bCPU::get_hcounter() {
  if(time.v == 240 && time.interlace == false && time.interlace_field == 1) {
    return time.hc >> 2;
  }
  return (time.hc - ((time.hc > 1292) << 1) - ((time.hc > 1310) << 1)) >> 2;
}

uint32 bCPU::cycles_executed() {
uint32 r = status.cycles_executed;
  status.cycles_executed = 0;
  return r;
}

void bCPU::cycle_edge() {
  if(time.hdmainit_triggered == false) {
    if(time.hc >= time.hdmainit_trigger_pos || time.v) {
      time.hdmainit_triggered = true;
      hdmainit_activate();
    }
  }

  if(time.hdma_triggered == false) {
    if(time.v <= (overscan() ? 239 : 224)) {
      if(time.hc >= 1106) {
        time.hdma_triggered = true;
        hdma_activate();
      }
    }
  }
}

void bCPU::add_cycles(int cycles) {
  status.cycles_executed += cycles;

  poll_interrupts(cycles);

  if(time.hc + cycles >= time.line_cycles) {
    cycles  = (time.hc + cycles) - time.line_cycles;
    time.hc = 0;

    inc_vcounter();
    poll_interrupts(cycles);

    if(time.v == 0) {
      frame();
      r_ppu->frame();
      snes->frame();
    }

    scanline();
    r_ppu->scanline();
    snes->scanline();
    time.line_rendered = false;
  }

  if(time.dram_refreshed == false) {
    if(time.hc + cycles >= time.dram_refresh_pos) {
      time.dram_refreshed = true;
      status.cycles_executed += 40;
      cycles  = (time.hc + cycles) - time.dram_refresh_pos;
      time.hc = time.dram_refresh_pos + 40;

      if(cpu_version == 2) {
        if(time.v != 240 || time.interlace != false || time.interlace_field != 1) {
          if(time.dram_refresh_pos == 534) {
            time.dram_refresh_pos = 538;
          } else {
            time.dram_refresh_pos = 534;
          }
        }
      }
    }
  }

  if(time.line_rendered == false) {
  //rendering should start at H=18 (+256=274), but since the
  //current PPU emulation renders the entire scanline at once,
  //PPU register changes mid-scanline do not show up.
  //therefore, wait a few dots before rendering the scanline
    if(time.hc + cycles >= (48 * 4)) {
      time.line_rendered = true;
      r_ppu->render_scanline();
    }
  }

  time.hc += cycles;
}

void bCPU::time_reset() {
  time.v  = 0;
  time.hc = 0;

//upon SNES reset, start at scanline 0 non-interlace
  time.interlace       = 0;
  time.interlace_field = 0;
  time.overscan        = false;
  time.line_cycles     = 1364;

  time.dram_refreshed   = false;
  time.dram_refresh_pos = (cpu_version == 2) ? 538 : 530;

  time.dma_counter = 0;

//set at V=0,H=0
  time.hdmainit_trigger_pos = 0;
  time.hdmainit_triggered   = true;

  time.hdma_triggered = false;

  time.nmi_pending = false;
  time.irq_pending = false;
  time.nmi_line = time.nmi_read = 1;
  time.irq_line = time.irq_read = 1;

  time.nmi_transition = 0;
  time.irq_transition = 0;

  update_interrupts();

  switch(region) {
  case NTSC:
    time.region_scanlines = 262;
    break;
  case PAL:
    time.region_scanlines = 312;
    break;
  }

  time.frame_lines = time.region_scanlines;
}
