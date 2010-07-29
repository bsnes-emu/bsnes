#include "bus/bus.hpp"

class SuperFX : public Processor, public MMIO {
public:
  //synchronization
  alwaysinline void create();
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

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
  void serialize(serializer&);

private:
  unsigned clockmode;
  unsigned instruction_counter;
};

extern SuperFX superfx;
extern SuperFXBus superfxbus;
