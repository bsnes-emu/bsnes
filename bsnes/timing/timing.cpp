#include "../base.h"
#include "../cpu/g65816.h"
#include "timing.h"

extern g65816     *gx816;
extern ppustate   ppu;
extern debugstate debugger;
snes_timer        *snes_time;

/*
  apu_cycles is incremented to keep in sync with master_cycles.
  (n << 5) is only a guess that one apu cycle is equivalent to
  32 cpu cycles.
*/
void add_apu_cycles(int n) {
  snes_time->apu_cycles += n;
  snes_time->bridge.apu_cycles += (n << 5);
}

void snes_timer::add_cpu_cycles(byte count, byte speed) {
ulong cycles = count * speed;
  master_cycles     += cycles;
  bridge.cpu_cycles += cycles;
}

void snes_timer::add_cpu_pcycles(byte count) {
ulong speed = mem_speed_map[(gx816->regs.pc & 0xffffff) >> 9];
ulong cycles = count * speed;
  master_cycles     += cycles;
  bridge.cpu_cycles += cycles;
}

void snes_timer::add_cpu_mcycles(byte count, ulong addr) {
ulong speed = mem_speed_map[(addr & 0xffffff) >> 9];
ulong cycles = count * speed;
  master_cycles     += cycles;
  bridge.cpu_cycles += cycles;
}

void snes_timer::add_cpu_scycles(byte count) {
ulong cycles = (count << 3); //count * 8
  master_cycles     += cycles;
  bridge.cpu_cycles += cycles;
}

void snes_timer::add_cpu_icycles(byte count) {
ulong cycles = (count << 2) + (count << 1); //count * 6
  master_cycles     += cycles;
  bridge.cpu_cycles += cycles;
}

void snes_timer::set_speed_map(byte speed) {
  if(speed == MEMSPEED_SLOWROM) {
    mem_speed_map = sm_slowrom;
  } else { //speed == MEMSPEED_FASTROM
    mem_speed_map = sm_fastrom;
  }
}

