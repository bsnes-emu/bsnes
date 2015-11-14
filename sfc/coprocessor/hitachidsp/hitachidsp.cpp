#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "memory.cpp"
#include "serialization.cpp"
HitachiDSP hitachidsp;

auto HitachiDSP::Enter() -> void { hitachidsp.enter(); }

auto HitachiDSP::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(mmio.dma) {
      for(auto n : range(mmio.dma_length)) {
        bus.write(mmio.dma_target + n, bus.read(mmio.dma_source + n));
        step(2);
      }
      mmio.dma = false;
    }

    exec(mmio.program_offset);
    step(1);

    synchronize_cpu();
  }
}

auto HitachiDSP::init() -> void {
}

auto HitachiDSP::load() -> void {
}

auto HitachiDSP::unload() -> void {
  rom.reset();
  ram.reset();
}

auto HitachiDSP::power() -> void {
  mmio.dma = false;

  mmio.dma_source = 0x000000;
  mmio.dma_length = 0x0000;
  mmio.dma_target = 0x000000;
  mmio.r1f48 = 0x00;
  mmio.program_offset = 0x000000;
  mmio.r1f4c = 0x00;
  mmio.page_number = 0x0000;
  mmio.program_counter = 0x00;
  mmio.r1f50 = 0x33;
  mmio.r1f51 = 0x00;
  mmio.r1f52 = 0x01;
}

auto HitachiDSP::reset() -> void {
  create(HitachiDSP::Enter, Frequency);
  HG51B::power();
}

}
