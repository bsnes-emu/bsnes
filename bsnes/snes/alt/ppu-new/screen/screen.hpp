class Screen {
  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool color_enable[6];

    unsigned color_b;
    unsigned color_g;
    unsigned color_r;
    unsigned color;
  } regs;

  struct Output {
    struct Pixel {
      unsigned color;
      unsigned priority;
      unsigned source;
    } main[256], sub[256];

    void plot_main(unsigned x, unsigned color, unsigned priority, unsigned source);
    void plot_sub(unsigned x, unsigned color, unsigned priority, unsigned source);
  } output;

  ColorWindow window;
  uint16 **light_table;

  unsigned get_palette(unsigned color);
  uint16 addsub(unsigned x, unsigned y, bool halve);
  void scanline();
  void render_black();
  uint16 get_pixel_main(unsigned x);
  void render();
  Screen(PPU &self);
  ~Screen();

  PPU &self;
  friend class PPU;
};
