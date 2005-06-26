#include "../../base.h"

#include "bcpu_op_adc.cpp"
#include "bcpu_op_and.cpp"
#include "bcpu_op_cmp.cpp"
#include "bcpu_op_eor.cpp"
#include "bcpu_op_incdec.cpp"
#include "bcpu_op_lda.cpp"
#include "bcpu_op_misc.cpp"
#include "bcpu_op_ora.cpp"
#include "bcpu_op_pc.cpp"
#include "bcpu_op_sbc.cpp"
#include "bcpu_op_shift.cpp"
#include "bcpu_op_sta.cpp"
#include "bcpu_op_stack.cpp"
#include "bcpu_exec.cpp"
#include "bcpu_mmio.cpp"

uint8 bCPU::pio_status() {
  return status.pio;
}

/***********
 *** IRQ ***
 ***********
cycles:
  [1] pbr,pc ; io
  [2] pbr,pc ; io
  [3] 0,s    ; pbr
  [4] 0,s-1  ; pch
  [5] 0,s-2  ; pcl
  [6] 0,s-3  ; p
  [7] 0,va   ; aavl
  [8] 0,va+1 ; aavh
*/
void bCPU::irq(uint16 addr) {
  if(cpustate == CPUSTATE_WAI) {
    regs.pc.w++;
    cpustate = CPUSTATE_RUN;
  }

  clock->add_cc1_cycles(mem_bus->speed(regs.pc)); //1
  clock->add_cc1_cycles(6);                       //2
  stack_write(regs.pc.b);                         //3
  stack_write(regs.pc.h);                         //4
  stack_write(regs.pc.l);                         //5
  stack_write(regs.p);                            //6
  rd.l = op_read(OPMODE_ADDR, addr);              //7
  rd.h = op_read(OPMODE_ADDR, addr + 1);          //8
  regs.pc.b = 0x00;
  regs.pc.w = rd.w;

  regs.p.d = 0;
  regs.p.i = 1;

  snes->notify(SNES::CPU_EXEC_OPCODE);
}

void bCPU::run() {
uint16 v, h, hc, vs;
  v  = clock->vcounter();
  h  = clock->hcounter();
  hc = clock->hcycles();
  vs = clock->visible_scanlines();

//HDMA test
  if(v < vs && h >= 278) {
    if(status.hdma_triggered == false) {
      status.hdma_triggered = true;
      dma->hdma_run();
      return;
    }
  }

  switch(cpustate) {
  case CPUSTATE_RUN:
  case CPUSTATE_WAI:
  //NMI test
    if(v >= (vs + 1) && status.nmi_triggered == false) {
      if((v == (vs + 1) && hc >= 12) || (v > (vs + 1))) {
        status.nmi_triggered = true;
        status.nmi_pin = 0;
        if(status.nmi_enabled == true) {
          irq(0xffea);
          return;
        }
      }
    }

  //IRQ test
    if(!regs.p.i) {
      if(status.virq_enabled == true && status.hirq_enabled == true) {
        if(v == status.virq_pos && h >= status.hirq_pos && status.irq_pin == 1) {
          status.irq_triggered = true;
          status.irq_pin = 0;
          irq(0xffee);
          return;
        }
      } else if(status.virq_enabled == true) {
        if(v == status.virq_pos && status.irq_pin == 1) {
          status.irq_triggered = true;
          status.irq_pin = 0;
          irq(0xffee);
          return;
        }
      } else if(status.hirq_enabled == true) {
        if(h >= status.hirq_pos && status.irq_pin == 1) {
          status.irq_triggered = true;
          status.irq_pin = 0;
          irq(0xffee);
          return;
        }
      }
    }

    exec_opcode();
    break;
  case CPUSTATE_DMA:
    dma->run();
    break;
  case CPUSTATE_STP:
    exec_opcode();
    break;
  }
}

void bCPU::scanline() {
uint16 v = clock->vcounter();
  status.hdma_triggered = false;

  if(v == 225 && status.auto_joypad_poll == true) {
    snes->poll_input();
//When the SNES auto-polls the joypads, it writes 1, then 0 to
//$4016, then reads from each 16 times to get the joypad state
//information. As a result, the joypad read positions are set
//to 16 after such a poll. Position 16 is the controller
//connected status bit.
    status.joypad1_read_pos = 16;
  }
  if(status.virq_enabled == false) {
    status.irq_pin = 1;
  }
}

void bCPU::frame() {
  status.hdma_triggered = false;

  status.nmi_triggered = false;
  status.r4210_read = false;

  status.irq_triggered = false;
  status.irq_pin = 1;

  dma->hdma_initialize();
}

void bCPU::power() {
  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;
  reset();
}

void bCPU::reset() {
/* reset vector location */
  regs.pc  = mem_bus->read(0xfffc) | (mem_bus->read(0xfffd) << 8);

/* registers are not fully reset by SNES */
  regs.x.h = 0x00;
  regs.y.h = 0x00;
  regs.s.h = 0x01;
  regs.d   = 0x0000;
  regs.db  = 0x00;
  regs.p   = 0x34;
  regs.e   = 1;

  cpustate = CPUSTATE_RUN;

  dma->reset();
  mmio_reset();

  optbl = optbl_e;

  status.hdma_triggered = false;

  status.nmi_triggered = false;
  status.nmi_pin = 1;
  status.r4210_read = false;

  status.irq_triggered = false;
  status.irq_pin = 1;
}

void bCPU::cpu_c2() {
  if(regs.d.l != 0x00) {
    clock->add_cc1_cycles(6);
  }
}

void bCPU::cpu_c4(uint16 a, uint16 b) {
  if(((a & 0xff00) != (b & 0xff00)) || !regs.p.x) {
    clock->add_cc1_cycles(6);
  }
}

void bCPU::cpu_c6(uint16 a) {
  if(regs.e) {
    if((regs.pc.w & 0xff00) != (a & 0xff00)) {
      clock->add_cc1_cycles(6);
    }
  }
}

void bCPU::cpu_io() {
  clock->add_cc1_cycles(6);
}

uint8 bCPU::op_read() {
uint8 r;
  r = mem_bus->read(regs.pc);
  clock->add_cc1_cycles(mem_bus->speed(regs.pc));
  regs.pc.w++;
  return r;
}

uint8 bCPU::op_read(uint8 mode, uint32 addr) {
uint8 r;
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
  r = mem_bus->read(addr);
  clock->add_cc1_cycles(mem_bus->speed(addr));
  return r;
}

void bCPU::op_write(uint8 mode, uint32 addr, uint8 value) {
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
  mem_bus->write(addr, value);
  clock->add_cc1_cycles(mem_bus->speed(addr));
}

uint8 bCPU::stack_read() {
byte r;
  if(regs.e) {
    regs.s.l++;
  } else {
    regs.s.w++;
  }
  r = mem_bus->read(regs.s);
  clock->add_cc1_cycles(mem_bus->speed(regs.s));
  return r;
}

void bCPU::stack_write(uint8 value) {
  mem_bus->write(regs.s, value);
  clock->add_cc1_cycles(mem_bus->speed(regs.s));
  if(regs.e) {
    regs.s.l--;
  } else {
    regs.s.w--;
  }
}

bCPU::bCPU() {
  dma = new bDMA(this);
  mmio = new bCPUMMIO(this);

  init_op_tables();
}

bCPU::~bCPU() {
  delete(mmio);
  delete(dma);
}
