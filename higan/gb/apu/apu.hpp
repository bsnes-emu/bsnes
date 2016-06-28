struct APU : Thread, MMIO {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;
  auto hipass(int16& sample, int64& bias) -> void;
  auto power() -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

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
