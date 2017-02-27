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

private:
  struct IO {
    uint9 address;
  } io;

  struct TimerA {
    //timer.cpp
    auto run() -> void;
    auto power() -> void;

    uint1  enable;
    uint1  irq;
    uint1  line;
    uint10 period;
    uint10 counter;
  } timerA;

  struct TimerB {
    //timer.cpp
    auto run() -> void;
    auto power() -> void;

    uint1 enable;
    uint1 irq;
    uint1 line;
    uint8 period;
    uint8 counter;
    uint4 divider;
  } timerB;

  struct DAC {
    uint1 enable;
    uint8 sample;
  } dac;
};

extern YM2612 ym2612;
