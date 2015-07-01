#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "registers.cpp"
#include "prefetch.cpp"
#include "bus.cpp"
#include "mmio.cpp"
#include "memory.cpp"
#include "dma.cpp"
#include "timer.cpp"
#include "serialization.cpp"
CPU cpu;

auto CPU::Enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::CPU) {
      scheduler.sync = Scheduler::SynchronizeMode::All;
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    cpu.main();
  }
}

auto CPU::main() -> void {
  #if defined(DEBUG)
  if(crash) {
    print(cpsr().t ? disassemble_thumb_instruction(pipeline.execute.address)
                     : disassemble_arm_instruction(pipeline.execute.address), "\n");
    print(disassemble_registers(), "\n");
    print("Executed: ", instructions, "\n");
    while(true) step(frequency);
  }
  #endif

  processor.irqline = regs.ime && (regs.irq.enable & regs.irq.flag);

  if(regs.mode == Registers::Mode::Stop) {
    if((regs.irq.enable.keypad & regs.irq.flag.keypad) == 0) {
      sync_step(16);  //STOP does not advance timers
    } else {
      regs.mode = Registers::Mode::Normal;
    }
    return;
  }

  dma_run();

  if(regs.mode == Registers::Mode::Halt) {
    if((regs.irq.enable & regs.irq.flag) == 0) {
      step(16);
    } else {
      regs.mode = Registers::Mode::Normal;
    }
    return;
  }

  exec();
}

auto CPU::step(unsigned clocks) -> void {
  timer_step(clocks);
  sync_step(clocks);
}

auto CPU::sync_step(unsigned clocks) -> void {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

auto CPU::keypad_run() -> void {
  if(regs.keypad.control.enable == false) return;

  bool test = regs.keypad.control.condition;  //0 = OR, 1 = AND
  for(auto n : range(10)) {
    if(regs.keypad.control.flag[n] == false) continue;
    bool input = interface->inputPoll(0, 0, n);
    if(regs.keypad.control.condition == 0) test |= input;
    if(regs.keypad.control.condition == 1) test &= input;
  }
  if(test) regs.irq.flag.keypad = true;
}

auto CPU::power() -> void {
  create(CPU::Enter, 16'777'216);

  ARM::power();
  for(auto n : range( 32 * 1024)) iwram[n] = 0;
  for(auto n : range(256 * 1024)) ewram[n] = 0;

  for(auto& dma : regs.dma) {
    dma.source = 0;
    dma.target = 0;
    dma.length = 0;
    dma.control = 0;
    dma.pending = 0;
    dma.run.target = 0;
    dma.run.source = 0;
    dma.run.length = 0;
  }
  for(auto& timer : regs.timer) {
    timer.period = 0;
    timer.reload = 0;
    timer.pending = false;
    timer.control = 0;
  }
  regs.keypad.control = 0;
  regs.ime = 0;
  regs.irq.enable = 0;
  regs.irq.flag = 0;
  regs.wait.control = 0;
  regs.postboot = 0;
  regs.mode = Registers::Mode::Normal;
  regs.clock = 0;
  regs.memory.control = 0x0d00'0020;

  pending.dma.vblank = 0;
  pending.dma.hblank = 0;
  pending.dma.hdma = 0;

  active.dma = false;

  for(unsigned n = 0x0b0; n <= 0x0df; n++) bus.mmio[n] = this;  //DMA
  for(unsigned n = 0x100; n <= 0x10f; n++) bus.mmio[n] = this;  //Timers
  for(unsigned n = 0x120; n <= 0x12b; n++) bus.mmio[n] = this;  //Serial
  for(unsigned n = 0x130; n <= 0x133; n++) bus.mmio[n] = this;  //Keypad
  for(unsigned n = 0x134; n <= 0x159; n++) bus.mmio[n] = this;  //Serial
  for(unsigned n = 0x200; n <= 0x209; n++) bus.mmio[n] = this;  //System
  for(unsigned n = 0x300; n <= 0x301; n++) bus.mmio[n] = this;  //System
  //0x080-0x083 mirrored via gba/memory/memory.cpp              //System
}

CPU::CPU() {
  iwram = new uint8[ 32 * 1024];
  ewram = new uint8[256 * 1024];

  regs.dma[0].source.bits(27); regs.dma[0].run.source.bits(27);
  regs.dma[0].target.bits(27); regs.dma[0].run.target.bits(27);
  regs.dma[0].length.bits(14); regs.dma[0].run.length.bits(14);

  regs.dma[1].source.bits(28); regs.dma[1].run.source.bits(28);
  regs.dma[1].target.bits(27); regs.dma[1].run.target.bits(27);
  regs.dma[1].length.bits(14); regs.dma[1].run.length.bits(14);

  regs.dma[2].source.bits(28); regs.dma[2].run.source.bits(28);
  regs.dma[2].target.bits(27); regs.dma[2].run.target.bits(27);
  regs.dma[2].length.bits(14); regs.dma[2].run.length.bits(14);

  regs.dma[3].source.bits(28); regs.dma[3].run.source.bits(28);
  regs.dma[3].target.bits(28); regs.dma[3].run.target.bits(28);
  regs.dma[3].length.bits(16); regs.dma[3].run.length.bits(16);
}

CPU::~CPU() {
  delete[] iwram;
  delete[] ewram;
}

}
