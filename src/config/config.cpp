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

Setting SNES::mute(&config_file, "snes.mute", "Mutes SNES audio output when enabled",
  false, Setting::TRUE_FALSE);

//do not save these settings to config_file
Setting CPU::hdma_enable(0, "cpu.hdma_enable", "Enable HDMA effects", true, Setting::TRUE_FALSE);
Setting PPU::opt_enable(0, "ppu.opt_enable", "Enable offset-per-tile effects", true, Setting::TRUE_FALSE);

Setting PPU::bg1_pri0_enable(0, "ppu.bg1_pri0_enable", "Enable BG1 Priority 0", true, Setting::TRUE_FALSE);
Setting PPU::bg1_pri1_enable(0, "ppu.bg1_pri1_enable", "Enable BG1 Priority 1", true, Setting::TRUE_FALSE);
Setting PPU::bg2_pri0_enable(0, "ppu.bg2_pri0_enable", "Enable BG2 Priority 0", true, Setting::TRUE_FALSE);
Setting PPU::bg2_pri1_enable(0, "ppu.bg2_pri1_enable", "Enable BG2 Priority 1", true, Setting::TRUE_FALSE);
Setting PPU::bg3_pri0_enable(0, "ppu.bg3_pri0_enable", "Enable BG3 Priority 0", true, Setting::TRUE_FALSE);
Setting PPU::bg3_pri1_enable(0, "ppu.bg3_pri1_enable", "Enable BG3 Priority 1", true, Setting::TRUE_FALSE);
Setting PPU::bg4_pri0_enable(0, "ppu.bg4_pri0_enable", "Enable BG4 Priority 0", true, Setting::TRUE_FALSE);
Setting PPU::bg4_pri1_enable(0, "ppu.bg4_pri1_enable", "Enable BG4 Priority 1", true, Setting::TRUE_FALSE);
Setting PPU::oam_pri0_enable(0, "ppu.oam_pri0_enable", "Enable OAM Priority 0", true, Setting::TRUE_FALSE);
Setting PPU::oam_pri1_enable(0, "ppu.oam_pri1_enable", "Enable OAM Priority 1", true, Setting::TRUE_FALSE);
Setting PPU::oam_pri2_enable(0, "ppu.oam_pri2_enable", "Enable OAM Priority 2", true, Setting::TRUE_FALSE);
Setting PPU::oam_pri3_enable(0, "ppu.oam_pri3_enable", "Enable OAM Priority 3", true, Setting::TRUE_FALSE);

};
