#include "bus/bus.hpp"

class SuperFX : public MMIO {
public:
  #include "core/core.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"
  #include "disasm/disasm.hpp"

  void enter();

  void init();
  void enable();
  void power();
  void reset();
};

extern SuperFX superfx;
extern SuperFXBus superfxbus;
