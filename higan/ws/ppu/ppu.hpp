#include "video.hpp"

struct PPU : Thread {
  static auto Enter() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  uint16 output[224 * 144] = {0};

  struct Status {
    uint vclk;
    uint hclk;
  } status;
};

extern PPU ppu;
