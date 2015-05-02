struct Screen {
  uint32* output;

  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool bg1_color_enable;
    bool bg2_color_enable;
    bool bg3_color_enable;
    bool bg4_color_enable;
    bool oam_color_enable;
    bool back_color_enable;

    uint5 color_b;
    uint5 color_g;
    uint5 color_r;
  } regs;

  struct Math {
    struct Layer {
      uint16 color;
      bool color_enable;
    } main, sub;
    bool transparent;
    bool addsub_mode;
    bool color_halve;
  } math;

  void scanline();
  alwaysinline void run();
  void reset();

  uint16 get_pixel_sub(bool hires);
  uint16 get_pixel_main();
  uint16 addsub(unsigned x, unsigned y);
  alwaysinline uint16 get_color(unsigned palette);
  alwaysinline uint16 get_direct_color(unsigned palette, unsigned tile);
  alwaysinline uint16 fixed_color() const;

  void serialize(serializer&);
  Screen(PPU& self);

  PPU& self;
  friend class PPU;
};
