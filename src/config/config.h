namespace config {

extern configuration& config();

string file_updatepath(const char*, const char*);

extern struct File {
  static integral_setting autodetect_type;
  static integral_setting bypass_patch_crc32;
} file;

extern struct Path {
  static string_setting base, rom, patch, save, cheat;
  static string_setting bsx, st;
} path;

extern struct SNES {
  static integral_setting controller_port0;
  static integral_setting controller_port1;
} snes;

extern struct CPU {
  static integral_setting ntsc_clock_rate, pal_clock_rate;
  static integral_setting wram_init_value;
  static integral_setting hdma_enable;
} cpu;

extern struct SMP {
  static integral_setting ntsc_clock_rate, pal_clock_rate;
} smp;

extern struct PPU {
  struct Hack {
    static integral_setting render_scanline_position;
    static integral_setting obj_cache;
    static integral_setting oam_address_invalidation;
    static integral_setting cgram_address_invalidation;
  } hack;

  static integral_setting opt_enable;
  static integral_setting bg1_pri0_enable, bg1_pri1_enable;
  static integral_setting bg2_pri0_enable, bg2_pri1_enable;
  static integral_setting bg3_pri0_enable, bg3_pri1_enable;
  static integral_setting bg4_pri0_enable, bg4_pri1_enable;
  static integral_setting oam_pri0_enable, oam_pri1_enable;
  static integral_setting oam_pri2_enable, oam_pri3_enable;
} ppu;

};
