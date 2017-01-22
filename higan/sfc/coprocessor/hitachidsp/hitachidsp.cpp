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
    for(auto n : range(mmio.dmaLength)) {
      write(mmio.dmaTarget + n, read(mmio.dmaSource + n));
      step(2);
      synchronize(cpu);
    }
    mmio.dma = false;
  }

  exec(mmio.programOffset);
  step(1);
  synchronize(cpu);
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
  HG51B::power();
  create(HitachiDSP::Enter, Frequency);

  mmio.dma = false;

  mmio.dmaSource = 0x000000;
  mmio.dmaLength = 0x0000;
  mmio.dmaTarget = 0x000000;
  mmio.r1f48 = 0x00;
  mmio.programOffset = 0x000000;
  mmio.r1f4c = 0x00;
  mmio.pageNumber = 0x0000;
  mmio.programCounter = 0x00;
  mmio.r1f50 = 0x33;
  mmio.r1f51 = 0x00;
  mmio.r1f52 = 0x01;
}

}
