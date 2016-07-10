//Yamaha YM7101

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;
  auto reset() -> void;
};

extern VDP vdp;
