#include "../base.h"
#include "../cpu/g65816.h"
extern g65816 *gx816;
#include "timing.h"

snes_timer::snes_timer() {
  master_cycles = 0;
  vscan_pos     = 0;
  hscan_pos     = 0;
}

#define MEMSPEED_FAST   6
#define MEMSPEED_SLOW   8
#define MEMSPEED_XSLOW 12
ulong snes_timer::get_master_cycle_count(ulong offset) {
byte db;
word addr;
  db   = (offset >> 16) & 0xff;
  addr = (offset) & 0xffff;
  if(db >= 0x00 && db <= 0x3f) {
    if     (addr >= 0x0000 && addr <= 0x1fff)return MEMSPEED_SLOW;
    else if(addr >= 0x2000 && addr <= 0x3fff)return MEMSPEED_FAST;
    else if(addr >= 0x4000 && addr <= 0x41ff)return MEMSPEED_XSLOW;
    else if(addr >= 0x4200 && addr <= 0x5fff)return MEMSPEED_FAST;
    else if(addr >= 0x6000 && addr <= 0xffff)return MEMSPEED_SLOW;
  } else if(db >= 0x40 && db <= 0x7f) {
    return MEMSPEED_SLOW;
  } else if(db >= 0x80 && db <= 0xbf) {
    if     (addr >= 0x0000 && addr <= 0x1fff)return MEMSPEED_SLOW;
    else if(addr >= 0x2000 && addr <= 0x3fff)return MEMSPEED_FAST;
    else if(addr >= 0x4000 && addr <= 0x41ff)return MEMSPEED_XSLOW;
    else if(addr >= 0x4200 && addr <= 0x5fff)return MEMSPEED_FAST;
    else if(addr >= 0x6000 && addr <= 0x7fff)return MEMSPEED_SLOW;
    else if(addr >= 0x8000 && addr <= 0xffff) {
      if(gx816->memory_speed == MEMSPEED_SLOWROM) {
        return MEMSPEED_SLOW;
      } else { //gx816->memory_speed == MEMSPEED_FASTROM
        return MEMSPEED_FAST;
      }
    }
  } else if(db >= 0xc0 && db <= 0xff) {
    if(gx816->memory_speed == MEMSPEED_SLOWROM) {
      return MEMSPEED_SLOW;
    } else { //gx816->memory_speed == MEMSPEED_FASTROM
      return MEMSPEED_FAST;
    }
  }
  return MEMSPEED_FAST; //this should never be hit
}

/*
  vpa = 1, vda = 1 -> add_cpu_pcycles (opcode fetch)
  vpa = 1, vda = 0 -> add_cpu_pcycles (operand fetch)
  vpa = 0, vda = 1 -> add_cpu_dcycles (memory fetch)
  vpa = 0, vda = 0 -> add_cpu_icycles (internal operation)
*/
void snes_timer::add_cpu_pcycles(ulong n) {
ulong speed;
  speed = get_master_cycle_count(gx816->regs.pc);
  master_cycles += n * speed;
}

void snes_timer::add_cpu_scycles(ulong n) {
ulong speed;
  speed = get_master_cycle_count(gx816->regs.s);
  master_cycles += n * speed;
}

void snes_timer::add_cpu_mcycles(ulong n, ulong addr) {
ulong speed;
  speed = get_master_cycle_count(addr);
  master_cycles += n * speed;
}

void snes_timer::add_cpu_icycles(ulong n) {
  master_cycles += n * MEMSPEED_FAST;
}

void snes_timer::add_cpu_icycles(ulong n, ulong flags) {
  if(flags & TIMING_BANKCROSS) {
    if(gx816->index_bank_crossed == true)n++;
  }
//regs.dl != 0x00
  if(flags & TIMING_CONDITION2) {
    if((gx816->regs.d & 0xff) != 0x00)n++;
  }
//add 1 cycle for indexing across page boundaries, or write, or x=0
  if(flags & TIMING_CONDITION4) {
    if(gx816->index_bank_crossed == true || !(gx816->regs.p & PF_X))n++;
  }
  master_cycles += n * MEMSPEED_FAST;
}

void snes_timer::add_cpu_cycles(ulong n) {
  master_cycles += n;
}

void snes_timer::update_timer(void) {
  vscan_pos = (master_cycles / CYCLES_PER_SCANLINE) % 262;
  hscan_pos = (master_cycles % CYCLES_PER_SCANLINE) / 4;
}

snes_timer *snes_time;
