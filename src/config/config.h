extern Config config_file;

namespace config {

extern struct FS {
  static Setting base_path, rom_path, save_path;
  static Setting save_ext;
} fs;

extern struct SNES {
  static Setting gamma_ramp, sepia, grayscale, invert, contrast, brightness, gamma;
  static Setting ntsc_merge_fields;
  static Setting mute;
  static Setting controller_port0;
  static Setting controller_port1;
} snes;

extern struct CPU {
  static Setting ntsc_clock_rate, pal_clock_rate;
  static Setting hdma_enable;
} cpu;

extern struct SMP {
  static Setting ntsc_clock_rate, pal_clock_rate;
} smp;

extern struct PPU {
  static Setting render_scanline_position;
  static Setting opt_enable;

  static Setting bg1_pri0_enable, bg1_pri1_enable;
  static Setting bg2_pri0_enable, bg2_pri1_enable;
  static Setting bg3_pri0_enable, bg3_pri1_enable;
  static Setting bg4_pri0_enable, bg4_pri1_enable;
  static Setting oam_pri0_enable, oam_pri1_enable;
  static Setting oam_pri2_enable, oam_pri3_enable;
} ppu;

};
