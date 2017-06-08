struct APU : Thread, IO {
  shared_pointer<Emulator::Stream> stream;

  #include "registers.hpp"

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto readIO(uint32 addr) -> uint8;
  auto writeIO(uint32 addr, uint8 byte) -> void;
  auto power() -> void;

  auto runsequencer() -> void;

  auto serialize(serializer&) -> void;

  uint clock;
};

extern APU apu;
