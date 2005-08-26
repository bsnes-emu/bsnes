#include "../../base.h"

#include "bcpu_opfn.cpp"
#include "bcpu_op_read.cpp"
#include "bcpu_op_rmw.cpp"
#include "bcpu_op_write.cpp"
#include "bcpu_op_pc.cpp"
#include "bcpu_op_misc.cpp"

#include "bcpu_exec.cpp"
#include "bcpu_mmio.cpp"
#include "bcpu_dma.cpp"

#include "bcpu_timing.cpp"

uint8 bCPU::pio_status() {
  return status.pio;
}

/***********
 *** IRQ ***
 ***********
cycles:
  [1] pbr,pc ; io/opcode
  [2] pbr,pc ; io
  [3] 0,s    ; pbr
  [4] 0,s-1  ; pch
  [5] 0,s-2  ; pcl
  [6] 0,s-3  ; p
  [7] 0,va   ; aavl
  [8] 0,va+1 ; aavh
*/
void bCPU::irq(uint16 addr) {
  if(status.cpu_state == CPUSTATE_WAI) {
    status.cpu_state = CPUSTATE_RUN;
    regs.pc.w++;
  }

//GTE documentation is incorrect, first cycle
//is a memory read fetch from PBR:PC
  add_cycles(mem_bus->speed(regs.pc.d));
  add_cycles(6);
  stack_write(regs.pc.b);
  stack_write(regs.pc.h);
  stack_write(regs.pc.l);
  stack_write(regs.p);
  rd.l = op_read(OPMODE_ADDR, addr);
  rd.h = op_read(OPMODE_ADDR, addr + 1);

  regs.pc.b = 0x00;
  regs.pc.w = rd.w;
  regs.p.i  = 1;
  regs.p.d  = 0;

//let debugger know the new IRQ opcode address
  snes->notify(SNES::CPU_EXEC_OPCODE_END);
}

//vcounter range verified on real hardware,
//HDMA runs on the very first scanline of vblank
bool bCPU::hdma_test() {
  if(status.hdma_triggered == false) {
    if(vcounter() <= (overscan()?239:224) && hcounter() >= 278) {
      status.hdma_triggered = true;
      return true;
    }
  }
  return false;
}

//NMI range: V==225/240,H>=12 ; V>225/240
bool bCPU::nmi_test() {
  if(status.nmi_exec == true)return false;

//[status.cycle_count index]
//  6->12
//  8->14
int hc = status.cycle_count + 6;
  if(vcounter() == ((overscan()?239:224) + 1) && hcycles() < hc) {
  //dprintf("* miss at %3d,%4d,%3x : %d x=%0.4x", vcounter(), hcycles(), hcounter(), status.cycle_count, regs.x.w);
  }

  if(
    (vcounter() == ((overscan()?239:224) + 1) && hcycles() >= hc) ||
    (vcounter()  > ((overscan()?239:224) + 1))
  ) {
  //dprintf("* %3d,%4d,%3x : %d x=%0.4x", vcounter(), hcycles(), hcounter(), status.cycle_count, regs.x.w);
    status.nmi_exec = true;
    return status.nmi_enabled;
  }

  return false;
}

bool bCPU::irq_test() {
int vpos, hpos;
  if(regs.p.i)return false; //no interrupt can occur with I flag set
  if(status.irq_read == true)return false; //same as above
  if(status.virq_enabled == false && status.hirq_enabled == false)return false;

//if irq_exec is true, then an IRQ occurred already.
//IRQs will continue to fire until $4211 is read from, or
//$4200 is written to, where irq_exec is set back to false.
  if(status.irq_exec == true) {
    return true;
  }

//calculate V/H positions required for IRQ to trigger
  vpos = status.virq_pos;
  hpos = (status.hirq_enabled) ? status.hirq_pos : 0;

//positions that can never be latched
//region_scanlines() = 262/NTSC, 312/PAL
//PAL results are unverified on hardware
  if(vpos == 240 && hpos == 339 && interlace() == false && interlace_field() == 1)return false;
  if(vpos == (region_scanlines() - 1) && hpos == 339 && interlace() == false)return false;
  if(vpos == region_scanlines() && interlace() == false)return false;
  if(vpos == region_scanlines() && hpos == 339)return false;
  if(vpos  > region_scanlines())return false;
  if(hpos  > 339)return false;

  if(hpos == 0) {
    hpos = status.cycle_count + 14;
  } else {
    hpos <<= 2;
    hpos += status.cycle_count + 18;
  //it should be OK to use the current line cycles/frame lines,
  //as the IRQ will only trigger on the correct scanline anyway...
    if(hpos >= time.line_cycles) {
      hpos -= time.line_cycles;
      vpos++;
      if(vpos >= time.frame_lines) {
        vpos = 0;
      }
    }
  }

  if(status.virq_enabled == true && vcounter() != vpos)return false;

  if(hcycles() >= hpos) {
    status.irq_exec = true;
    return true;
  }

  return false;
}

