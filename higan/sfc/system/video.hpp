struct Video {
  Video();
  ~Video();

  auto reset() -> void;
  auto refresh() -> void;

  uint32* output = nullptr;
  uint32* paletteStandard = nullptr;
  uint32* paletteEmulation = nullptr;

private:
  bool hires;
  uint line_width[240];

  auto update() -> void;
  auto scanline() -> void;
  auto init() -> void;

  auto drawCursor(uint32 color, int x, int y) -> void;
  auto drawCursors() -> void;

  static const uint8 gamma_ramp[32];
  static const uint8 cursor[15 * 15];

  friend class System;
};

extern Video video;
