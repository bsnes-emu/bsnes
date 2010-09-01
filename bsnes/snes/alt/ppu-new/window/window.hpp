class Window {
  struct Regs {
    bool bg1_one_enable;
    bool bg1_one_invert;
    bool bg1_two_enable;
    bool bg1_two_invert;

    bool bg2_one_enable;
    bool bg2_one_invert;
    bool bg2_two_enable;
    bool bg2_two_invert;

    bool bg3_one_enable;
    bool bg3_one_invert;
    bool bg3_two_enable;
    bool bg3_two_invert;

    bool bg4_one_enable;
    bool bg4_one_invert;
    bool bg4_two_enable;
    bool bg4_two_invert;

    bool oam_one_enable;
    bool oam_one_invert;
    bool oam_two_enable;
    bool oam_two_invert;

    bool col_one_enable;
    bool col_one_invert;
    bool col_two_enable;
    bool col_two_invert;

    unsigned one_left;
    unsigned one_right;

    unsigned two_left;
    unsigned two_right;

    unsigned bg1_mask;
    unsigned bg2_mask;
    unsigned bg3_mask;
    unsigned bg4_mask;
    unsigned oam_mask;
    unsigned col_mask;

    bool bg1_main_enable;
    bool bg2_main_enable;
    bool bg3_main_enable;
    bool bg4_main_enable;
    bool oam_main_enable;

    bool bg1_sub_enable;
    bool bg2_sub_enable;
    bool bg3_sub_enable;
    bool bg4_sub_enable;
    bool oam_sub_enable;

    unsigned col_main_mask;
    unsigned col_sub_mask;
  } regs;

  Window(PPU &self);

  PPU &self;
  friend class PPU;
};
