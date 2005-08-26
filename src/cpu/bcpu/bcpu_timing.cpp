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

uint16 bCPU::vcounter()         { return time.v; }
uint16 bCPU::hcounter()         { return get_hcounter(); }
uint16 bCPU::hcycles()          { return time.hc; }
bool   bCPU::interlace()        { return time.interlace; }
bool   bCPU::interlace_field()  { return time.interlace_field; }
bool   bCPU::overscan()         { return time.overscan; }
uint16 bCPU::region_scanlines() { return time.region_scanlines; }

void   bCPU::set_interlace(bool r) { time.interlace = r; }
void   bCPU::set_overscan(bool r)  { time.overscan  = r; }

uint8  bCPU::dma_counter() { return (time.dma_counter + time.hc) & 6; }

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

    if(time.interlace == true && time.interlace_field == 0) {
      time.frame_lines = time.region_scanlines + 1;
    } else {
      time.frame_lines = time.region_scanlines;
    }
  }

  time.hc = 0;
  time.dma_counter = time.line_cycles & 6;
  if(time.v == 240 && time.interlace == false && time.interlace_field == 1) {
    time.line_cycles = 1360;
  } else {
    time.line_cycles = 1364;
  }
  time.dram_refreshed = false;
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

void bCPU::dram_refresh() {
  time.dram_refreshed = true;
  add_cycles(40);
  if(time.v != 240 || time.interlace != false || time.interlace_field != 1) {
  //alternate between 534 and 538 every scanline except 240ni1
  //in reality, this is probably based on frame cycle length...
    time.dram_refresh_pos ^= 12;
  }
}

uint32 bCPU::cycles_executed() {
uint32 r = status.cycles_executed;
  status.cycles_executed = 0;
  return r;
}

void bCPU::add_cycles(int cycles) {
  status.cycles_executed += cycles;

  cycles >>= 1;
  while(cycles--) {
    time.hc += 2;

    if(time.hc >= time.line_cycles) {
      inc_vcounter();
      if(time.v == 0) {
        frame();
        ppu->frame();
      }
      scanline();
      ppu->scanline();
    }

    if(time.dram_refreshed == false && time.hc >= time.dram_refresh_pos) {
      dram_refresh();
    }

    if(hdma_test() == true) {
      hdma_run();
    }
  }
}

void bCPU::time_reset() {
//initial latch values for $213c/$213d
//[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
//[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  time.v  = 0;
  time.hc = 186;

//upon SNES reset, start at scanline 0 non-interlace
  time.interlace       = false;
  time.interlace_field = false;
  time.overscan        = false;
  time.line_cycles     = 1364;

  time.dram_refreshed   = false;
  time.dram_refresh_pos = 538;

  time.dma_counter = 0;

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
