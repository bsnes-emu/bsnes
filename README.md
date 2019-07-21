bsnes
=====

bsnes is a multi-platform Super Nintendo (Super Famicom) emulator that focuses
on performance, features, and ease of use.

bsnes currently enjoys 100% known, bug-free compatibility with the entire SNES
library when configured to its most accurate settings.

Development
-----------

Git is used for the development of new releases, and represents a staging
environment. As bsnes is rather mature, things should generally be quite stable.
However, bugs will exist, regressions will occur, so proceed at your own risk.

If stability is required, please download the latest stable release from the
[official website.](https://bsnes.byuu.org)

Unique Features
---------------

  - True Super Game Boy emulation (using the SameBoy core by Lior Halphon)
  - HD mode 7 graphics with optional supersampling (by DerKoun)
  - Low-level emulation of all SNES coprocessors (DSP-n, ST-01n, Cx4)
  - Multi-threaded PPU graphics renderer
  - Speed mode settings which retain smooth audio output (50%, 75%, 100%, 150%, 200%)
  - Built-in games database with thousands of game entries
  - Built-in cheat code database for hundreds of popular games (by mightymo)
  - Built-in save state manager with screenshot previews and naming capabilities
  - Support for ASIO low-latency audio
  - Customizable per-byte game mappings to support any cartridges, including prototype games
  - 7-zip decompression support
  - Extensive Satellaview emulation, including BS Memory flash write and wear-leveling emulation
  - 30-bit color output support (where supported)
  - Optional higan game folder support (standard game ROM files are also fully supported!)

Standard Features
-----------------

  - MSU1 support
  - BPS and IPS soft-patching support
  - Save states with undo and redo support (for reverting accidental saves and loads)
  - OpenGL multi-pass pixel shaders
  - Several built-in software filters, including HQ2x (by MaxSt) and snes_ntsc (by blargg)
  - Adaptive sync and dynamic rate control for perfect audio/video synchronization
  - Just-in-time input polling for minimal input latency
  - Support for Direct3D exclusive mode video
  - Support for WASAPI exclusive mode audio
  - Periodic auto-saving of game saves
  - Auto-saving of states when unloading games, and auto-resuming of states when reloading games
  - Sprite limit disable support
  - Cubic audio interpolation support
  - Optional high-level emulation of most SNES coprocessors
  - SuperFX overclocking of up to 800%
  - Frame advance support
  - Screenshot support
  - Cheat code search support

Links
-----

  - [Official website](https://bsnes.byuu.org)
  - [Official git repository](https://github.com/byuu/bsnes)
  - [Donations](https://patreon.com/byuu)
