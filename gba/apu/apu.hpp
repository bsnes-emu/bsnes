struct APU : Thread, MMIO {
  #include "registers.hpp"

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto read(uint32 addr) -> uint8;
  auto write(uint32 addr, uint8 byte) -> void;
  auto power() -> void;

  auto runsequencer() -> void;

  auto serialize(serializer&) -> void;
};

extern APU apu;
