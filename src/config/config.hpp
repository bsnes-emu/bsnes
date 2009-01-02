namespace config {

extern configuration& config();

string filepath(const char *filename, const char *pathname);

extern struct File {
  static integral_setting autodetect_type;
  static integral_setting bypass_patch_crc32;
} file;

extern struct Path {
  static string_setting base, user;
  static string_setting rom, save, patch, cheat, exportdata;
  static string_setting bsx, st;
} path;

extern struct SNES {
  static integral_setting controller_port1;
  static integral_setting controller_port2;
  static integral_setting expansion_port;
  static integral_setting region;
} snes;

extern struct CPU {
  static integral_setting ntsc_clock_rate, pal_clock_rate;
  static integral_setting wram_init_value;
} cpu;

extern struct SMP {
  static integral_setting ntsc_clock_rate, pal_clock_rate;
} smp;

extern struct Temp {
  static integral_setting alu_mul_delay, alu_div_delay;
} temp;

};
