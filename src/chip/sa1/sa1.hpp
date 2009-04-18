#include "bus/bus.hpp"

class SA1 : public CPUcore, public MMIO {
public:
  #include "dma/dma.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  uint8_t iram[2048];

  struct Status {
    bool interrupt_pending;
    uint16_t interrupt_vector;

    uint16_t scanlines;
    uint16_t vcounter;
    uint16_t hcounter;
  } status;

  void enter();
  void interrupt(uint16_t vector);
  void add_clocks(unsigned);

  void last_cycle();
  bool interrupt_pending();

  void init();
  void enable();
  void power();
  void reset();

  SA1();
};

extern SA1 sa1;
extern SA1Bus sa1bus;