void bCPU::run() {
  switch(status.cpu_state) {
  case CPUSTATE_DMA:
    dma_run();
    break;
  case CPUSTATE_RUN:
  case CPUSTATE_WAI:
    if(status.cycle_pos == 0) {
    //interrupts only trigger on opcode edges
      if(nmi_test() == true) {
        irq(0xffea);
        break;
      }
      if(irq_test() == true) {
        irq(0xffee);
        break;
      }
    }
  //fallthrough
  case CPUSTATE_STP:
    exec_cycle();
    break;
  }

  cycle_edge();
}

void bCPU::scanline() {
  status.hdma_triggered = false;

  if(vcounter() == 225 && status.auto_joypad_poll == true) {
    snes->poll_input();
  //When the SNES auto-polls the joypads, it writes 1, then 0 to
  //$4016, then reads from each 16 times to get the joypad state
  //information. As a result, the joypad read positions are set
  //to 16 after such a poll. Position 16 is the controller
  //connected status bit.
    status.joypad1_read_pos = 16;
  }

  if(status.virq_enabled == false) {
    status.irq_read = false;
  }
}

void bCPU::frame() {
  hdma_initialize();

  status.nmi_read = false;
  status.nmi_exec = false;

  status.irq_read = false;
}

void bCPU::power() {
  region = snes->region();

  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;
  reset();
}

void bCPU::reset() {
//reset vector location
  regs.pc  = mem_bus->read(0xfffc) | (mem_bus->read(0xfffd) << 8);

//registers are not fully reset by SNES
  regs.x.h = 0x00;
  regs.y.h = 0x00;
  regs.s.h = 0x01;
  regs.d   = 0x0000;
  regs.db  = 0x00;
  regs.p   = 0x34;
  regs.e   = 1;
  regs.mdr = 0x00;

  time_reset();
  mmio_reset();
  dma_reset();

  status.cpu_state = CPUSTATE_RUN;
  status.dma_state = DMASTATE_STOP;

  status.cycle_pos       = 0;
  status.cycle_count     = 0;
  status.cycles_executed = 0;

  status.hdma_triggered = false;

  status.nmi_read = false;
  status.nmi_exec = false;

  status.irq_read = false;
  status.irq_exec = false;

  apu_port[0] = 0x00;
  apu_port[1] = 0x00;
  apu_port[2] = 0x00;
  apu_port[3] = 0x00;

  frame();
}

uint8 bCPU::port_read(uint8 port) {
  return apu_port[port & 3];
}

void bCPU::port_write(uint8 port, uint8 value) {
  apu_port[port & 3] = value;
}

void bCPU::cpu_c2() {
  if(regs.d.l != 0x00) {
    status.cycle_count = 6;
    add_cycles(6);
  }
}

void bCPU::cpu_c4(uint16 x, uint16 y) {
  if(!regs.p.x && (x & 0xff00) != (y & 0xff00)) {
    status.cycle_count = 6;
    add_cycles(6);
  }
}

void bCPU::cpu_c6(uint16 addr) {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    status.cycle_count = 6;
    add_cycles(6);
  }
}

void bCPU::cpu_io() {
  status.cycle_count = 6;
  add_cycles(6);
}

uint8 bCPU::mem_read(uint32 addr) {
  status.cycle_count = mem_bus->speed(addr);
  add_cycles(2);
  regs.mdr = mem_bus->read(addr);
  add_cycles(status.cycle_count - 2);
  return regs.mdr;
}

void bCPU::mem_write(uint32 addr, uint8 value) {
  status.cycle_count = mem_bus->speed(addr);
  add_cycles(6);
  mem_bus->write(addr, value);
  add_cycles(status.cycle_count - 6);
}

uint32 bCPU::op_addr(uint8 mode, uint32 addr) {
  switch(mode) {
  case OPMODE_ADDR:
    addr &= 0xffff;
    break;
  case OPMODE_LONG:
    addr &= 0xffffff;
    break;
  case OPMODE_DBR:
    addr &= 0xffffff;
    addr = (regs.db << 16) + addr;
    break;
  case OPMODE_PBR:
    addr &= 0xffff;
    addr = (regs.pc.b << 16) | addr;
    break;
  case OPMODE_DP:
    addr &= 0xffff;
    addr = (regs.d + addr) & 0xffff;
    break;
  case OPMODE_SP:
    addr &= 0xffff;
    addr = (regs.s + addr) & 0xffff;
    break;
  }
  return addr;
}

uint8 bCPU::op_read() {
uint8 r;
  r = mem_read(regs.pc.d);
  regs.pc.w++;
  return r;
}

uint8 bCPU::op_read(uint8 mode, uint32 addr) {
  addr = op_addr(mode, addr);
  return mem_read(addr);
}

void bCPU::op_write(uint8 mode, uint32 addr, uint8 value) {
  addr = op_addr(mode, addr);
  mem_write(addr, value);
}

uint8 bCPU::stack_read() {
  if(regs.e) {
    regs.s.l++;
  } else {
    regs.s.w++;
  }
  return mem_read(regs.s);
}

void bCPU::stack_write(uint8 value) {
  mem_write(regs.s, value);
  if(regs.e) {
    regs.s.l--;
  } else {
    regs.s.w--;
  }
}

bCPU::bCPU() {
  mmio = new bCPUMMIO(this);
  init_op_tables();
}

bCPU::~bCPU() {
  delete(mmio);
}
