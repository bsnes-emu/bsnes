uint16 bCPU::vcounter()        { return time.v; }
uint16 bCPU::hcounter()        { return get_hcounter(); }
uint16 bCPU::hcycles()         { return time.hc; }
bool   bCPU::interlace()       { return time.interlace; }
bool   bCPU::interlace_field() { return time.interlace_field; }
bool   bCPU::overscan()        { return time.overscan; }

void   bCPU::set_interlace(bool r) { time.interlace = r; }
void   bCPU::set_overscan(bool r)  { time.overscan  = r; }

uint8  bCPU::dma_counter()       { return (time.dma_counter + time.hc) & 6; }

//all scanlines are 1364 cycles long, except scanline 240
//on non-interlace odd-frames, which is 1360 cycles long.
//interlace mode has 525 scanlines: 263 on the even frame,
//and 262 on the odd.
//non-interlace mode has 524 scanlines: 262 scanlines on
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
      time.frame_lines = 263;
    } else {
      time.frame_lines = 262;
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

//all dots are 4 cycles long, except dots 322 and 326. dots 322 and 326
//are 6 cycles long. this holds true for all scanlines except scanline
//240 on non-interlace odd frames. the reason for this is because this
//scanline is only 1360 cycles long, instead of 1364 like all other
//scanlines.
//this makes the effective range of hscan_pos 0-339 at all times.
//dot 322 range = { 1288, 1290, 1292 }
//dot 326 range = { 1306, 1308, 1310 }
uint16 bCPU::get_hcounter() {
  if(time.v == 240 && time.interlace == false && time.interlace_field == 1) {
    return time.hc >> 2;
  }
  return (time.hc - ((time.hc > 1288) << 1) - ((time.hc > 1306) << 1)) >> 2;
}

void bCPU::apu_sync() {
int cycles;
  while(apusync.cycles >= 0) {
    apu->run();

    cycles = apu->cycles_executed();
    if(cycles) {
      apusync.cycles -= apusync.cpu_multbl[cycles];
    }
  }
}

void bCPU::dram_refresh() {
  time.dram_refreshed = true;
  add_cycles(40);
  if(time.v != 240 || time.interlace != false || time.interlace_field != 1) {
  //alternate between 534 and 538 every scanline except 240ni1
    time.dram_refresh_pos ^= 12;
  }
}

void bCPU::add_cycles(int cycles) {
  cycles >>= 1;
  while(cycles--) {
    apusync.cycles += apusync.apu_freq << 1;
    apu_sync();
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
  time.frame_lines     = 262;

  time.dram_refreshed   = false;
  time.dram_refresh_pos = 538;

  time.dma_counter = 0;

  apusync.cycles = apusync.cpu_multbl[255];
}

void bCPU::time_init() {
  apusync.cpu_freq = 21477272 >> 3;
  apusync.apu_freq = 24576000 >> 3;

int i;
  for(i=0;i<1024;i++) {
    apusync.cpu_multbl[i] = i * apusync.cpu_freq;
    apusync.apu_multbl[i] = i * apusync.apu_freq;
  }
}
