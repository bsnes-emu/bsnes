Config config_file;

namespace config {

FS::Path FS::base_path(0, "fs.base_path", "Directory that bsnes resides in", "");

FS::Path FS::rom_path(&config_file, "fs.rom_path",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
FS::Path FS::save_path(&config_file, "fs.save_path",
  "Default path for all save RAM and cheat files (\"\" = use current directory)", "");

void FS::Path::sset(const char *_data) {
string path;
  strcpy(path, _data);
  strunquote(path);
  replace(path, "\\", "/");

//blank path?
  if(strlen(path) == 0) {
    Setting::sset(strptr(path));
    return;
  }

//missing final directory marker?
  if(strptr(path)[strlen(path) - 1] != '/') {
    strcat(path, "/");
  }

  Setting::sset(strptr(path));
}

Setting FS::save_ext(&config_file, "fs.save_ext",
  "Extension to be used for all save RAM files", "srm");

SNES::VideoColorAdjust SNES::gamma_ramp(&config_file, "snes.colorfilter.gamma_ramp",
  "Use precalculated TV-style gamma ramp", true, Setting::TRUE_FALSE);
SNES::VideoColorAdjust SNES::sepia(&config_file, "snes.colorfilter.sepia",
  "Convert color to sepia tone", false, Setting::TRUE_FALSE);
SNES::VideoColorAdjust SNES::grayscale(&config_file, "snes.colorfilter.grayscale",
  "Convert color to grayscale tone", false, Setting::TRUE_FALSE);
SNES::VideoColorAdjust SNES::invert(&config_file, "snes.colorfilter.invert",
  "Invert output image colors", false, Setting::TRUE_FALSE);
SNES::VideoColorAdjust SNES::contrast(&config_file, "snes.colorfilter.contrast",
  "", 0,  Setting::DEC);
SNES::VideoColorAdjust SNES::brightness(&config_file, "snes.colorfilter.brightness",
  "", 0, Setting::DEC);
SNES::VideoColorAdjust SNES::gamma(&config_file, "snes.colorfilter.gamma",
  "", 100, Setting::DEC);

void SNES::VideoColorAdjust::set(uint32 _data) {
  Setting::set(_data);
  ::snes->update_color_lookup_table();
}

Setting SNES::ntsc_merge_fields(&config_file, "snes.ntsc_merge_fields",
"Merge fields in NTSC video filter\n"
"Set to true if using filter at any refresh rate other than 60hz\n"
"", false, Setting::TRUE_FALSE);

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
