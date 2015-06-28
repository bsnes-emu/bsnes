struct SuperFX : Processor::GSU, Coprocessor {
  MappedRAM rom;
  MappedRAM ram;

  #include "bus/bus.hpp"
  #include "core/core.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"
  #include "disassembler/disassembler.hpp"

  //superfx.cpp
  static auto Enter() -> void;

  auto enter() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern SuperFX superfx;
