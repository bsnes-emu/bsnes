namespace config {

configuration& config() {
static configuration config;
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
    ltrim(path(), "./");
  string temp;
    strcpy(temp, config::path.base);
    strcat(temp, path);
    strcpy(path, temp);
  }

lstring part;
  split(part, "/", file);
  strcat(path, part[count(part) - 1]);
  return path;
}

string_setting Path::base("path.base",
  "Path that bsnes resides in", "");
string_setting Path::rom(config(), "path.rom",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
string_setting Path::save(config(), "path.save",
  "Default path for all save RAM and cheat files (\"\" = use current directory)", "");
string_setting Path::bsx(config(), "path.bsx", "", "");
string_setting Path::st(config(), "path.st", "", "");

integral_setting SNES::gamma_ramp(config(), "snes.colorfilter.gamma_ramp",
  "Use precalculated TV-style gamma ramp", integral_setting::boolean, true);
integral_setting SNES::sepia(config(), "snes.colorfilter.sepia",
  "Convert color to sepia tone", integral_setting::boolean, false);
integral_setting SNES::grayscale(config(), "snes.colorfilter.grayscale",
  "Convert color to grayscale tone", integral_setting::boolean, false);
integral_setting SNES::invert(config(), "snes.colorfilter.invert",
  "Invert output image colors", integral_setting::boolean, false);
integral_setting SNES::contrast(config(), "snes.colorfilter.contrast",
  "Contrast", integral_setting::decimal, 0);
integral_setting SNES::brightness(config(), "snes.colorfilter.brightness",
  "Brightness", integral_setting::decimal, 0);
integral_setting SNES::gamma(config(), "snes.colorfilter.gamma",
  "Gamma", integral_setting::decimal, 80);

integral_setting SNES::ntsc_merge_fields(config(), "snes.ntsc_merge_fields",
  "Merge fields in NTSC video filter\n"
  "Set to true if using filter at any refresh rate other than 60hz\n"
  "", integral_setting::boolean, true);

integral_setting SNES::controller_port0(config(), "snes.controller_port_1",
  "Controller attached to SNES port 1", integral_setting::decimal, ::SNES::DEVICEID_JOYPAD1);
integral_setting SNES::controller_port1(config(), "snes.controller_port_2",
  "Controller attached to SNES port 2", integral_setting::decimal, ::SNES::DEVICEID_JOYPAD2);

integral_setting CPU::ntsc_clock_rate(config(), "cpu.ntsc_clock_rate",
  "NTSC S-CPU clock rate (in hz)", integral_setting::decimal, 21477272);
integral_setting CPU::pal_clock_rate(config(), "cpu.pal_clock_rate",
  "PAL S-CPU clock rate (in hz)", integral_setting::decimal, 21281370);
integral_setting CPU::wram_init_value(config(), "cpu.wram_init_value",
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
  integral_setting::hex, 0x55);

integral_setting CPU::hdma_enable("cpu.hdma_enable",
  "Enable HDMA effects", integral_setting::boolean, true);

integral_setting SMP::ntsc_clock_rate(config(), "smp.ntsc_clock_rate",
  "NTSC S-SMP clock rate (in hz)", integral_setting::decimal, 24606720);
integral_setting SMP::pal_clock_rate(config(), "smp.pal_clock_rate",
  "PAL S-SMP clock rate (in hz)", integral_setting::decimal, 24606720);

integral_setting PPU::Hack::render_scanline_position(config(), "ppu.hack.render_scanline_position",
  "Approximate HCLOCK position to render at for scanline-based renderers",
  integral_setting::decimal, 512);
integral_setting PPU::Hack::obj_cache(config(), "ppu.hack.obj_cache",
  "Cache OAM OBJ attributes one scanline before rendering\n"
  "This is technically closer to the actual operation of the SNES,\n"
  "but can cause problems in some games if enabled",
  integral_setting::boolean, false);
integral_setting PPU::Hack::oam_address_invalidation(config(), "ppu.hack.oam_address_invalidation",
  "OAM access address changes during active display, as the S-PPU reads\n"
  "data to render the display. Thusly, the address retrieved when accessing\n"
  "OAM during active display is unpredictable. Unfortunately, the exact\n"
  "algorithm for this is completely unknown at this time. It is more hardware\n"
  "accurate to enable this setting, but one must *not* rely on the actual\n"
  "address to match hardware under emulation.",
  integral_setting::boolean, true);
integral_setting PPU::Hack::cgram_address_invalidation(config(), "ppu.hack.cgram_address_invalidation",
  "CGRAM access address changes during active display (excluding hblank), as\n"
  "the S-PPU reads data to render the display. Thusly, as with OAM, the access\n"
  "address is unpredictable. Again, enabling this setting is more hardware\n"
  "accurate, but one must *not* rely on the actual address to match hardware\n"
  "under emulation.",
  integral_setting::boolean, true);

integral_setting PPU::opt_enable("ppu.opt_enable", "Enable offset-per-tile effects", integral_setting::boolean, true);
integral_setting PPU::bg1_pri0_enable("ppu.bg1_pri0_enable", "Enable BG1 Priority 0", integral_setting::boolean, true);
integral_setting PPU::bg1_pri1_enable("ppu.bg1_pri1_enable", "Enable BG1 Priority 1", integral_setting::boolean, true);
integral_setting PPU::bg2_pri0_enable("ppu.bg2_pri0_enable", "Enable BG2 Priority 0", integral_setting::boolean, true);
integral_setting PPU::bg2_pri1_enable("ppu.bg2_pri1_enable", "Enable BG2 Priority 1", integral_setting::boolean, true);
integral_setting PPU::bg3_pri0_enable("ppu.bg3_pri0_enable", "Enable BG3 Priority 0", integral_setting::boolean, true);
integral_setting PPU::bg3_pri1_enable("ppu.bg3_pri1_enable", "Enable BG3 Priority 1", integral_setting::boolean, true);
integral_setting PPU::bg4_pri0_enable("ppu.bg4_pri0_enable", "Enable BG4 Priority 0", integral_setting::boolean, true);
integral_setting PPU::bg4_pri1_enable("ppu.bg4_pri1_enable", "Enable BG4 Priority 1", integral_setting::boolean, true);
integral_setting PPU::oam_pri0_enable("ppu.oam_pri0_enable", "Enable OAM Priority 0", integral_setting::boolean, true);
integral_setting PPU::oam_pri1_enable("ppu.oam_pri1_enable", "Enable OAM Priority 1", integral_setting::boolean, true);
integral_setting PPU::oam_pri2_enable("ppu.oam_pri2_enable", "Enable OAM Priority 2", integral_setting::boolean, true);
integral_setting PPU::oam_pri3_enable("ppu.oam_pri3_enable", "Enable OAM Priority 3", integral_setting::boolean, true);

};
