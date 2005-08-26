#include "../../base.h"

#include "bapu_op_fn.cpp"
#include "bapu_op_mov.cpp"
#include "bapu_op_pc.cpp"
#include "bapu_op_read.cpp"
#include "bapu_op_rmw.cpp"
#include "bapu_op_misc.cpp"

#include "bapu_exec.cpp"

uint8 bAPU::spcram_read(uint16 addr) {
uint8 r;
  if(addr >= 0x00f0 && addr <= 0x00ff) {
    switch(addr) {
    case 0xf0: //TEST -- operation unknown, supposedly returns 0x00
      r = 0x00;
      break;
    case 0xf1: //CONTROL -- write-only register, always returns 0x00
      r = 0x00;
      break;
    case 0xf2: //DSPADDR
      r = status.dsp_addr;
      break;
    case 0xf3: //DSPDATA
    //0x80-0xff is a read-only mirror of 0x00-0x7f
      r = dsp_regs[status.dsp_addr & 0x7f];
      break;
    case 0xf4: //CPUIO0
    case 0xf5: //CPUIO1
    case 0xf6: //CPUIO2
    case 0xf7: //CPUIO3
      r = cpu->port_read(addr & 3);
      break;
    case 0xf8: //???
    case 0xf9: //??? -- Mapped to SPCRAM
      r = spcram[addr];
      break;
    case 0xfa: //T0TARGET
    case 0xfb: //T1TARGET
    case 0xfc: //T2TARGET -- write-only registers, always return 0x00
      r = 0x00;
      break;
    case 0xfd: //T0OUT -- 4-bit counter value
      r = t0.stage3_ticks & 15;
      t0.stage3_ticks = 0;
      break;
    case 0xfe: //T1OUT -- 4-bit counter value
      r = t1.stage3_ticks & 15;
      t1.stage3_ticks = 0;
      break;
    case 0xff: //T2OUT -- 4-bit counter value
      r = t2.stage3_ticks & 15;
      t2.stage3_ticks = 0;
      break;
    }
  } else if(addr < 0xffc0) {
    r = spcram[addr];
  } else {
    if(status.iplrom_enabled == true) {
      r = iplrom[addr & 0x3f];
    } else {
      r = spcram[addr];
    }
  }
  snes->notify(SNES::SPCRAM_READ, addr, r);
  return r;
}

void bAPU::spcram_write(uint16 addr, uint8 value) {
  if(addr >= 0x00f0 && addr <= 0x00ff) {
    switch(addr) {
    case 0xf0: //TEST -- operation unknown
      break;
    case 0xf1: //CONTROL
      status.iplrom_enabled = !!(value & 0x80);

    //one-time clearing of APU port read registers,
    //emulated by simulating CPU writes of 0x00
      if(value & 0x20) {
        cpu->port_write(2, 0x00);
        cpu->port_write(3, 0x00);
      }
      if(value & 0x10) {
        cpu->port_write(0, 0x00);
        cpu->port_write(1, 0x00);
      }

    //0->1 transistion resets timers
      if(t2.enabled == false && (value & 0x04)) {
        t2.stage2_ticks = 0;
        t2.stage3_ticks = 0;
      }
      t2.enabled = !!(value & 0x04);

      if(t1.enabled == false && (value & 0x02)) {
        t1.stage2_ticks = 0;
        t1.stage3_ticks = 0;
      }
      t1.enabled = !!(value & 0x02);

      if(t0.enabled == false && (value & 0x01)) {
        t0.stage2_ticks = 0;
        t0.stage3_ticks = 0;
      }
      t0.enabled = !!(value & 0x01);
      break;
    case 0xf2: //DSPADDR
      status.dsp_addr = value;
      break;
    case 0xf3: //DSPDATA
    //0x80-0xff is a read-only mirror of 0x00-0x7f
      if(status.dsp_addr < 0x80) {
        dsp_regs[status.dsp_addr & 0x7f] = value;
      }
      break;
    case 0xf4: //CPUIO0
    case 0xf5: //CPUIO1
    case 0xf6: //CPUIO2
    case 0xf7: //CPUIO3
      port_write(addr & 3, value);
      break;
    case 0xf8: //???
    case 0xf9: //??? - Mapped to SPCRAM
      spcram[addr] = value;
      break;
    case 0xfa: //T0TARGET
      t0.target = value;
      break;
    case 0xfb: //T1TARGET
      t1.target = value;
      break;
    case 0xfc: //T2TARGET
      t2.target = value;
      break;
    case 0xfd: //T0OUT
    case 0xfe: //T1OUT
    case 0xff: //T2OUT -- read-only registers
      break;
    }
  } else {
  //writes to $ffc0-$ffff always go to spcram,
  //even if the iplrom is enabled.
    spcram[addr] = value;
  }
  snes->notify(SNES::SPCRAM_WRITE, addr, value);
}

