//TI SN76489

struct PSG : Thread {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //io.cpp
  auto write(uint8 data) -> void;

private:
  struct Tone {
    //tone.cpp
    auto run() -> void;
    auto power() -> void;

    uint4  volume;
    uint10 counter;
    uint10 pitch;
    uint1  clock;
    uint1  output;
  } tone0, tone1, tone2;

  struct Noise {
    //noise.cpp
    auto run() -> void;
    auto power() -> void;

    uint4  volume;
    uint6  counter;
    uint1  enable;
    uint2  rate;
    uint16 lfsr;
    uint1  clock;
    uint1  output;
  } noise;

  uint3  select;
  int    lowpass;
  uint16 levels[16];
};

extern PSG psg;
