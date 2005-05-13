#include "../../base.h"

void bClock::frameskip_update_status() {
  if(frameskip.changed == true) {
    frameskip.changed = false;
    frameskip.count = frameskip.new_count;
    frameskip.pos = 0;
  } else {
    frameskip.pos++;
    frameskip.pos %= (frameskip.count + 1);
  }

  frameskip.notify_ppu = (frameskip.pos == 0)?true:false;
}

void bClock::dram_refresh_test() {
  if(cc1.dram_refreshed == false && status.hcycles >= cc1.dram_refresh_pos) {
    cc1.dram_refreshed = true;

    if(status.interlace != false || status.interlace_field != 1 || status.vcounter != 240) {
     if(cc1.dram_refresh_pos == 534) {
        cc1.dram_refresh_pos = 538;
      } else { //cc1.dram_refresh_pos == 538
        cc1.dram_refresh_pos = 534;
      }
    }

    cc1.pos        += 40;
    cc1.frame_pos  += 40;
    status.hcycles += 40;
  }
}

/*
  all scanlines are 1364 cycles long, except scanline 240
  on non-interlace odd-frames, which is 1360 cycles long.
  interlace mode has 525 scanlines: 263 on the even frame,
  and 262 on the odd.
  non-interlace mode has 524 scanlines: 262 scanlines on
  both even and odd frames.

  cycles per frame:
    263 * 1364     = 358732
    262 * 1364     = 357368
    262 * 1364 - 4 = 357364
*/
void bClock::inc_vcounter() {
  status.hcycles -= cc1.line_cycles;

  status.vcounter++;
  if(status.vcounter >= cc1.frame_lines) {
    frameskip_update_status();

    cc1.frame_pos -= cc1.frame_cycles;
    status.vcounter = 0;
    status.interlace_field ^= 1;
    if(status.interlace == true) {
      if(status.interlace_field == 0) {
        cc1.frame_cycles = 263 * 1364;
        cc1.frame_lines  = 263;
      } else {
        cc1.frame_cycles = 262 * 1364;
        cc1.frame_lines  = 262;
      }
    } else {
      if(status.interlace_field == 0) {
        cc1.frame_cycles = 262 * 1364;
        cc1.frame_lines  = 262;
      } else {
        cc1.frame_cycles = (262 * 1364) - 4;
        cc1.frame_lines  = 262;
      }
    }
    cc1.render_frame = true;
    cpu->frame();
    if(frameskip.notify_ppu == true)ppu->frame();
  }

  if(status.interlace == false && status.interlace_field == 1 && status.vcounter == 240) {
    cc1.line_cycles = 1360;
  } else {
    cc1.line_cycles = 1364;
  }

  cc1.dram_refreshed = false;

  cpu->scanline();
  if(frameskip.notify_ppu == true)ppu->scanline();

//if(status.vcounter == visible_scanlines()) {
//  cc1.render_frame = true;
//}
}

/*
  all dots are 4 cycles long, except dots 322 and 326. dots 322 and 326
  are 6 cycles long. this holds true for all scanlines except scanline
  240 on non-interlace odd frames. the reason for this is because this
  scanline is only 1360 cycles long, instead of 1364 like all other
  scanlines.
  this makes the effective range of hscan_pos 0-339 at all times.
  dot 322 range = { 1288, 1290, 1292 }
  dot 326 range = { 1306, 1308, 1310 }
*/
void bClock::sync() {
uint32 new_cycles, cycles;
  new_cycles = cc1.pos - cc1.last_pos;
  while(new_cycles) {
    if(new_cycles > 40) {
      cycles = 40;
      new_cycles -= 40;
    } else {
      cycles = new_cycles;
      new_cycles = 0;
    }

    status.hcycles += cycles;
    cc1.frame_pos  += cycles;
    dram_refresh_test();
    if(status.hcycles >= cc1.line_cycles) {
      inc_vcounter();
    }
  }
  cc1.last_pos = cc1.pos;

  if(cc1.pos > cc1.frequency) {
    cc1.pos      -= cc1.frequency;
    cc1.last_pos -= cc1.frequency;
  }

  if(status.interlace == false && status.interlace_field == 1 && status.vcounter == 240) {
    status.hcounter = status.hcycles >> 2;
  } else {
  //1288 = 322 * 4, 1306 = 326 * 4 + 2
    status.hcounter = (status.hcycles - ((status.hcycles > 1288) << 1) - ((status.hcycles > 1306) << 1)) >> 2;
  }
}

void bClock::set_frameskip(uint8 fs) {
  frameskip.changed   = true;
  frameskip.new_count = fs;
}

bool bClock::update_frame() {
  if(cc1.render_frame == true) {
    cc1.render_frame = false;
    return true;
  }
  return false;
}

void bClock::power() {
  reset();
}

void bClock::reset() {
  cc1.render_frame = false;

//upon SNES reset, start at scanline 0 non-interlace
  cc1.frame_cycles = 262 * 1364;
  cc1.frame_lines  = 262;
  cc1.line_cycles  = 1364;

/*
  Initial latch values for $213c/$213d
  0035:0000 (53.0 -> 212) [lda $2137]
  0038:0000 (56.5 -> 226) [nop : lda $2137]
*/
  cc1.pos         = 188;
  cc1.last_pos    = 0;
  cc1.frame_pos   = 0;
  status.vcounter = 0;
  status.hcounter = 0;
  status.hcycles  = 0;

  cc1.dram_refresh_pos = 538;
  cc1.dram_refreshed = false;

  cc2.pos = 0;

  sync();
}

void bClock::run() {
  cpu->run();
  if(frameskip.notify_ppu == true)ppu->run();
  sync();
}

void bClock::add_cc1_cycles(uint32 cycles) {
  cc1.pos += cycles;
}

void bClock::add_cc2_cycles(uint32 cycles) {
  cc2.pos += cycles;
}

bClock::bClock() {
  cc1.frequency = 21477272;
  cc2.frequency = 24576000;

  frameskip.changed = false;
  frameskip.count   = 0;
  frameskip.pos     = 0;
}
