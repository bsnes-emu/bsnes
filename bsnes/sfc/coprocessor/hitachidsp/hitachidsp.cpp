#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "memory.cpp"
#include "serialization.cpp"
#include "data-rom.cpp"
HitachiDSP hitachidsp;

auto HitachiDSP::synchronizeCPU() -> void {
  if(clock >= 0 && scheduler.mode != Scheduler::Mode::SynchronizeAll) co_switch(cpu.thread);
}

auto HitachiDSP::Enter() -> void {
  while(true) scheduler.synchronize(), hitachidsp.main();
}

auto HitachiDSP::step(uint clocks) -> void {
  HG51B::step(clocks);
  clock += clocks * (uint64_t)cpu.frequency;
  synchronizeCPU();
}

auto HitachiDSP::halt() -> void {
  HG51B::halt();
  if(io.irq == 0) r.i = 1, cpu.r.irq = 1;
}

auto HitachiDSP::unload() -> void {
  rom.reset();
  ram.reset();
}

auto HitachiDSP::power() -> void {
  HG51B::power();
  create(HitachiDSP::Enter, Frequency);
}

}
