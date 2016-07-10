//Yamaha YM2612

struct YM2612 : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;
  auto reset() -> void;
};

extern YM2612 ym2612;
