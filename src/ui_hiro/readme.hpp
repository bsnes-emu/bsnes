static const char text_readme[] =
  "1. General:\n\n"

  "bsnes is a Super Nintendo / Super Famicom emulator that began development on October 14th, 2004.\n"
  "The latest version can be downloaded from: http://byuu.org/\n\n"

  "2. Configuration:\n\n"

  "bsnes has two configuration files: bsnes.cfg, for program settings; and locale.cfg, for localization.\n"
  "For each file, bsnes will start by looking inside the same folder where the "
  "bsnes executable is located. If said file is not found, it will then check your "
  "user profile folder. On Windows, this is located at '%APPDATA%/.bsnes'. On all "
  "other operating systems, this is located at '~/.bsnes'. If said file is still "
  "not found, it will automatically be created in your user profile folder.\n\n"

  "If you wish to use bsnes in single-user mode, be sure that both files exist "
  "inside the same folder as the bsnes executable. If they do not, you can simply "
  "create new blank files and bsnes will use them in the future.\n\n"

  "If you wish to use bsnes in multi-user mode, simply delete these two files from "
  "the bsnes executable directory if they exist.\n\n"

  "If you wish to have multiple configuration profiles for the same user, you will "
  "need to make copies of the bsnes executable, and use each one in single-user "
  "mode.\n\n"

  "3. Known Limitations:\n\n"

  "S-CPU\n"
  "- Multiply / divide register delays not implemented\n"
  "- 'Glitch' when reading joypad registers during auto polling not implemented\n"

  "S-PPU\n"
  "- Uses scanline-based renderer. This is very inaccurate, but few (if any) "
  "games rely on mid-scanline writes to function correctly\n"
  "- Does not support FirstSprite+Y priority\n"
  "- OAM / CGRAM accesses during active display not supported correctly\n"
  "- RTO flags are not calculated on frames that are skipped when frameskipping "
  "is enabled. This provides a major speedup, however it will cause in issues "
  "in games that test these flags, eg the SNES Test Program Electronics Test. "
  "Turning frameskipping off will allow RTO flag calculation on every frame\n\n"

  "Hardware Bugs\n"
  "- S-CPU.r1 HDMA crashing bug not emulated\n"
  "- S-CPU<>S-SMP communication bus conflicts not emulated\n\n"

  "4. Unsupported Hardware:\n\n"

  "SA-1\n"
  "Coprocessor used in many popular games, including:\n"
  "- Dragon Ball Z Hyper Dimension\n"
  "- Kirby Super Star\n"
  "- Kirby's Dreamland 3\n"
  "- Marvelous\n"
  "- SD Gundam G-NEXT\n"
  "- Super Mario RPG\n\n"

  "Super FX\n"
  "Coprocessor used in many popular games, including:\n"
  "- Doom\n"
  "- Star Fox\n"
  "- Star Fox 2 (unreleased beta)\n"
  "- Super Mario World 2: Yoshi's Island\n\n"

  "ST-011\n"
  "SETA DSP used by Quick-move Shogi Match with Nidan Rank-holder Morita\n\n"

  "ST-018\n"
  "SETA RISC CPU used by Quick-move Shogi Match with Nidan Rank-holder Morita 2\n\n"

  "Super Gameboy\n"
  "Cartridge passthrough used for playing Gameboy games\n\n"

  "5. Contributors:\n\n"

  "Andreas Naive, anomie, blargg, DMV27, FitzRoy, GIGO, Jonas Quinn, kode54, krom, "
  "Matthew Callis, Nach, neviksti, Overload, RedDwarf, Richard Bannister, "
  "tetsuo55, TRAC, zones"

  "";