uint8 bAPU::port_read(uint8 port) {
  return spcram[0xf4 + (port & 3)];
}

void bAPU::port_write(uint8 port, uint8 value) {
  spcram[0xf4 + (port & 0x03)] = value;
}

void bAPU::add_cycles(int cycles) {
  status.cycles_executed += cycles;

  t0.add_cycles(cycles);
  t1.add_cycles(cycles);
  t2.add_cycles(cycles);
}

uint32 bAPU::cycles_executed() {
uint32 r = status.cycles_executed;
  status.cycles_executed = 0;
  return (r << 4) + (r << 3);
}

uint8 bAPU::op_read() {
uint8 r;
  r = spcram_read(regs.pc);
  regs.pc++;
  return r;
}

uint8 bAPU::op_read(uint8 mode, uint16 addr) {
uint8 r;
  switch(mode) {
  case OPMODE_ADDR:
    r = spcram_read(addr);
    break;
  case OPMODE_DP:
    r = spcram_read(((regs.p.p)?0x100:0x000) + (addr & 0xff));
    break;
  }
  return r;
}

void bAPU::op_write(uint8 mode, uint16 addr, uint8 value) {
  switch(mode) {
  case OPMODE_ADDR:
    spcram_write(addr, value);
    break;
  case OPMODE_DP:
    spcram_write(((regs.p.p)?0x100:0x000) + (addr & 0xff), value);
    break;
  }
}

uint8 bAPU::stack_read() {
  regs.sp++;
  return spcram_read(0x0100 | regs.sp);
}

void bAPU::stack_write(uint8 value) {
  spcram_write(0x0100 | regs.sp, value);
  regs.sp--;
}

void bAPU::run() {
  exec_cycle();
}

void bAPU::power() {
  memset(spcram, 0x00, 65536);
  reset();
}

void bAPU::reset() {
  regs.pc = 0xffc0;
  regs.a  = 0x00;
  regs.x  = 0x00;
  regs.y  = 0x00;
  regs.sp = 0xef;
  regs.p  = 0x02;

  status.cycle_pos       = 0;
  status.cycles_executed = 0;

//$f1
  status.iplrom_enabled = true;

  t0.enabled = false;
  t1.enabled = false;
  t2.enabled = false;

  t0.stage1_ticks = 0;
  t1.stage1_ticks = 0;
  t2.stage1_ticks = 0;

  t0.stage2_ticks = 0;
  t1.stage2_ticks = 0;
  t2.stage2_ticks = 0;

  t0.stage3_ticks = 0;
  t1.stage3_ticks = 0;
  t2.stage3_ticks = 0;

  memset(dsp_regs, 0, 128);
}

bAPU::bAPU() {
  init_op_table();

  spcram = (uint8*)malloc(65536);
  memcpy(iplrom, spc700_iplrom, 64);

  t0.cycle_frequency = 128; //1.024mhz /  8khz = 128
  t1.cycle_frequency = 128; //1.024mhz /  8khz = 128
  t2.cycle_frequency =  16; //1.024mhz / 64khz =  16

//targets not initialized/changed upon reset
  t0.target = 0;
  t1.target = 0;
  t2.target = 0;
}

bAPU::~bAPU() {
  if(spcram)free(spcram);
}

//cycles should never be greater than 12. since the minimum
//cycle_frequency value is 16, we don't have to worry about
//two ticks occuring in one call to this function.
void bAPUTimer::add_cycles(int cycles) {
//stage 1 increment
  stage1_ticks += cycles;
  if(stage1_ticks < cycle_frequency)return;

  stage1_ticks -= cycle_frequency;
  if(enabled == false)return;

//stage 2 increment
  stage2_ticks++;

  if(stage2_ticks != target)return;

//stage 3 increment
  stage2_ticks = 0;
  stage3_ticks++;
  stage3_ticks &= 15;
}
