struct APU : Processor, MMIO {
  #include "mmio/mmio.hpp"

  void power();

  void serialize(serializer&);
};

extern APU apu;
