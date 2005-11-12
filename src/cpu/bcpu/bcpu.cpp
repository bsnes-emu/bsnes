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
  if(run_state.hdma) {
    exec_hdma();
    return;
  }

  if(run_state.dma) {
    exec_dma();
    return;
  }

  if(status.cycle_pos == 0) {
  //interrupts only trigger on opcode edges
    if(!run_state.irq && !run_state.stp) {
      if(time.nmi_pending == true) {
        time.nmi_pending = false;
        aa.w = 0xffea;
        run_state.irq = true;
      } else if(time.irq_pending == true) {
        time.irq_pending = false;
        aa.w = 0xffee;
        run_state.irq = true;
      }
    }
  }

  exec_cycle();
}

void bCPU::scanline() {
  time.hdma_triggered = false;

  if(vcounter() == 225 && status.auto_joypad_poll == true) {
    snes->poll_input(SNES::DEV_JOYPAD1);
    snes->poll_input(SNES::DEV_JOYPAD2);
  //When the SNES auto-polls the joypads, it writes 1, then 0 to
  //$4016, then reads from each 16 times to get the joypad state
  //information. As a result, the joypad read positions are set
  //to 16 after such a poll. Position 16 is the controller
  //connected status bit.
    status.joypad1_read_pos = 16;
    status.joypad2_read_pos = 16;
  }
}

void bCPU::frame() {
  time.nmi_read = 1;
  time.nmi_line = 1;
  time.nmi_transition = 0;

  if(cpu_version == 2) {
    time.hdmainit_trigger_pos = 12 + dma_counter();
  } else {
    time.hdmainit_trigger_pos = 12 + 8 - dma_counter();
  }
  time.hdmainit_triggered = false;
}

void bCPU::power() {
  region = snes->region();

  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;
  reset();
}

void bCPU::reset() {
//reset vector location
  regs.pc  = r_mem->read(0xfffc) | (r_mem->read(0xfffd) << 8);

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

  run_state.hdma = false;
  run_state.dma  = false;
  run_state.irq  = false;
  run_state.wai  = false;
  run_state.stp  = false;

  status.cycle_pos       = 0;
  status.cycle_count     = 0;
  status.cycles_executed = 0;

  apu_port[0] = 0x00;
  apu_port[1] = 0x00;
  apu_port[2] = 0x00;
  apu_port[3] = 0x00;

  frame();

//initial latch values for $213c/$213d
//[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
//[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  add_cycles(186);
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
 * both on location and the $420d.d0 FastROM enable bit.
 */

void bCPU::cpu_io() {
  status.cycle_count = 6;
  pre_exec_cycle();
  add_cycles(6);
  cycle_edge();
}

uint8 bCPU::mem_read(uint32 addr) {
  status.cycle_count = r_mem->speed(addr);
  pre_exec_cycle();
  add_cycles(status.cycle_count - 4);
  regs.mdr = r_mem->read(addr);
  add_cycles(4);
  cycle_edge();
  return regs.mdr;
}

void bCPU::mem_write(uint32 addr, uint8 value) {
  status.cycle_count = r_mem->speed(addr);
  pre_exec_cycle();
  add_cycles(status.cycle_count);
  r_mem->write(addr, value);
  cycle_edge();
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
