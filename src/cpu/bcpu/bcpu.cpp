#include "../../base.h"

#include "core/core.cpp"
#include "memory/memory.cpp"
#include "dma/dma.cpp"
#include "timing/timing.cpp"

#include "bcpu_exec.cpp"
#include "bcpu_mmio.cpp"
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

  if(vcounter() == (overscan() == false ? 227 : 242) && status.auto_joypad_poll == true) {
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
  regs.pc.d = 0;
  regs.pc.l = r_mem->read(0xfffc);
  regs.pc.h = r_mem->read(0xfffd);

//registers are not fully reset by SNES
  regs.x.h = 0x00;
  regs.y.h = 0x00;
  regs.s.h = 0x01;
  regs.d   = 0x0000;
  regs.db  = 0x00;
  regs.p   = 0x34;
  regs.e   = 1;
  regs.mdr = 0x00;

//simulate pbr:pc push during reset irq vector
  regs.s.l -= 3;

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

bCPU::bCPU() {
  init_op_tables();
}

bCPU::~bCPU() {}
