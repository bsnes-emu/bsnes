//TI TMS9918A (derivative)

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;
  auto reset() -> void;

private:
  uint32 buffer[256 * 240];
};

extern VDP vdp;
