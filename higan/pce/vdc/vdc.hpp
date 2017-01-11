//Video Display Controller

struct VDC : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;

private:
  uint32 buffer[512 * 484];
};

extern VDC vdc;
