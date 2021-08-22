# bsnes

![bsnes logo](bsnes/target-bsnes/resource/logo.png)

bsnes is a multi-platform Super Nintendo (Super Famicom) emulator, originally
developed by [Near](https://near.sh), which focuses on performance,
features, and ease of use.

## Community

  - [Official git repository](https://github.com/bsnes-emu/bsnes)
  - [Discussion Forum](https://helmet.kafuka.org/bboard/forum.php?id=6)
  - [![Discord](https://img.shields.io/badge/chat-on%20discord-7289da.svg?logo=discord)](https://discord.gg/B27hf27ZVf)

## Unique Features

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

## Standard Features

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

## Nightly Builds

  - [Download](https://cirrus-ci.com/github/bsnes-emu/bsnes/master)
  - ![Build status](https://api.cirrus-ci.com/github/bsnes-emu/bsnes.svg?task=windows-x86_64-binaries)
  - ![Build status](https://api.cirrus-ci.com/github/bsnes-emu/bsnes.svg?task=macOS-x86_64-binaries)
  - ![Build status](https://api.cirrus-ci.com/github/bsnes-emu/bsnes.svg?task=linux-x86_64-binaries)
  - ![Build status](https://api.cirrus-ci.com/github/bsnes-emu/bsnes.svg?task=freebsd-x86_64-binaries)

## Compiling instructions

If you want to compile by yourself, it's also very easy to. Just follow these steps:

### Prerequisites

 * Updated C/C++ compilation suite already installed and with enviroment paths ready for your operational system 
For Windows, we can suggest [MinGW] (http://mingw-w64.org/).
Another good suggestion [MSYS2](https://www.msys2.org/). It also has versions for MacOS and Linux (and even come in some distributions).
 * (optional) [Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).

### Generating the executable

1. Open a command prompt in your directory of choice and clone the most recent version of the code in a directory of your choice.
  * you can get it ready manually just extracting the source code (that you can download just clicking in the "Code" button and choosing "download zip") in your directory of choice
  * if you have Git installed, you can do it with the command prompt/terminal:
  `git clone https://github.com/bsnes-emu/bsnes.git`

2. Go to `bsnes` folder with: `cd bsnes`.

3. Now you just need to put the command: `make` (for most of C/C++ compilation suites) or `mingw32-make`(if you're using Windows MinGW). The compilation will start.

4. The brand new compiled file will be at directory `./bsnes/out`. Enjoy!

## Preview

![bsnes user interface](.assets/user-interface.png)
![bsnes running Bahamut Lagoon](.assets/bahamut-lagoon.png)
![bsnes running Tengai Makyou Zero](.assets/tengai-makyou-zero.png)
