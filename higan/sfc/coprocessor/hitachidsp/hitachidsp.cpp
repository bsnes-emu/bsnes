#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "memory.cpp"
#include "serialization.cpp"
HitachiDSP hitachidsp;

auto HitachiDSP::Enter() -> void {
  while(true) scheduler.synchronize(), hitachidsp.main();
}

auto HitachiDSP::main() -> void {
  if(mmio.dma) {
    for(auto n : range(mmio.dma_length)) {
      write(mmio.dma_target + n, read(mmio.dma_source + n));
      step(2);
    }
    mmio.dma = false;
  }

  exec(mmio.program_offset);
  step(1);

  synchronizeCPU();
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
