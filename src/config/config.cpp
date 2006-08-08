Config config_file;

namespace config {

void fs_set_path(Setting &s, const char *data) {
string path;
  strcpy(path, data);
  strunquote(path);
  replace(path, "\\", "/");

//blank path?
  if(strlen(path) == 0) {
    s.sset(strptr(path));
    return;
  }

//missing final directory marker?
  if(strptr(path)[strlen(path) - 1] != '/') {
    strcat(path, "/");
  }

  s.sset(strptr(path));
}

Setting FS::base_path(0, "fs.base_path",
  "Directory that bsnes resides in", "");
Setting FS::rom_path(&config_file, "fs.rom_path",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
Setting FS::save_path(&config_file, "fs.save_path",
  "Default path for all save RAM and cheat files (\"\" = use current directory)", "");

Setting FS::save_ext(&config_file, "fs.save_ext",
  "Extension to be used for all save RAM files", "srm");

Setting SNES::gamma_ramp(&config_file, "snes.colorfilter.gamma_ramp",
  "Use precalculated TV-style gamma ramp", true, Setting::TRUE_FALSE);
Setting SNES::sepia(&config_file, "snes.colorfilter.sepia",
  "Convert color to sepia tone", false, Setting::TRUE_FALSE);
Setting SNES::grayscale(&config_file, "snes.colorfilter.grayscale",
  "Convert color to grayscale tone", false, Setting::TRUE_FALSE);
Setting SNES::invert(&config_file, "snes.colorfilter.invert",
  "Invert output image colors", false, Setting::TRUE_FALSE);
Setting SNES::contrast(&config_file, "snes.colorfilter.contrast",
  "Contrast", 0,  Setting::DEC);
Setting SNES::brightness(&config_file, "snes.colorfilter.brightness",
  "Brightness", 0, Setting::DEC);
Setting SNES::gamma(&config_file, "snes.colorfilter.gamma",
  "Gamma", 80, Setting::DEC);

Setting SNES::ntsc_merge_fields(&config_file, "snes.ntsc_merge_fields",
"Merge fields in NTSC video filter\n"
"Set to true if using filter at any refresh rate other than 60hz\n"
"", true, Setting::TRUE_FALSE);

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
