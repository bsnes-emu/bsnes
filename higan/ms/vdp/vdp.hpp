//...

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
};

extern VDP vdp;
