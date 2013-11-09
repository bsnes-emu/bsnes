struct SuperFX : Processor::GSU, Coprocessor {
  MappedRAM rom;
  MappedRAM ram;

  #include "bus/bus.hpp"
  #include "core/core.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"
  #include "disasm/disasm.hpp"

  static void Enter();
  void enter();
  void init();
  void load();
  void unload();
  void power();
  void reset();
  void serialize(serializer&);

privileged:
  unsigned clockmode = 0;  //0 = selectable, 1 = force 10.74mhz, 2 = force 21.48mhz
  unsigned instruction_counter;
};

extern SuperFX superfx;
