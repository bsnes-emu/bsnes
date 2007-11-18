namespace config {

Config& config() {
static Config config;
  return config;
}

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

StringSetting Path::base(0, "path.base",
  "Path that bsnes resides in", "");
StringSetting Path::rom(&config(), "path.rom",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
StringSetting Path::save(&config(), "path.save",
  "Default path for all save RAM and cheat files (\"\" = use current directory)", "");
StringSetting Path::bsx(&config(), "path.bsx", "", "");
StringSetting Path::st(&config(), "path.st", "", "");

IntegerSetting SNES::gamma_ramp(&config(), "snes.colorfilter.gamma_ramp",
  "Use precalculated TV-style gamma ramp", IntegerSetting::Boolean, true);
IntegerSetting SNES::sepia(&config(), "snes.colorfilter.sepia",
  "Convert color to sepia tone", IntegerSetting::Boolean, false);
IntegerSetting SNES::grayscale(&config(), "snes.colorfilter.grayscale",
  "Convert color to grayscale tone", IntegerSetting::Boolean, false);
IntegerSetting SNES::invert(&config(), "snes.colorfilter.invert",
  "Invert output image colors", IntegerSetting::Boolean, false);
IntegerSetting SNES::contrast(&config(), "snes.colorfilter.contrast",
  "Contrast", IntegerSetting::Decimal, 0);
IntegerSetting SNES::brightness(&config(), "snes.colorfilter.brightness",
  "Brightness", IntegerSetting::Decimal, 0);
IntegerSetting SNES::gamma(&config(), "snes.colorfilter.gamma",
  "Gamma", IntegerSetting::Decimal, 80);

IntegerSetting SNES::ntsc_merge_fields(&config(), "snes.ntsc_merge_fields",
  "Merge fields in NTSC video filter\n"
  "Set to true if using filter at any refresh rate other than 60hz\n"
  "", IntegerSetting::Boolean, true);

IntegerSetting SNES::mute(&config(), "snes.mute", "Mutes SNES audio output when enabled",
  IntegerSetting::Boolean, false);

IntegerSetting SNES::controller_port0(&config(), "snes.controller_port_1",
  "Controller attached to SNES port 1", IntegerSetting::Decimal, ::SNES::DEVICEID_JOYPAD1);
IntegerSetting SNES::controller_port1(&config(), "snes.controller_port_2",
  "Controller attached to SNES port 2", IntegerSetting::Decimal, ::SNES::DEVICEID_JOYPAD2);

IntegerSetting CPU::ntsc_clock_rate(&config(), "cpu.ntsc_clock_rate",
  "NTSC S-CPU clock rate (in hz)", IntegerSetting::Decimal, 21477272);
IntegerSetting CPU::pal_clock_rate(&config(), "cpu.pal_clock_rate",
  "PAL S-CPU clock rate (in hz)", IntegerSetting::Decimal, 21281370);
IntegerSetting CPU::wram_init_value(&config(), "cpu.wram_init_value",
  "Value to initialize 128k WRAM to upon power cycle.\n"
  "Note that on real hardware, this value is undefined; meaning it can vary\n"
  "per power-on, and per SNES unit. Such randomness is undesirable for an\n"
  "emulator, so a static value is needed. There is also some form of pattern\n"
  "to the randomness that has yet to be determined, which some games rely upon.\n"
  "A value of 0x55 is safe for all known commercial software, and should be used.\n"
  "However, some software written for SNES copiers, or backup units, relies on\n"
  "WRAM being initialized to 0x00; which was a side-effect of the BIOS program\n"
  "which executed on these copiers. Using 0x00 will therefore fix many homebrew\n"
  "programs, but *will* break some poorly programmed commercial software titles,\n"
  "which do not properly initialize WRAM upon power cycle.\n",
  IntegerSetting::Hex, 0x55);

IntegerSetting CPU::hdma_enable(0, "cpu.hdma_enable",
  "Enable HDMA effects", IntegerSetting::Boolean, true);

IntegerSetting SMP::ntsc_clock_rate(&config(), "smp.ntsc_clock_rate",
  "NTSC S-SMP clock rate (in hz)", IntegerSetting::Decimal, 24606720);
IntegerSetting SMP::pal_clock_rate(&config(), "smp.pal_clock_rate",
  "PAL S-SMP clock rate (in hz)", IntegerSetting::Decimal, 24606720);

IntegerSetting PPU::Hack::render_scanline_position(&config(), "ppu.hack.render_scanline_position",
  "Approximate HCLOCK position to render at for scanline-based renderers",
  IntegerSetting::Decimal, 512);
IntegerSetting PPU::Hack::obj_cache(&config(), "ppu.hack.obj_cache",
  "Cache OAM OBJ attributes one scanline before rendering\n"
  "This is technically closer to the actual operation of the SNES,\n"
  "but can cause problems in some games if enabled",
  IntegerSetting::Boolean, false);
IntegerSetting PPU::Hack::oam_address_invalidation(&config(), "ppu.hack.oam_address_invalidation",
  "OAM access address changes during active display, as the S-PPU reads\n"
  "data to render the display. Thusly, the address retrieved when accessing\n"
  "OAM during active display is unpredictable. Unfortunately, the exact\n"
  "algorithm for this is completely unknown at this time. It is more hardware\n"
  "accurate to enable this setting, but one must *not* rely on the actual\n"
  "address to match hardware under emulation.",
  IntegerSetting::Boolean, true);
IntegerSetting PPU::Hack::cgram_address_invalidation(&config(), "ppu.hack.cgram_address_invalidation",
  "CGRAM access address changes during active display (excluding hblank), as\n"
  "the S-PPU reads data to render the display. Thusly, as with OAM, the access\n"
  "address is unpredictable. Again, enabling this setting is more hardware\n"
  "accurate, but one must *not* rely on the actual address to match hardware\n"
  "under emulation.",
  IntegerSetting::Boolean, true);

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
