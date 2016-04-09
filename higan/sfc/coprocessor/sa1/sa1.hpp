struct SA1 : Processor::R65816, Cothread {
  #include "bus/bus.hpp"
  #include "dma/dma.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"

  struct Status {
    uint8 tick_counter;

    bool interrupt_pending;

    uint16 scanlines;
    uint16 vcounter;
    uint16 hcounter;
  } status;

  static auto Enter() -> void;
  auto main() -> void;
  auto tick() -> void;
  auto interrupt() -> void override;

  alwaysinline auto trigger_irq() -> void;
  alwaysinline auto lastCycle() -> void override;
  alwaysinline auto interruptPending() const -> bool override;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  MappedRAM rom;
  MappedRAM iram;
  MappedRAM bwram;
};

extern SA1 sa1;
