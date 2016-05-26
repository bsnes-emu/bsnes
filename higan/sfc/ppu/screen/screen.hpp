struct Screen {
  uint32* lineA;
  uint32* lineB;

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

  Screen(PPU& self);

  auto scanline() -> void;
  alwaysinline auto run() -> void;
  auto reset() -> void;

  auto get_pixel_sub(bool hires) -> uint16;
  auto get_pixel_main() -> uint16;
  auto addsub(uint x, uint y) -> uint16;
  alwaysinline auto get_color(uint palette) -> uint16;
  alwaysinline auto get_direct_color(uint palette, uint tile) -> uint16;
  alwaysinline auto fixed_color() const -> uint16;

  auto serialize(serializer&) -> void;

  PPU& self;
  friend class PPU;
};
