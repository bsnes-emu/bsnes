//Texas Instruments TMS9918A

struct VDP : Thread {
  //vdp.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint32 buffer[256 * 192];
};

extern VDP vdp;
