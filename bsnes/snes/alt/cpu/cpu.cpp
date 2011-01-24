#include <snes/snes.hpp>

#define CPU_CPP
namespace SNES {

#if defined(DEBUGGER)
  #include "debugger/debugger.cpp"
  CPUDebugger cpu;
#else
  CPU cpu;
#endif

#include "serialization.cpp"
#include "dma.cpp"
#include "memory.cpp"
#include "mmio.cpp"
#include "timing.cpp"

void CPU::step(unsigned clocks) {
  smp.clock -= clocks * (uint64)smp.frequency;
  ppu.clock -= clocks;
  for(unsigned i = 0; i < coprocessors.size(); i++) {
    Processor &chip = *coprocessors[i];
    chip.clock -= clocks * (uint64)chip.frequency;
  }
}

void CPU::synchronize_smp() {
  if(SMP::Threaded == true) {
    if(smp.clock < 0) co_switch(smp.thread);
  } else {
    while(smp.clock < 0) smp.enter();
  }
}

void CPU::synchronize_ppu() {
  if(PPU::Threaded == true) {
    if(ppu.clock < 0) co_switch(ppu.thread);
  } else {
    while(ppu.clock < 0) ppu.enter();
  }
}

void CPU::synchronize_coprocessor() {
  for(unsigned i = 0; i < coprocessors.size(); i++) {
    Processor &chip = *coprocessors[i];
    if(chip.clock < 0) co_switch(chip.thread);
  }
}

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::CPU) {
      scheduler.sync = Scheduler::SynchronizeMode::All;
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(status.nmi_pending) {
      status.nmi_pending = false;
      op_irq(regs.e == false ? 0xffea : 0xfffa);
    }

    if(status.irq_pending) {
      status.irq_pending = false;
      op_irq(regs.e == false ? 0xffee : 0xfffe);
    }

    op_step();
  }
}

alwaysinline void CPU::op_step() {
  (this->*opcode_table[op_readpc()])();
}

void CPU::op_irq(uint16 vector) {
  op_read(regs.pc.d);
  op_io();
  if(!regs.e) op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.e ? (regs.p & ~0x10) : regs.p);
  rd.l = op_read(vector + 0);
  regs.pc.b = 0x00;
  regs.p.i = 1;
  regs.p.d = 0;
  rd.h = op_read(vector + 1);
  regs.pc.w = rd.w;
}

void CPU::enable() {
  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x2140, 0x2183, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x2140, 0x2183, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x4016, 0x4017, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x4016, 0x4017, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x4200, 0x421f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x4200, 0x421f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x4300, 0x437f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x4300, 0x437f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  bus.map(
    Bus::MapMode::Linear, 0x00, 0x3f, 0x0000, 0x1fff,
    { &StaticRAM::read, &memory::wram }, { &StaticRAM::write, &memory::wram },
    0x000000, 0x002000
  );
  bus.map(
    Bus::MapMode::Linear, 0x80, 0xbf, 0x0000, 0x1fff,
    { &StaticRAM::read, &memory::wram }, { &StaticRAM::write, &memory::wram },
    0x000000, 0x002000
  );
  bus.map(
    Bus::MapMode::Linear, 0x7e, 0x7f, 0x0000, 0xffff,
    { &StaticRAM::read, &memory::wram }, { &StaticRAM::write, &memory::wram }
  );
}

void CPU::power() {
  regs.a = 0x0000;
  regs.x = 0x0000;
  regs.y = 0x0000;
  regs.s = 0x01ff;

  reset();
}

void CPU::reset() {
  create(Enter, system.cpu_frequency());
  coprocessors.reset();
  PPUcounter::reset();

  regs.pc = 0x000000;
  regs.x.h = 0x00;
  regs.y.h = 0x00;
  regs.s.h = 0x01;
  regs.d = 0x0000;
  regs.db = 0x00;
  regs.p = 0x34;
  regs.e = 1;
  regs.mdr = 0x00;
  regs.wai = false;
  update_table();

  regs.pc.l = bus.read(0xfffc);
  regs.pc.h = bus.read(0xfffd);
  regs.pc.b = 0x00;

  status.nmi_valid = false;
  status.nmi_line = false;
  status.nmi_transition = false;
  status.nmi_pending = false;

  status.irq_valid = false;
  status.irq_line = false;
  status.irq_transition = false;
  status.irq_pending = false;

  status.irq_lock = false;
  status.hdma_pending = false;

  status.wram_addr = 0x000000;

  status.joypad_strobe_latch = 0;

  status.nmi_enabled = false;
  status.virq_enabled = false;
  status.hirq_enabled = false;
  status.auto_joypad_poll_enabled = false;

  status.pio = 0xff;

  status.htime = 0x0000;
  status.vtime = 0x0000;

  status.rom_speed = 8;

  status.joy1l = status.joy1h = 0x00;
  status.joy2l = status.joy2h = 0x00;
  status.joy3l = status.joy3h = 0x00;
  status.joy4l = status.joy4h = 0x00;

  dma_reset();
}

CPU::CPU() : queue(512, { &CPU::queue_event, this }) {
  PPUcounter::scanline = { &CPU::scanline, this };
}

CPU::~CPU() {
}

}
