class Screen {
  Screen(PPU& self);

  alwaysinline auto get_palette(uint color) -> uint;
  auto get_direct_color(uint palette, uint tile) -> uint;
  alwaysinline auto addsub(uint x, uint y, bool halve) -> uint16;
  auto scanline() -> void;
  auto render_black() -> void;
  alwaysinline auto get_pixel_main(uint x) -> uint16;
  alwaysinline auto get_pixel_sub(uint x) -> uint16;
  auto render() -> void;

  auto serialize(serializer&) -> void;

  PPU& self;

  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool color_enable[7];

    uint color_b;
    uint color_g;
    uint color_r;
    uint color;
  } regs;

  struct Output {
    alwaysinline auto plot_main(uint x, uint color, uint priority, uint source) -> void;
    alwaysinline auto plot_sub(uint x, uint color, uint priority, uint source) -> void;

    struct Pixel {
      uint color;
      uint priority;
      uint source;
    } main[256], sub[256];
  } output;

  ColorWindow window;

  friend class PPU;
};
