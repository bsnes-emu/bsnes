bsnes
Version: 0.028
Author: byuu

--------
General:
--------
bsnes is a Super Nintendo / Super Famicom emulator that began on
October 14th, 2004.

The latest version can be downloaded from:
http://byuu.org/

Please see license.txt for important licensing information.

------------------
Known Limitations:
------------------
S-CPU
- Invalid DMA / HDMA transfers not fully emulated
- Multiply / Divide register delays not implemented

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

---------------------
Unsupported Hardware:
---------------------
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

SPC7110
Coprocessor used only by the following games:
- Far East of Eden Zero
- Far East of Eden Zero: Shounen Jump no Shou
- Momotarou Densetsu Happy
- Super Power League 4

ST-011
SETA DSP used only by Quick-move Shogi Match with Nidan Rank-holder Morita

ST-018
SETA RISC CPU used only by Quick-move Shogi Match with Nidan Rank-holder Morita 2

Super Gameboy
Cartridge passthrough used for playing Gameboy games

------------------------
Unsupported Controllers:
------------------------
Mouse
Super Scope
Justifier
Multitap (4-port and 5-port)
