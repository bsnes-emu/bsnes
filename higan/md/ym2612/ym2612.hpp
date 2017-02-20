//Yamaha YM2612

struct YM2612 : Thread {
  shared_pointer<Emulator::Stream> stream;

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //io.cpp
  auto readStatus() -> uint8;
  auto writeAddress(uint9 data) -> void;
  auto writeData(uint8 data) -> void;
};

extern YM2612 ym2612;
