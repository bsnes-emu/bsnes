#include <sfc/sfc.hpp>

#define SUPERFX_CPP
namespace SuperFamicom {

#include "serialization.cpp"
#include "bus/bus.cpp"
#include "core/core.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"
#include "disassembler/disassembler.cpp"

SuperFX superfx;

auto SuperFX::Enter() -> void {
  superfx.enter();
}

auto SuperFX::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(regs.sfr.g == 0) {
      step(6);
      synchronize_cpu();
      continue;
    }

    (this->*opcode_table[(regs.sfr & 0x0300) + peekpipe()])();
    if(r15_modified == false) regs.r[15]++;

    if(++instruction_counter >= 128) {
      instruction_counter = 0;
      synchronize_cpu();
    }
  }
}

auto SuperFX::init() -> void {
  initialize_opcode_table();
  regs.r[14].modify = {&SuperFX::r14_modify, this};
  regs.r[15].modify = {&SuperFX::r15_modify, this};
}

auto SuperFX::load() -> void {
}

auto SuperFX::unload() -> void {
  rom.reset();
  ram.reset();
}

auto SuperFX::power() -> void {
  GSU::power();
}

auto SuperFX::reset() -> void {
  GSU::reset();
  create(SuperFX::Enter, system.cpu_frequency());
  instruction_counter = 0;
  memory_reset();
  timing_reset();
}

}
