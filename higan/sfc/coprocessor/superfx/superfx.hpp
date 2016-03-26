struct SuperFX : Processor::GSU, Coprocessor {
  MappedRAM rom;
  MappedRAM ram;

  #include "bus/bus.hpp"
  #include "core/core.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"

  //superfx.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern SuperFX superfx;
