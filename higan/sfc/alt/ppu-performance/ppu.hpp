struct PPU : Thread, public PPUcounter {
  enum : bool { Threaded = true };

  PPU();
  ~PPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeCPU() -> void;

  auto latch_counters() -> void;
  auto interlace() const -> bool;
  auto overscan() const -> bool;
  auto hires() const -> bool;

  auto enter() -> void;
  auto enable() -> void;
  auto power() -> void;
  auto reset() -> void;
  auto scanline() -> void;
  auto frame() -> void;

  auto layer_enable(uint layer, uint priority, bool enable) -> void;
  auto set_frameskip(uint frameskip) -> void;

  auto serialize(serializer&) -> void;

  uint8 vram[64 * 1024];
  uint8 oam[544];
  uint8 cgram[512];

private:
  uint32* surface;
  uint32* output;

  #include "mmio/mmio.hpp"
  #include "window/window.hpp"
  #include "cache/cache.hpp"
  #include "background/background.hpp"
  #include "sprite/sprite.hpp"
  #include "screen/screen.hpp"

  Cache cache;
  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Sprite sprite;
  Screen screen;

  struct Display {
    bool interlace;
    bool overscan;
    uint width;
    uint height;
    uint frameskip;
    uint framecounter;
  } display;

  static auto Enter() -> void;
  auto add_clocks(uint clocks) -> void;
  auto render_scanline() -> void;

  friend class PPU::Cache;
  friend class PPU::Background;
  friend class PPU::Sprite;
  friend class PPU::Screen;
  friend class Video;
};

extern PPU ppu;
