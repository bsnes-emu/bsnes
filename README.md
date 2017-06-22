higan, the multi-system emulator
================================

higan emulates a number of classic videogame consoles of the 1980s and 1990s,
allowing you to play classic games on a modern general-purpose computer.

As of v102,
higan has top-tier support for the following consoles:

  - Nintendo Super Famicom/Super Nintendo Entertainment System,
    including addon hardware:
      - Super Game Boy
      - Sufami Turbo
  - Nintendo Game Boy Advance

It also includes some level of support for these consoles:

  - BS-X Satellaview addon for the Super Famicom
  - Nintendo Famicom/Nintendo Entertainment System
  - Nintendo Game Boy
  - Nintendo Game Boy Color
  - Sega Master System
  - Sega Game Gear
  - Sega Megadrive/Genesis
  - NEC PC Engine/TurboGrafx 16 (but not the CD-ROM² System/TurboGrafx-CD)
  - NEC SuperGrafx
  - Bandai Wonderswan
  - Bandai Wonderswan Color

higan is actively supported on
FreeBSD 10 and above, and
Microsoft Windows 7 and above.
It also includes some level of support
for GNU/Linux and macOS.

higan is officially spelled with a lowercase "h", not a capital.

About this document
-------------------

This is the unofficial higan README,
a community-maintained introduction and reference.
It may be out of date
by the time you read this,
and it may contain errors or omissions.
If you find something that's wrong,
or you have a suggestion,
see "Unofficial higan resources" below.

Official higan resources
------------------------

  - [Official homepage](https://byuu.org/emulation/higan/)
  - [Official forum](https://board.byuu.org/viewforum.php?f=4)

Unofficial higan resources
--------------------------

  - [Source code repository](https://gitlab.com/higan/higan/)
    archives official higan releases
    and WIP snapshots
    since approximately v067r21.
  - [Quark shader repository](https://github.com/hizzlekizzle/quark-shaders)
    collects shaders that higan can use
    to add special effects like TV scanlines to its video output,
    or smarter algorithms for scaling up to modern PC resolutions.
    See [Installing custom shaders][shaders] below for details.
  - [Mercurial Magic](https://github.com/hex-usr/Mercurial-Magic/)
    is a tool for converting MSU-1 games and mods into a format
    higan can use.
    See [Importing MSU-1 games][msu1] below for details.

[shaders]: #installing-custom-shaders
[msu1]: #importing-msu-1-games

There are also other projects
based on current or older versions of higan,
in whole or in part,
that you might want to check out.

  - [Mednafen](https://mednafen.github.io/)
    is another multi-system emulator.
    Its SNES emulation is based on bsnes v059,
    from the time before bsnes was renamed to higan.
  - [BizHawk](http://tasvideos.org/BizHawk.html)
    is another multi-system emulator,
    specialising in the creation of
    tool-assisted speedruns.
    Its SNES emulation is based on bsnes v087.
  - [nSide](https://github.com/hex-usr/nSide)
    is a fork of higan that greatly enhances
    its NES emulation support,
    and adds minor features to the other cores too.
    It also restores the "balanced" SNES emulation core
    that was removed from higan in v099,
    which is less CPU intensive
    than the current accuracy-focussed core.
  - [bsnes-plus](https://github.com/devinacker/bsnes-plus)
    is a fork of bsnes v073
    that adds improved support for debugging SNES software.

Installing and uninstalling higan
=================================

The best way to install higan
depends on what platform you're using,
as well as whether you want to use official binaries
or compile the source-code from scratch.

Installing an official release on Windows
-----------------------------------------

Official higan releases are distributed in [7-zip][7z] archives.
You will need to install 7-zip,
or another compatible archiving tool,
to install higan.

[7z]: http://www.7-zip.org/

Once you have a suitable archiving tool,
extract the contents of the higan archive into a new directory.

When you're done,
the new directory should contain `higan.exe` and `icarus.exe`
along with other assorted files and directories
that describe the systems higan emulates.

You may put that directory wherever you like.

To run higan, open the `higan.exe` file.

Before you can actually play games,
you'll need to [import them][#importing-games]
and [configure higan][#configuring-higan].

Uninstalling an official release on Windows
-------------------------------------------

Delete the directory containing `higan.exe`
and the other associated data from the original archive.

To remove higan's configuration:

 1. Press Win+R to open the Run dialog
 2. Type `%APPDATA%` and press Enter
    to open the folder where higan's configuration data lives
 3. Delete the subdirectories named `icarus` and `higan`
    if they exist.

You might also want to remove the games imported into higan's library
(including in-game saves and save-states):

 1. Press Win+R to open the Run dialog
 2. Type `%USERPROFILE%` and press Enter
    to open the folder where higan keeps its game library
 3. Delete the subdirectory named `Emulation` if it exists

Compiling from source on Windows
--------------------------------

Official higan releases are distributed in [7-zip][7z] archives.
You will need to install 7-zip,
or another compatible archiving tool,
to extract the higan source code.
Alternatively,
you may obtain higan source code from
[the unofficial git repo](https://gitlab.com/higan/higan/)
using the Git source-code management tool,
or by clicking the download button on the right-hand side of the web-page
and choosing an archive format.

- TDM-GCC?
- mingw-w64?
- mingw?
- isn't there some bug with building higan for 32-bit Windows?

Compiling from source on Linux
------------------------------

Official higan releases are distributed in [7-zip][7z] archives.
You will need to install 7-zip,
or another compatible archiving tool,
to extract the higan source code.
Alternatively,
you may obtain higan source code from
[the unofficial git repo](https://gitlab.com/higan/higan/)
using the Git source-code management tool,
or by clicking the download button on the right-hand side of the web-page
and choosing an archive format.

You will also need GCC 4.9 or higher,
including the C and C++ compiler,
GNU Make,
and development files
(headers, etc.)
for the following libraries:

  - GTK 2.x
  - PulseAudio
  - Mesa
  - gtksourceview 2.x
  - Cairo
  - SDL 1.2
  - libXv
  - libAO
  - OpenAL
  - udev

On a Debian-derived Linux distribution,
you can install everything you need with a command like:

    sudo apt-get install build-essential libgtk2.0-dev libpulse-dev \
        mesa-common-dev libgtksourceview2.0-dev libcairo2-dev libsdl1.2-dev \
        libxv-dev libao-dev libopenal-dev libudev-dev

Installing the GBA BIOS
-----------------------

Importing games
===============

What's a game folder?
---------------------

Importing regular games
-----------------------

Importing games with co-processor firmware
------------------------------------------

Importing BS-X games
--------------------

Importing Sufami Turbo games
----------------------------

Importing Super Game Boy games
------------------------------

Importing MSU-1 games
---------------------

Configuring higan
=================

Drivers
-------

Installing custom shaders
-------------------------

Controls
--------

  - mapping PC inputs to emulated controllers
  - configuring which emulated controllers
    are connected to the emulated system

Notes on specific emulation cores
=================================

The WonderSwan rotates!

Shaders look weird on SNES because of interlace/hi-res

GBA in-game saves are Difficult

Frequently Asked Questions
==========================

where is the vsync option?

exclusive full-screen?

phones and tablets
