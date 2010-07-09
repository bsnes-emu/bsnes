#include <snes/snes.hpp>

#define SERIAL_CPP
namespace SNES {

Serial serial;

void Serial::enter() {
  scheduler.clock.cop_freq = cartridge.serial_baud_rate() * 2;

  latch = 0;
  add_clocks(512);

  for(unsigned i = 0; i < 6; i++) {
    latch = 1; add_clocks(2);

    latch = 1; add_clocks(2);
    latch = 0; add_clocks(2);
    latch = 0; add_clocks(2);
    latch = 1; add_clocks(2);
    latch = 0; add_clocks(2);
    latch = 1; add_clocks(2);
    latch = 1; add_clocks(2);
    latch = 0; add_clocks(2);

    latch = 0; add_clocks(2);
  }

  while(true) {
    while(cpu.joylatch() == 0) add_clocks(1);
    while(cpu.joylatch() == 1) add_clocks(1);
    add_clocks(1);

    uint8 data = 0;
    for(unsigned i = 0; i < 8; i++) {
      add_clocks(2);
      data = (data << 1) | cpu.joylatch();
    }

    print("Read ", strhex<2>(data), "\n");
  }
}

void Serial::add_clocks(unsigned clocks) {
  scheduler.addclocks_cop(clocks);
  scheduler.sync_copcpu();
}

void Serial::init() {
}

void Serial::enable() {
}

void Serial::power() {
}

void Serial::reset() {
}

}
