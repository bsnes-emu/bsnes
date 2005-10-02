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

#include "bcpu_int.cpp"

uint8 bCPU::pio_status() { return status.pio; }

void bCPU::run() {
  switch(status.cpu_state) {
  case CPUSTATE_DMA:
    dma_run();
    break;
  case CPUSTATE_WAI:
  case CPUSTATE_RUN:
    if(status.cycle_pos == 0) {
    //interrupts only trigger on opcode edges
      if(time.nmi_pending == true) {
        time.nmi_pending = false;
        irq(0xffea);
        break;
      }
      if(time.irq_pending == true) {
        time.irq_pending = false;
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
}

void bCPU::frame() {
  hdma_initialize();

  time.nmi_read = 1;
  time.nmi_line = 1;
  time.nmi_transition = 0;
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
    cpu_io();
  }
}

void bCPU::cpu_c4(uint16 x, uint16 y) {
  if(!regs.p.x && (x & 0xff00) != (y & 0xff00)) {
    cpu_io();
  }
}

void bCPU::cpu_c6(uint16 addr) {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    cpu_io();
  }
}

/* The next 3 functions control bus timing for the CPU.
 * cpu_io is an I/O cycle, and always 6 clock cycles long.
 * mem_read / mem_write indicate memory access bus cycle,
 * they are either 6, 8, or 12 bus cycles long, depending
 * both on location and the $420d.1 FastROM enable bit.
 */

void bCPU::cpu_io() {
  if(status.is_last_cycle)last_cycle_exec();

  status.cycle_count = 6;
  add_cycles(6);
}

uint8 bCPU::mem_read(uint32 addr) {
  if(status.is_last_cycle)last_cycle_exec();

  status.cycle_count = mem_bus->speed(addr);
  add_cycles(status.cycle_count - 4);
  regs.mdr = mem_bus->read(addr);
  add_cycles(4);
  return regs.mdr;
}

void bCPU::mem_write(uint32 addr, uint8 value) {
  if(status.is_last_cycle)last_cycle_exec();

  status.cycle_count = mem_bus->speed(addr);
  add_cycles(status.cycle_count);
  mem_bus->write(addr, value);
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
    addr = ((regs.db << 16) + addr) & 0xffffff;
    break;
  case OPMODE_PBR:
    addr &= 0xffff;
    addr = (regs.pc.b << 16) + addr;
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
