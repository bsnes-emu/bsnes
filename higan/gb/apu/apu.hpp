struct APU : Thread, MMIO {
  static auto Main() -> void;
  auto main() -> void;
  auto hipass(int16& sample, int64& bias) -> void;
  auto power() -> void;

  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  #include "square1/square1.hpp"
  #include "square2/square2.hpp"
  #include "wave/wave.hpp"
  #include "noise/noise.hpp"
  #include "sequencer/sequencer.hpp"

  Square1 square1;
  Square2 square2;
  Wave wave;
  Noise noise;
  Sequencer sequencer;

  uint3 phase;   //high 3-bits of clock counter
  uint12 cycle;  //low 12-bits of clock counter
};

extern APU apu;
