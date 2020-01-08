bsnes
=====

![bsnes logo © byuu](https://byuu.org/images/bsnes/github/byuu-bsnes-logo.png)

bsnes is a multi-platform Super Nintendo (Super Famicom) emulator from
[byuu](https://byuu.org) that focuses on performance, features, and ease of use.

bsnes currently enjoys 100% known, bug-free compatibility with the entire SNES
library when configured to its most accurate settings, giving it the same
accuracy level as higan. Accuracy can also optionally be traded for performance,
allowing bsnes to operate more than 300% faster than higan while still remaining
almost as accurate.

Development
-----------

Git is used for the development of new releases, and represents a staging
environment. As bsnes is rather mature, things should generally be quite stable.
However, bugs will exist, regressions will occur, so proceed at your own risk.

If stability is required, please download the latest stable release from the
[official website.](https://bsnes.byuu.org)

Unique Features
---------------

  - 100% (known) bug-free compatibility with the entire officially licensed SNES games library
  - True Super Game Boy emulation (using the SameBoy core by Lior Halphon)
  - HD mode 7 graphics with optional supersampling (by DerKoun)
  - Low-level emulation of all SNES coprocessors (DSP-n, ST-01n, Cx4)
  - Multi-threaded PPU graphics renderer
  - Speed mode settings which retain smooth audio output (50%, 75%, 100%, 150%, 200%)
  - Built-in games database with thousands of game entries
  - Built-in cheat code database for hundreds of popular games (by mightymo)
  - Built-in save state manager with screenshot previews and naming capabilities
  - Customizable per-byte game mappings to support any cartridges, including prototype games
  - 7-zip decompression support
  - Extensive Satellaview emulation, including BS Memory flash write and wear-leveling emulation
  - Optional higan game folder support (standard game ROM files are also fully supported!)
  - Advanced mapping system allowing multiple bindings to every emulated input

Standard Features
-----------------

  - MSU1 support
  - BPS and IPS soft-patching support
  - Save states with undo and redo support (for reverting accidental saves and loads)
  - OpenGL multi-pass pixel shaders
  - Several built-in software filters, including HQ2x (by MaxSt) and snes_ntsc (by blargg)
  - Adaptive sync and dynamic rate control for perfect audio/video synchronization
  - Just-in-time input polling for minimal input latency
  - Run-ahead support for removing internal game engine input latency
  - Support for Direct3D exclusive mode video
  - Support for WASAPI exclusive mode audio
  - Periodic auto-saving of game saves
  - Auto-saving of states when unloading games, and auto-resuming of states when reloading games
  - Sprite limit disable support
  - Cubic audio interpolation support
  - Optional high-level emulation of most SNES coprocessors
  - Optional emulation of flaws in older emulators for compatibility with older unofficial software
  - CPU, SA1, and SuperFX overclocking support
  - Frame advance support
  - Screenshot support
  - Cheat code search support
  - Movie recording and playback support
  - Rewind support
  - HiDPI support
  - Multi-monitor support
  - Turbo support for controller inputs

Links
-----

  - [Official website](https://byuu.org/bsnes)
  - [Official git repository](https://github.com/byuu/bsnes)
  - [Developer resources](https://byuu.net)
  - [Donations](https://patreon.com/byuu)

Release Builds
--------------

  - [Windows binaries](https://byuu.itch.io/bsnes)

Nightly Builds
--------------

  - [Download](https://cirrus-ci.com/github/byuu/bsnes/master)
  - ![Build status](https://api.cirrus-ci.com/github/byuu/bsnes.svg?task=windows-x86_64-binaries)
  - ![Build status](https://api.cirrus-ci.com/github/byuu/bsnes.svg?task=macOS-x86_64-binaries)
  - ![Build status](https://api.cirrus-ci.com/github/byuu/bsnes.svg?task=linux-x86_64-binaries)
  - ![Build status](https://api.cirrus-ci.com/github/byuu/bsnes.svg?task=freebsd-x86_64-binaries)

Preview
-------

![bsnes user interface © byuu](https://byuu.org/images/bsnes/github/byuu-bsnes-user-interface.png)
![bsnes running Bahamut Lagoon © byuu](https://byuu.org/images/bsnes/github/byuu-bsnes-bahamut-lagoon.png)
![bsnes running Tengai Makyou Zero © byuu](https://byuu.org/images/bsnes/github/byuu-bsnes-tengai-makyou-zero.png)
