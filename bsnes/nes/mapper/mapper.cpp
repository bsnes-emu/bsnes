#include <nes/nes.hpp>

namespace NES {
namespace Mapper {

void Mapper::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    cartridge.clock += 12 * 4095;
    tick();
  }
}

void Mapper::tick() {
  cartridge.clock += 12;
  if(cartridge.clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

unsigned Mapper::mirror(unsigned addr, unsigned size) const {
  unsigned base = 0;
  if(size) {
    unsigned mask = 1 << 23;
    while(addr >= size) {
      while(!(addr & mask)) mask >>= 1;
      addr -= mask;
      if(size > mask) {
        size -= mask;
        base += mask;
      }
      mask >>= 1;
    }
    base += addr;
  }
  return base;
}

uint8& Mapper::prg_data(unsigned addr) {
  return cartridge.prg_data[mirror(addr, cartridge.prg_size)];
}

uint8& Mapper::chr_data(unsigned addr) {
  return cartridge.chr_data[mirror(addr, cartridge.chr_size)];
}

unsigned Mapper::ram_size() {
  return 0u;
}

uint8* Mapper::ram_data() {
  return 0;
}

#include "none/none.cpp"
#include "aorom/aorom.cpp"
#include "bandai-fcg/bandai-fcg.cpp"
#include "cnrom/cnrom.cpp"
#include "mmc1/mmc1.cpp"
#include "mmc3/mmc3.cpp"
#include "uorom/uorom.cpp"
#include "vrc6/vrc6.cpp"

}
}
