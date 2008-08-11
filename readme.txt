bsnes
Version: 0.034
Author: byuu

========
General:
========

bsnes is a Super Nintendo / Super Famicom emulator that began on
October 14th, 2004.

The latest version can be downloaded from:
http://byuu.org/

Please see license.txt for important licensing information.

==============
Configuration:
==============

bsnes has two configuration files: bsnes.cfg, for program settings; and
locale.cfg, for localization.

For each file, bsnes will start by looking inside the same folder where the
bsnes executable is located. If said file is not found, it will then check your
user profile folder. On Windows, this is located at "%APPDATA%/.bsnes". On all
other operating systems, this is located at "~/.bsnes". If said file is still
not found, it will automatically be created in your user profile folder.

If you wish to use bsnes in single-user mode, be sure that both files exist
inside the same folder as the bsnes executable. If they do not, you can simply
create new blank files and bsnes will use them in the future.

If you wish to use bsnes in multi-user mode, simply delete these two files from
the bsnes executable directory if they exist.

If you wish to have multiple configuration profiles for the same user, you will
need to make copies of the bsnes executable, and use each one in single-user
mode.

==================
Known Limitations:
==================

S-CPU
- Multiply / divide register delays not implemented
- "Glitch" when reading joypad registers during auto polling not implemented

S-PPU
- Uses scanline-based renderer. This is very inaccurate, but few (if any)
  games rely on mid-scanline writes to function correctly
- Does not support FirstSprite+Y priority
- OAM / CGRAM accesses during active display not supported correctly
- RTO flags are not calculated on frames that are skipped when frameskipping
  is enabled. This provides a major speedup, however it will cause in issues
  in games that test these flags, eg the SNES Test Program Electronics Test.
  Turning frameskipping off will allow RTO flag calculation on every frame

Hardware Bugs
- S-CPU.r1 HDMA crashing bug not emulated
- S-CPU<>S-SMP communication bus conflicts not emulated

=====================
Unsupported Hardware:
=====================

SA-1
Coprocessor used in many popular games, including:
- Dragon Ball Z Hyper Dimension
- Kirby Super Star
- Kirby's Dreamland 3
- Marvelous
- SD Gundam G-NEXT
- Super Mario RPG

Super FX
Coprocessor used in many popular games, including:
- Doom
- Star Fox
- Star Fox 2 (unreleased beta)
- Super Mario World 2: Yoshi's Island

ST-011
SETA DSP used by Quick-move Shogi Match with Nidan Rank-holder Morita

ST-018
SETA RISC CPU used by Quick-move Shogi Match with Nidan Rank-holder Morita 2

Super Gameboy
Cartridge passthrough used for playing Gameboy games

========================
Unsupported Controllers:
========================

Mouse
Super Scope
Justifier

=============
Contributors:
=============

Andreas Naive, anomie, blargg, DMV27, FitzRoy, GIGO, Jonas Quinn, kode54, krom,
mudlord, Nach, neviksti, Overload, RedDwarf, Richard Bannister, tetsuo55, TRAC,
zones
