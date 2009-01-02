#include <../base.hpp>

namespace config {

configuration& config() {
  static configuration config;
  return config;
}

string filepath(const char *filename, const char *pathname) {
  //if no pathname, return filename as-is
  string file(filename);
  replace(file, "\\", "/");
  if(!pathname || !*pathname) return file;

  //ensure path ends with trailing '/'
  string path(pathname);
  replace(path, "\\", "/");
  if(!strend(path, "/")) strcat(path, "/");

  //replace relative path with absolute path
  if(strbegin(path, "./")) {
    ltrim(path, "./");
    path = string() << config::path.base << path;
  }

  //remove folder part of filename
  lstring part;
  split(part, "/", file);
  return path << part[count(part) - 1];
}

integral_setting File::autodetect_type(config(), "file.autodetect_type",
  "Auto-detect file type by inspecting file header, rather than by file extension.\n"
  "In other words, if a .zip file is renamed to .smc, it will still be correctly "
  "identified as a .zip file. However, there is an infinitesimal (1:~500,000,000) "
  "chance of a false detection when loading an uncompressed image file, if this "
  "option is enabled.",
  integral_setting::boolean, false);

integral_setting File::bypass_patch_crc32(config(), "file.bypass_patch_crc32",
  "UPS patches contain CRC32s to validate that a patch was applied successfully.\n"
  "By default, if this validation fails, said patch will not be applied. "
  "Setting this option to true will bypass the validation, "
  "which may or may not result in a working image. "
  "Thus, enabling this option is strongly discouraged.",
  integral_setting::boolean, false);

string_setting Path::base("path.base", "Path that bsnes resides in", "");
string_setting Path::user("path.user", "Path to user folder", "");

string_setting Path::rom(config(), "path.rom",
  "Default path to look for ROM files in (\"\" = use default directory)", "");
string_setting Path::save(config(), "path.save",
  "Default path for all save RAM files (\"\" = use current directory)", "");
string_setting Path::patch(config(), "path.patch",
  "Default path for all UPS patch files (\"\" = use current directory)", "");
string_setting Path::cheat(config(), "path.cheat",
  "Default path for all cheat files (\"\" = use current directory)", "");
string_setting Path::exportdata(config(), "path.export",
  "Default path for all exported data files\n", "");

string_setting Path::bsx(config(), "path.bsx", "", "");
string_setting Path::st(config(), "path.st", "", "");

integral_setting SNES::controller_port1(config(), "snes.controller_port1",
  "Controller attached to SNES port 1", integral_setting::decimal, ::SNES::Input::DeviceJoypad);
integral_setting SNES::controller_port2(config(), "snes.controller_port2",
  "Controller attached to SNES port 2", integral_setting::decimal, ::SNES::Input::DeviceJoypad);
integral_setting SNES::expansion_port(config(), "snes.expansion_port",
  "Device attached to SNES expansion port\n"
  "0 = None\n"
  "1 = Satellaview BS-X",
  integral_setting::decimal, ::SNES::ExpansionBSX);
integral_setting SNES::region(config(), "snes.region",
  "SNES regional model\n"
  "0 = Auto-detect based on cartridge\n"
  "1 = NTSC\n"
  "2 = PAL",
  integral_setting::decimal, ::SNES::Autodetect);

integral_setting CPU::ntsc_clock_rate(config(), "cpu.ntsc_clock_rate",
  "NTSC S-CPU clock rate (in hz)", integral_setting::decimal, 21477272);
integral_setting CPU::pal_clock_rate(config(), "cpu.pal_clock_rate",
  "PAL S-CPU clock rate (in hz)", integral_setting::decimal,  21281370);
integral_setting CPU::wram_init_value(config(), "cpu.wram_init_value",
  "Value to initialize 128k WRAM to upon power cycle.\n"
  "Note that on real hardware, this value is undefined; meaning it can vary "
  "per power-on, and per SNES unit. Such randomness is undesirable for an "
  "emulator, so a static value is needed. There is also some form of pattern "
  "to the randomness that has yet to be determined, which some games rely upon.\n"
  "A value of 0x55 is safe for all known commercial software, and should be used. "
  "However, some software written for SNES copiers, or backup units, relies on "
  "WRAM being initialized to 0x00; which was a side-effect of the BIOS program "
  "which executed on these copiers. Using 0x00 will therefore fix many homebrew "
  "programs, but *will* break some poorly programmed commercial software titles, "
  "which do not properly initialize WRAM upon power cycle.",
  integral_setting::hex, 0x55);

integral_setting SMP::ntsc_clock_rate(config(), "smp.ntsc_clock_rate",
  "NTSC S-SMP clock rate (in hz)", integral_setting::decimal, 32041 * 768);
integral_setting SMP::pal_clock_rate(config(), "smp.pal_clock_rate",
  "PAL S-SMP clock rate (in hz)", integral_setting::decimal,  32041 * 768);

integral_setting Temp::alu_mul_delay(config(), "temp.alu_mul_delay",
  "", integral_setting::decimal, 32);
integral_setting Temp::alu_div_delay(config(), "temp.alu_div_delay",
  "", integral_setting::decimal, 64);

} //namespace config
