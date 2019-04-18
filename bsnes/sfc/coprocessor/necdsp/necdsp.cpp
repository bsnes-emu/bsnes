#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
NECDSP necdsp;

auto NECDSP::Enter() -> void {
  while(true) scheduler.synchronize(), necdsp.main();
}

auto NECDSP::main() -> void {
  exec();
  step(1);
  synchronize(cpu);
}

auto NECDSP::read(uint24 addr, uint8) -> uint8 {
  cpu.synchronize(*this);
  if(addr & 1) {
    return uPD96050::readSR();
  } else {
    return uPD96050::readDR();
  }
}

auto NECDSP::write(uint24 addr, uint8 data) -> void {
  cpu.synchronize(*this);
  if(addr & 1) {
    return uPD96050::writeSR(data);
  } else {
    return uPD96050::writeDR(data);
  }
}

auto NECDSP::readRAM(uint24 addr, uint8) -> uint8 {
  cpu.synchronize(*this);
  return uPD96050::readDP(addr);
}

auto NECDSP::writeRAM(uint24 addr, uint8 data) -> void {
  cpu.synchronize(*this);
  return uPD96050::writeDP(addr, data);
}

auto NECDSP::power() -> void {
  uPD96050::power();
  create(NECDSP::Enter, Frequency);
}

}
