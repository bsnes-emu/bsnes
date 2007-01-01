bsnes
Version 0.019
Author: byuu


General
-------
bsnes is a Super Nintendo / Super Famicom emulator that began on
October 14th, 2004.

The latest version can be downloaded from:
http://byuu.org/

Please see license.txt for important licensing information.


Known Limitations
-----------------
S-CPU
- Invalid DMA / HDMA transfers (eg WRAM<>WRAM) not fully emulated
- Multiply / Divide register delays not implemented

S-SMP
- Cycle breakdown of opcodes is theoretical, but mostly correct

S-PPU
- Uses scanline-based renderer. This is very inaccurate, but very
  few games rely on mid-scanline writes to function correctly
- Does not support FirstSprite+Y priority
- OAM / CGRAM accesses during active display not supported correctly
- RTO flags are not calculated on frames that are skipped when frameskipping
  is enabled. This provides a major speedup, however it will cause in issues
  in games that test these flags, eg the SNES Test Program Electronics Test.
  Turning frameskipping off will allow RTO flag calculation on every frame

S-DSP
- Runs at 32khz. Hardware S-DSP likely runs at 1.024mhz to perform
  multiple reads / writes per sample. Sound is still output at 32khz,
  of course

Hardware Bugs
- CPUr1 HDMA crashing bug not emulated
- CPU<>APU communication bus conflicts not emulated


Unsupported Hardware
--------------------
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

DSP-3
Coprocessor used only by SD Gundam GX

DSP-4
Coprocessor used only by Top Gear 3000

ST010 / ST011 / ST018
SETA coprocessors used by very few games

BS-X (Broadcast Satellite)
Add-on unit sold only in Japan that played specially-made games that
were downloaded via satellite

BS-X Flashcart
Flash cartridge used by BS-X, as well as some standalone games by
Asciisoft

Super Gameboy
Cartridge passthrough used for playing Gameboy games


Unsupported controllers
-----------------------
Mouse
Super Scope
Justifier
Multitap (4-port)
Multitap (5-port)
