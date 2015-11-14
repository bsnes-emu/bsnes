struct Video {
  Video();
  ~Video();
  auto generate_palette(Emulator::Interface::PaletteMode mode) -> void;

  uint32_t* palette = nullptr;

private:
  bool hires;
  uint line_width[240];

  auto update() -> void;
  auto scanline() -> void;
  auto init() -> void;

  static const uint8 gamma_ramp[32];
  static const uint8 cursor[15 * 15];
  auto draw_cursor(uint16 color, int x, int y) -> void;

  friend class System;
};

extern Video video;
