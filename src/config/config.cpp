Config config_file;

namespace config {

string file_updatepath(const char *req_file, const char *req_path) {
string file(req_file);
  replace(file, "\\", "/");
  if(!req_path || strlen(req_path) == 0) { return file; }

string path(req_path);
  replace(path, "\\", "/");
  if(!strend(path, "/")) { strcat(path, "/"); }

  if(strbegin(path, "./")) {
    strltrim(path, "./");
  string temp;
    strcpy(temp, config::path.base);
    strcat(temp, path);
    strcpy(path, temp);
  }

stringarray part;
  split(part, "/", file);
  strcat(path, part[count(part) - 1]);
  return path;
}

StringSetting Path::base(0, "fs.base_path",
  "Path that bsnes resides in", "");
StringSetting Path::rom(&config_file, "path.rom",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
StringSetting Path::save(&config_file, "path.save",
  "Default path for all save RAM and cheat files (\"\" = use current directory)", "");
StringSetting Path::bios(&config_file, "path.bios",
  "Path where BIOS file(s) are located\n"
  "Supported BIOS files:\n"
  "stbios.bin - Bandai Sufami Turbo"
  "", "./bios");

StringSetting Path::save_ext(&config_file, "path.save_ext",
  "Extension to be used for all save RAM files", "srm");

IntegerSetting SNES::gamma_ramp(&config_file, "snes.colorfilter.gamma_ramp",
  "Use precalculated TV-style gamma ramp", IntegerSetting::Boolean, true);
IntegerSetting SNES::sepia(&config_file, "snes.colorfilter.sepia",
  "Convert color to sepia tone", IntegerSetting::Boolean, false);
IntegerSetting SNES::grayscale(&config_file, "snes.colorfilter.grayscale",
  "Convert color to grayscale tone", IntegerSetting::Boolean, false);
IntegerSetting SNES::invert(&config_file, "snes.colorfilter.invert",
  "Invert output image colors", IntegerSetting::Boolean, false);
IntegerSetting SNES::contrast(&config_file, "snes.colorfilter.contrast",
  "Contrast", IntegerSetting::Decimal, 0);
IntegerSetting SNES::brightness(&config_file, "snes.colorfilter.brightness",
  "Brightness", IntegerSetting::Decimal, 0);
IntegerSetting SNES::gamma(&config_file, "snes.colorfilter.gamma",
  "Gamma", IntegerSetting::Decimal, 80);

IntegerSetting SNES::ntsc_merge_fields(&config_file, "snes.ntsc_merge_fields",
  "Merge fields in NTSC video filter\n"
  "Set to true if using filter at any refresh rate other than 60hz\n"
  "", IntegerSetting::Boolean, true);

IntegerSetting SNES::mute(&config_file, "snes.mute", "Mutes SNES audio output when enabled",
  IntegerSetting::Boolean, false);

IntegerSetting SNES::controller_port0(&config_file, "snes.controller_port_1",
  "Controller attached to SNES port 1", IntegerSetting::Decimal, ::SNES::DEVICEID_JOYPAD1);
IntegerSetting SNES::controller_port1(&config_file, "snes.controller_port_2",
  "Controller attached to SNES port 2", IntegerSetting::Decimal, ::SNES::DEVICEID_JOYPAD2);

IntegerSetting CPU::ntsc_clock_rate(&config_file, "cpu.ntsc_clock_rate",
  "NTSC S-CPU clock rate (in hz)", IntegerSetting::Decimal, 21477272);
IntegerSetting CPU::pal_clock_rate(&config_file, "cpu.pal_clock_rate",
  "PAL S-CPU clock rate (in hz)", IntegerSetting::Decimal, 21281370);

IntegerSetting CPU::hdma_enable(0, "cpu.hdma_enable",
  "Enable HDMA effects", IntegerSetting::Boolean, true);

IntegerSetting SMP::ntsc_clock_rate(&config_file, "smp.ntsc_clock_rate",
  "NTSC S-SMP clock rate (in hz)", IntegerSetting::Decimal, 24606720);
IntegerSetting SMP::pal_clock_rate(&config_file, "smp.pal_clock_rate",
  "PAL S-SMP clock rate (in hz)", IntegerSetting::Decimal, 24606720);

IntegerSetting PPU::Hack::render_scanline_position(&config_file, "ppu.hack.render_scanline_position",
  "Approximate HCLOCK position to render at for scanline-based renderers",
  IntegerSetting::Decimal, 512);
IntegerSetting PPU::Hack::obj_cache(&config_file, "ppu.hack.obj_cache",
  "Cache OAM OBJ attributes one scanline before rendering\n"
  "This is technically closer to the actual operation of the SNES,\n"
  "but can cause problems in some games if enabled",
  IntegerSetting::Boolean, false);

IntegerSetting PPU::opt_enable(0, "ppu.opt_enable", "Enable offset-per-tile effects", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg1_pri0_enable(0, "ppu.bg1_pri0_enable", "Enable BG1 Priority 0", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg1_pri1_enable(0, "ppu.bg1_pri1_enable", "Enable BG1 Priority 1", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg2_pri0_enable(0, "ppu.bg2_pri0_enable", "Enable BG2 Priority 0", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg2_pri1_enable(0, "ppu.bg2_pri1_enable", "Enable BG2 Priority 1", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg3_pri0_enable(0, "ppu.bg3_pri0_enable", "Enable BG3 Priority 0", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg3_pri1_enable(0, "ppu.bg3_pri1_enable", "Enable BG3 Priority 1", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg4_pri0_enable(0, "ppu.bg4_pri0_enable", "Enable BG4 Priority 0", IntegerSetting::Boolean, true);
IntegerSetting PPU::bg4_pri1_enable(0, "ppu.bg4_pri1_enable", "Enable BG4 Priority 1", IntegerSetting::Boolean, true);
IntegerSetting PPU::oam_pri0_enable(0, "ppu.oam_pri0_enable", "Enable OAM Priority 0", IntegerSetting::Boolean, true);
IntegerSetting PPU::oam_pri1_enable(0, "ppu.oam_pri1_enable", "Enable OAM Priority 1", IntegerSetting::Boolean, true);
IntegerSetting PPU::oam_pri2_enable(0, "ppu.oam_pri2_enable", "Enable OAM Priority 2", IntegerSetting::Boolean, true);
IntegerSetting PPU::oam_pri3_enable(0, "ppu.oam_pri3_enable", "Enable OAM Priority 3", IntegerSetting::Boolean, true);

};
