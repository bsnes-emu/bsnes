Config config_file;

namespace config {

SNES::VideoColorAdjust SNES::video_color_curve(&config_file, "snes.video_color_curve",
  "Applies contrast adjust filter to video output when enabled\n"
  "Works by lowering the brightness of darker colors,\n"
  "while leaving brighter colors alone; thus reducing saturation",
  true, Setting::TRUE_FALSE);

SNES::VideoColorAdjust SNES::video_color_adjust_mode(&config_file, "snes.video_color_adjust_mode",
  "Selects color adjustment filter for video output\n"
  "  0 = Normal (no filter, rgb555)\n"
  "  1 = Grayscale mode (l5)\n"
  "  2 = VGA mode (rgb332)\n"
  "  3 = Genesis mode (rgb333)",
  0, Setting::DEC);

void SNES::VideoColorAdjust::set(uint32 _data) {
  data = _data;
  ::snes->update_color_lookup_table();
}

Setting SNES::mute(&config_file, "snes.mute",
  "Mutes SNES audio output when enabled",
  true, Setting::TRUE_FALSE);

};
