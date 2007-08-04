extern Config config_file;

namespace config {

string file_updatepath(const char *, const char *);

extern struct Path {
  static StringSetting base, rom, save, bios;
  static StringSetting save_ext;
} path;

extern struct SNES {
  static IntegerSetting gamma_ramp, sepia, grayscale, invert, contrast, brightness, gamma;
  static IntegerSetting ntsc_merge_fields;
  static IntegerSetting mute;
  static IntegerSetting controller_port0;
  static IntegerSetting controller_port1;
} snes;

extern struct CPU {
  static IntegerSetting ntsc_clock_rate, pal_clock_rate;
  static IntegerSetting hdma_enable;
} cpu;

extern struct SMP {
  static IntegerSetting ntsc_clock_rate, pal_clock_rate;
} smp;

extern struct PPU {
  struct Hack {
    static IntegerSetting render_scanline_position;
    static IntegerSetting obj_cache;
    static IntegerSetting oam_address_invalidation;
    static IntegerSetting cgram_address_invalidation;
  } hack;

  static IntegerSetting opt_enable;
  static IntegerSetting bg1_pri0_enable, bg1_pri1_enable;
  static IntegerSetting bg2_pri0_enable, bg2_pri1_enable;
  static IntegerSetting bg3_pri0_enable, bg3_pri1_enable;
  static IntegerSetting bg4_pri0_enable, bg4_pri1_enable;
  static IntegerSetting oam_pri0_enable, oam_pri1_enable;
  static IntegerSetting oam_pri2_enable, oam_pri3_enable;
} ppu;

};