void snes_timer::build_speed_map(void) {
int   i;
byte  db;
word  addr;
ulong z;
  sm_slowrom = (byte*)malloc(0x8000);
  sm_fastrom = (byte*)malloc(0x8000);
  for(i=0;i<0x8000;i++) {
    z    = (i << 9);
    db   = (z >> 16);
    addr = (z & 0xffff);
    if(db >= 0x00 && db <= 0x3f) {
      if     (addr >= 0x0000 && addr <= 0x1fff) {
        sm_slowrom[i] = MEMSPEED_SLOW;
        sm_fastrom[i] = MEMSPEED_SLOW;
      }
      else if(addr >= 0x2000 && addr <= 0x3fff) {
        sm_slowrom[i] = MEMSPEED_FAST;
        sm_fastrom[i] = MEMSPEED_FAST;
      }
      else if(addr >= 0x4000 && addr <= 0x41ff) {
        sm_slowrom[i] = MEMSPEED_XSLOW;
        sm_fastrom[i] = MEMSPEED_XSLOW;
      }
      else if(addr >= 0x4200 && addr <= 0x5fff) {
        sm_slowrom[i] = MEMSPEED_FAST;
        sm_fastrom[i] = MEMSPEED_FAST;
      }
      else { //(addr >= 0x6000 && addr <= 0xffff)
        sm_slowrom[i] = MEMSPEED_SLOW;
        sm_fastrom[i] = MEMSPEED_SLOW;
      }
    } else if(db >= 0x40 && db <= 0x7f) {
      sm_slowrom[i] = MEMSPEED_SLOW;
      sm_fastrom[i] = MEMSPEED_SLOW;
    } else if(db >= 0x80 && db <= 0xbf) {
      if     (addr >= 0x0000 && addr <= 0x1fff) {
        sm_slowrom[i] = MEMSPEED_SLOW;
        sm_fastrom[i] = MEMSPEED_SLOW;
      }
      else if(addr >= 0x2000 && addr <= 0x3fff) {
        sm_slowrom[i] = MEMSPEED_FAST;
        sm_fastrom[i] = MEMSPEED_FAST;
      }
      else if(addr >= 0x4000 && addr <= 0x41ff) {
        sm_slowrom[i] = MEMSPEED_XSLOW;
        sm_fastrom[i] = MEMSPEED_XSLOW;
      }
      else if(addr >= 0x4200 && addr <= 0x5fff) {
        sm_slowrom[i] = MEMSPEED_FAST;
        sm_fastrom[i] = MEMSPEED_FAST;
      }
      else if(addr >= 0x6000 && addr <= 0x7fff) {
        sm_slowrom[i] = MEMSPEED_SLOW;
        sm_fastrom[i] = MEMSPEED_SLOW;
      }
      else { //(addr >= 0x8000 && addr <= 0xffff)
        sm_slowrom[i] = MEMSPEED_SLOW;
        sm_fastrom[i] = MEMSPEED_FAST;
      }
    } else { //(db >= 0xc0 && db <= 0xff)
      sm_slowrom[i] = MEMSPEED_SLOW;
      sm_fastrom[i] = MEMSPEED_FAST;
    }
  }
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

  latch_table_a is used for interlace-even, interlace-odd, and
  non-interlace-even frames. this is done to conserve memory.
  interlace-even frames have one extra scanline, but the
  cycle-to-x/y positions are still identical.
  latch_table_b is used for non-interlace-odd, because these
  frames have one scanline that is 4 master cycles shorter than
  the others. this would offset all x/y positions from (322,240)
  and onward without a separate table.

  both tables are one scanline longer than the snes allows so that
  if the x/y positions are refreshed and the cycle position used
  goes beyond the end of the frame, it won't access an invalid
  memory address. the routine will then see that the y position
  is higher than the current frame allows, and it will reset the
  y position at that time.
*/
void snes_timer::build_dot_map(void) {
int x, y, cycle;
int ptr;
  latch_table_a = (latch_pos*)malloc(264 * (1364 / 2) * sizeof(latch_pos));
  latch_table_b = (latch_pos*)malloc(263 * (1364 / 2) * sizeof(latch_pos));

  ptr = 0;
  for(y=0;y<264;y++) {
    for(x=cycle=0;x<340;x++) {
      latch_table_a[ptr].cx = cycle;
      latch_table_a[ptr].x  = x;
      latch_table_a[ptr].y  = y;
      ptr++;
      cycle += 2;
      latch_table_a[ptr].cx = cycle;
      latch_table_a[ptr].x  = x;
      latch_table_a[ptr].y  = y;
      ptr++;
      cycle += 2;
      if(x == 322 || x == 326) {
        latch_table_a[ptr].cx = cycle;
        latch_table_a[ptr].x  = x;
        latch_table_a[ptr].y  = y;
        ptr++;
        cycle += 2;
      }
    }
  }

  ptr = 0;
  for(y=0;y<263;y++) {
    for(x=cycle=0;x<340;x++) {
      latch_table_b[ptr].cx = cycle;
      latch_table_b[ptr].x  = x;
      latch_table_b[ptr].y  = y;
      ptr++;
      cycle += 2;
      latch_table_b[ptr].cx = cycle;
      latch_table_b[ptr].x  = x;
      latch_table_b[ptr].y  = y;
      ptr++;
      cycle += 2;
      if(y != 240 && (x == 322 || x == 326)) {
        latch_table_b[ptr].cx = cycle;
        latch_table_b[ptr].x  = x;
        latch_table_b[ptr].y  = y;
        ptr++;
        cycle += 2;
      }
    }
  }
}

void snes_timer::reset_clock(void) {
  mem_speed_map      = sm_slowrom;

//upon SNES reset, starts at scanline 0 non-interlace
  frame_cycles         = 262 * 1364;
  frame_lines          = 262;
  line_cycles          = 1364;
  ppu.interlace        = false;
  ppu.toggle_interlace = false;
  ppu.interlace_frame  = 0;
  latch_table          = latch_table_a;

  vscan_pos  = 0;
  hscan_pos  = 0;
  hcycle_pos = 0;

  dram_refresh_pos = 538;
  dram_refreshed   = false;

  nmi_triggered = false;

  apu_cycles        = 0;
  bridge.cpu_cycles = 0;
  bridge.apu_cycles = 0;

/*
  Initial latch values for $213c/$213d
  0035:0000 (53.0 -> 212) [lda $2137]
  0038:0000 (56.5 -> 226) [nop : lda $2137]
*/
  master_cycles      = 188;
  prev_master_cycles = 0;
  update_timer();
}

snes_timer::snes_timer() {
  build_speed_map();
  build_dot_map();
}

snes_timer::~snes_timer() {
  if(mem_speed_map)free(mem_speed_map);
  if(latch_table_a)free(latch_table_a);
  if(latch_table_b)free(latch_table_b);
}

/*
  all scanlines are 1364 cycles long, except scanline 240
  on non-interlace odd-frames, which is 1360 cycles long.
  interlace mode has 525 scanlines: 263 on the even frame,
  and 262 on the odd.
  non-interlace mode has 524 scanlines: 262 scanlines on
  both even and odd frames.
*/
void snes_timer::inc_vscan_pos(void) {
  if(vscan_pos >= frame_lines) {
    master_cycles -= frame_cycles;
    vscan_pos = 0;
    nmi_triggered = false;

    ppu.interlace = ppu.toggle_interlace;
    ppu.interlace_frame ^= 1;
    if(ppu.interlace == true) {
      if(ppu.interlace_frame == 0) {
        frame_cycles = 263 * 1364;
        frame_lines = 263;
      } else {
        frame_cycles = 262 * 1364;
        frame_lines = 262;
      }
      latch_table = latch_table_a;
    } else { //ppu.interlace == false
      if(ppu.interlace_frame == 0) {
        frame_cycles = 262 * 1364;
        frame_lines = 262;
        latch_table = latch_table_a;
      } else {
        frame_cycles = (262 * 1364) - 4;
        frame_lines = 262;
        latch_table = latch_table_b;
      }
    }
    vscan_wrapped = true;
  }

  if(ppu.interlace == false && ppu.interlace_frame == 1 && vscan_pos == 240) {
    line_cycles = 1360;
  } else {
    line_cycles = 1364;
  }

  hscan_wrapped  = true;
  dram_refreshed = false;
}

void snes_timer::update_dram_refresh_pos(void) {
  if(ppu.interlace == false && ppu.interlace_frame == 1 && vscan_pos == 240) {
    //dram refresh position doesn't change
  } else {
    if(dram_refresh_pos == 534) {
      dram_refresh_pos = 538;
    } else {
      dram_refresh_pos = 534;
    }
  }
}

/*
  this routine is *not* designed to seek to any scan position passed to it.
  it is intended that cycle_pos is no more than 40 master cycles above the
  previous cycle_pos given to this routine. the routine has two purposes,
  one is to actually update the screen x/y positions and trigger system
  events; the other is to allow the 4 master cycle difference between latching
  the x/y counters from reading $2137 and writing $4201.
  the former passes update_frame_info as true, as the changes should be permanent.
  the latter passes update_frame_info as false, as the real cycle counter will
  not be incremented at that time (that is handled within the opcodes themselves).

  the cycle_pos passed is divided by 2 because the smallest possible cpu clock
  frequency is 2 master cycles, and this allows the latch tables to consume only
  half as much memory.
*/
void snes_timer::set_scan_pos(ulong cycle_pos, bool update_frame_info) {
int y;
  y = vscan_pos;
  cycle_pos >>= 1;
  vscan_pos  = latch_table[cycle_pos].y;
  hscan_pos  = latch_table[cycle_pos].x;
  hcycle_pos = latch_table[cycle_pos].cx;
  if(update_frame_info == true) {
    if(vscan_pos > y) {
      inc_vscan_pos();
    }
  } else {
    if(vscan_pos >= frame_lines) {
      vscan_pos -= frame_lines;
    }
  }

  if(dram_refreshed == false && hcycle_pos >= dram_refresh_pos) {
    if(update_frame_info == true) {
      add_cpu_cycles(1, 40);
      dram_refreshed = true;
      update_dram_refresh_pos();
    }
    hcycle_pos += 40;
    hscan_pos  += 10;
  }
}

void snes_timer::update_timer(void) {
  set_scan_pos(master_cycles, true);
}

void snes_timer::update_timer_events(void) {
  if(gx816->cpu_state == CPUSTATE_STP)return;

  if(snes_time->vscan_pos == (ppu.visible_scanlines + 1) && snes_time->hcycle_pos >= 12 && nmi_triggered == false) {
    nmi_triggered  = true;
    gx816->nmi_pin = 0;
    if(gx816->nmi_enabled == true) {
      gx816->InvokeIRQ(0xffea);
    }
  }
}
