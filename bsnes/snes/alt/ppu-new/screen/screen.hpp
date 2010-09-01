class Screen {
  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool back_color_enable;
    bool oam_color_enable;
    bool bg4_color_enable;
    bool bg3_color_enable;
    bool bg2_color_enable;
    bool bg1_color_enable;

    unsigned color_b;
    unsigned color_g;
    unsigned color_r;
  } regs;

  struct Output {
    struct {
      unsigned color;
      unsigned priority;
    } main[256], sub[256];

    void plot_main(unsigned x, unsigned color, unsigned priority);
    void plot_sub(unsigned x, unsigned color, unsigned priority);
  } output;

  uint16 **light_tables;
  uint16 *light_table;

  unsigned get_palette(unsigned color);

  void scanline();
  void render_black();
  void render();
  Screen(PPU &self);
  ~Screen();

  PPU &self;
  friend class PPU;
};
