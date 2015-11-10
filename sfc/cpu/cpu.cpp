#include <sfc/sfc.hpp>

#define CPU_CPP
namespace SuperFamicom {

CPU cpu;

#include "serialization.cpp"
#include "dma/dma.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"

void CPU::step(unsigned clocks) {
  smp.clock -= clocks * (uint64)smp.frequency;
  ppu.clock -= clocks;
  for(unsigned i = 0; i < coprocessors.size(); i++) {
    auto& chip = *coprocessors[i];
    chip.clock -= clocks * (uint64)chip.frequency;
  }
  device.controllerPort1->clock -= clocks * (uint64)device.controllerPort1->frequency;
  device.controllerPort2->clock -= clocks * (uint64)device.controllerPort2->frequency;
  synchronize_controllers();
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

void CPU::synchronize_coprocessors() {
  for(unsigned i = 0; i < coprocessors.size(); i++) {
    auto& chip = *coprocessors[i];
    if(chip.clock < 0) co_switch(chip.thread);
  }
}

void CPU::synchronize_controllers() {
  if(device.controllerPort1->clock < 0) co_switch(device.controllerPort1->thread);
  if(device.controllerPort2->clock < 0) co_switch(device.controllerPort2->thread);
}

void CPU::Enter() { cpu.enter(); }

void CPU::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::CPU) {
      scheduler.sync = Scheduler::SynchronizeMode::All;
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(status.interrupt_pending) {
      status.interrupt_pending = false;
      if(status.nmi_pending) {
        status.nmi_pending = false;
        regs.vector = (regs.e == false ? 0xffea : 0xfffa);
        op_irq();
        debugger.op_nmi();
      } else if(status.irq_pending) {
        status.irq_pending = false;
        regs.vector = (regs.e == false ? 0xffee : 0xfffe);
        op_irq();
        debugger.op_irq();
      } else if(status.reset_pending) {
        status.reset_pending = false;
        add_clocks(186);
        regs.pc.l = bus.read(0xfffc);
        regs.pc.h = bus.read(0xfffd);
      }
    }

    op_step();
  }
}

void CPU::op_step() {
  debugger.op_exec(regs.pc.d);
  op_exec();
}

void CPU::enable() {
  function<uint8 (unsigned)> reader = {&CPU::mmio_read, (CPU*)&cpu};
  function<void (unsigned, uint8)> writer = {&CPU::mmio_write, (CPU*)&cpu};

  bus.map(reader, writer, 0x00, 0x3f, 0x2140, 0x2183);
  bus.map(reader, writer, 0x80, 0xbf, 0x2140, 0x2183);

  bus.map(reader, writer, 0x00, 0x3f, 0x4016, 0x4017);
  bus.map(reader, writer, 0x80, 0xbf, 0x4016, 0x4017);

  bus.map(reader, writer, 0x00, 0x3f, 0x4200, 0x421f);
  bus.map(reader, writer, 0x80, 0xbf, 0x4200, 0x421f);

  bus.map(reader, writer, 0x00, 0x3f, 0x4300, 0x437f);
  bus.map(reader, writer, 0x80, 0xbf, 0x4300, 0x437f);

  reader = [](unsigned addr) { return cpu.wram[addr]; };
  writer = [](unsigned addr, uint8 data) { cpu.wram[addr] = data; };

  bus.map(reader, writer, 0x00, 0x3f, 0x0000, 0x1fff, 0x002000);
  bus.map(reader, writer, 0x80, 0xbf, 0x0000, 0x1fff, 0x002000);
  bus.map(reader, writer, 0x7e, 0x7f, 0x0000, 0xffff, 0x020000);
}

void CPU::power() {
  for(auto& byte : wram) byte = random(0x55);

  regs.a = regs.x = regs.y = 0x0000;
  regs.s = 0x01ff;

  mmio_power();
  dma_power();
  timing_power();
}

void CPU::reset() {
  create(Enter, system.cpuFrequency());
  coprocessors.reset();
  PPUcounter::reset();

  //note: some registers are not fully reset by SNES
  regs.pc     = 0x000000;
  regs.x.h    = 0x00;
  regs.y.h    = 0x00;
  regs.s.h    = 0x01;
  regs.d      = 0x0000;
  regs.db     = 0x00;
  regs.p      = 0x34;
  regs.e      = 1;
  regs.mdr    = 0x00;
  regs.wai    = false;
  regs.vector = 0xfffc;  //reset vector address

  mmio_reset();
  dma_reset();
  timing_reset();
}

CPU::CPU() {
  PPUcounter::scanline = {&CPU::scanline, this};
}

CPU::~CPU() {
}

}
