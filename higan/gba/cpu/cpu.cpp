#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "prefetch.cpp"
#include "bus.cpp"
#include "io.cpp"
#include "memory.cpp"
#include "dma.cpp"
#include "timer.cpp"
#include "serialization.cpp"
CPU cpu;

CPU::CPU() {
  iwram = new uint8[ 32 * 1024];
  ewram = new uint8[256 * 1024];

  regs.dma[0].source.resize(27); regs.dma[0].run.source.resize(27);
  regs.dma[0].target.resize(27); regs.dma[0].run.target.resize(27);
  regs.dma[0].length.resize(14); regs.dma[0].run.length.resize(14);

  regs.dma[1].source.resize(28); regs.dma[1].run.source.resize(28);
  regs.dma[1].target.resize(27); regs.dma[1].run.target.resize(27);
  regs.dma[1].length.resize(14); regs.dma[1].run.length.resize(14);

  regs.dma[2].source.resize(28); regs.dma[2].run.source.resize(28);
  regs.dma[2].target.resize(27); regs.dma[2].run.target.resize(27);
  regs.dma[2].length.resize(14); regs.dma[2].run.length.resize(14);

  regs.dma[3].source.resize(28); regs.dma[3].run.source.resize(28);
  regs.dma[3].target.resize(28); regs.dma[3].run.target.resize(28);
  regs.dma[3].length.resize(16); regs.dma[3].run.length.resize(16);
}

CPU::~CPU() {
  delete[] iwram;
  delete[] ewram;
}

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
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
    if(!(regs.irq.enable & regs.irq.flag & Interrupt::Keypad)) {
      syncStep(16);  //STOP does not advance timers
    } else {
      regs.mode = Registers::Mode::Normal;
    }
    return;
  }

  dmaRun();

  if(regs.mode == Registers::Mode::Halt) {
    if(!(regs.irq.enable & regs.irq.flag)) {
      step(16);
    } else {
      regs.mode = Registers::Mode::Normal;
    }
    return;
  }

  exec();
}

auto CPU::step(uint clocks) -> void {
  timerStep(clocks);
  syncStep(clocks);
}

auto CPU::syncStep(uint clocks) -> void {
  ppu.clock -= clocks;
  if(ppu.clock < 0) co_switch(ppu.thread);

  apu.clock -= clocks;
  if(apu.clock < 0) co_switch(apu.thread);
}

auto CPU::keypadRun() -> void {
  //lookup table to convert button indexes to Emulator::Interface indexes
  static const uint lookup[] = {5, 4, 8, 9, 3, 2, 0, 1, 7, 6};

  if(!regs.keypad.control.enable) return;

  bool test = regs.keypad.control.condition;  //0 = OR, 1 = AND
  for(auto n : range(10)) {
    if(!regs.keypad.control.flag[n]) continue;
    bool input = interface->inputPoll(0, 0, lookup[n]);
    if(regs.keypad.control.condition == 0) test |= input;
    if(regs.keypad.control.condition == 1) test &= input;
  }
  if(test) regs.irq.flag |= Interrupt::Keypad;
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
    dma.data = 0;
    dma.control.targetmode = 0;
    dma.control.sourcemode = 0;
    dma.control.repeat = 0;
    dma.control.size = 0;
    dma.control.drq = 0;
    dma.control.timingmode = 0;
    dma.control.irq = 0;
    dma.control.enable = 0;
    dma.pending = 0;
    dma.run.target = 0;
    dma.run.source = 0;
    dma.run.length = 0;
  }
  for(auto& timer : regs.timer) {
    timer.period = 0;
    timer.reload = 0;
    timer.pending = false;
    timer.control.frequency = 0;
    timer.control.cascade = 0;
    timer.control.irq = 0;
    timer.control.enable = 0;
  }
  for(auto& flag : regs.keypad.control.flag) flag = 0;
  regs.keypad.control.enable = 0;
  regs.keypad.control.condition = 0;
  regs.ime = 0;
  regs.irq.enable = 0;
  regs.irq.flag = 0;
  for(auto& nwait : regs.wait.control.nwait) nwait = 0;
  for(auto& swait : regs.wait.control.swait) swait = 0;
  regs.wait.control.phi = 0;
  regs.wait.control.prefetch = 0;
  regs.wait.control.gametype = 0;
  regs.postboot = 0;
  regs.mode = Registers::Mode::Normal;
  regs.clock = 0;
  regs.memory.control.disable = 0;
  regs.memory.control.unknown1 = 0;
  regs.memory.control.ewram = 1;
  regs.memory.control.ewramwait = 13;
  regs.memory.control.unknown2 = 0;

  pending.dma.vblank = 0;
  pending.dma.hblank = 0;
  pending.dma.hdma = 0;

  active.dma = false;

  for(uint n = 0x0b0; n <= 0x0df; n++) bus.io[n] = this;  //DMA
  for(uint n = 0x100; n <= 0x10f; n++) bus.io[n] = this;  //Timers
  for(uint n = 0x120; n <= 0x12b; n++) bus.io[n] = this;  //Serial
  for(uint n = 0x130; n <= 0x133; n++) bus.io[n] = this;  //Keypad
  for(uint n = 0x134; n <= 0x159; n++) bus.io[n] = this;  //Serial
  for(uint n = 0x200; n <= 0x209; n++) bus.io[n] = this;  //System
  for(uint n = 0x300; n <= 0x301; n++) bus.io[n] = this;  //System
  //0x080-0x083 mirrored via gba/memory/memory.cpp          //System
}

}
