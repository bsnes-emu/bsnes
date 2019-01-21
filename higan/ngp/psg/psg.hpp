//Texas Instruments SN76489A

struct PSG : Thread {
  shared_pointer<Emulator::Stream> stream;

  //psg.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern PSG psg;
