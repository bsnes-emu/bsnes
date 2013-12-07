struct Video {
  unsigned* palette;
  void generate_palette(bool color_emulation);
  Video();
  ~Video();

private:
  bool hires;
  unsigned line_width[240];

  void update();
  void scanline();
  void init();

  static const uint8_t gamma_ramp[32];
  static const uint8_t cursor[15 * 15];
  void draw_cursor(uint16_t color, int x, int y);

  friend class System;
};

extern Video video;
