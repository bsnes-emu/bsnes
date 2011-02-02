struct APU : Processor, MMIO {
  #include "mmio/mmio.hpp"
  #include "square/square.hpp"
  #include "wave/wave.hpp"
  #include "noise/noise.hpp"
  #include "master/master.hpp"

  Square square1;
  Square square2;
  Wave wave;
  Noise noise;
  Master master;

  static void Main();
  void main();
  void power();

  void serialize(serializer&);
};

extern APU apu;
