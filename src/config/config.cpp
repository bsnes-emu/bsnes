namespace config {

configuration& config() {
  static configuration config;
  return config;
}

integral_setting File::autodetect_type(config(), "file.autodetect_type",
  "Auto-detect file type by inspecting file header, rather than by file extension.\n"
  "In other words, if a .zip file is renamed to .smc, it will still be correctly\n"
  "identified as a .zip file. However, there is an infinitesimal (1:~500,000,000)\n"
  "chance of a false detection when loading an uncompressed image file, if this\n"
  "option is enabled.",
  integral_setting::boolean, false);

integral_setting File::bypass_patch_crc32(config(), "file.bypass_patch_crc32",
  "UPS patches contain CRC32s to validate that a patch was applied successfully.\n"
  "By default, if this validation fails, said patch will not be applied.\n"
  "Setting this option to true will bypass the validation,\n"
  "which may or may not result in a working image.\n"
  "Enabling this option is strongly discouraged.",
  integral_setting::boolean, false);

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

string_setting Path::base("path.base", "Path that bsnes resides in", "");
string_setting Path::user("path.user", "Path to user folder", "");

string_setting Path::rom(config(), "path.rom",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
string_setting Path::patch(config(), "path.patch",
  "Default path for all UPS patch files (\"\" = use current directory)", "");
string_setting Path::save(config(), "path.save",
  "Default path for all save RAM files (\"\" = use current directory)", "");
string_setting Path::cheat(config(), "path.cheat",
  "Default path for all cheat files (\"\" = use current directory)", "");
string_setting Path::bsx(config(), "path.bsx", "", "");
string_setting Path::st(config(), "path.st", "", "");

integral_setting SNES::controller_port1(config(), "snes.controller_port1",
  "Controller attached to SNES port 1", integral_setting::decimal, ::SNES::Input::DeviceJoypad);
integral_setting SNES::controller_port2(config(), "snes.controller_port2",
  "Controller attached to SNES port 2", integral_setting::decimal, ::SNES::Input::DeviceJoypad);
integral_setting SNES::expansion_port(config(), "snes.expansion_port",
  "Device attached to SNES expansion port\n"
  "0 = None\n"
  "1 = Satellaview BS-X\n"
  "", integral_setting::decimal, ::SNES::ExpansionBSX);
integral_setting SNES::region(config(), "snes.region",
  "SNES regional model\n"
  "0 = Auto-detect based on cartridge\n"
  "1 = NTSC\n"
  "2 = PAL\n"
  "", integral_setting::decimal, ::SNES::Autodetect);

integral_setting CPU::ntsc_clock_rate(config(), "cpu.ntsc_clock_rate",
  "NTSC S-CPU clock rate (in hz)", integral_setting::decimal, 21477272);
integral_setting CPU::pal_clock_rate(config(), "cpu.pal_clock_rate",
  "PAL S-CPU clock rate (in hz)", integral_setting::decimal,  21281370);
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

integral_setting SMP::ntsc_clock_rate(config(), "smp.ntsc_clock_rate",
  "NTSC S-SMP clock rate (in hz)", integral_setting::decimal, 32041 * 768);
integral_setting SMP::pal_clock_rate(config(), "smp.pal_clock_rate",
  "PAL S-SMP clock rate (in hz)", integral_setting::decimal,  32041 * 768);

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

} //namespace config
