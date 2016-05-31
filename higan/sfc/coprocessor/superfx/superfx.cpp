#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
#include "bus/bus.cpp"
#include "core/core.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"

SuperFX superfx;

auto SuperFX::Enter() -> void {
  while(true) scheduler.synchronize(), superfx.main();
}

auto SuperFX::main() -> void {
  if(regs.sfr.g == 0) return step(6);
  instruction(peekpipe());
  if(!r15_modified) regs.r[15]++;
}

auto SuperFX::init() -> void {
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
  create(SuperFX::Enter, system.cpuFrequency());
  memory_reset();
  timing_reset();
}

}
