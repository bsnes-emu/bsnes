higan, the multi-system emulator
================================

higan emulates a number of classic 2D video-game consoles,
allowing you to play classic games on a modern general-purpose computer.

To get started with higan right away,
see the [Quick Start](qs.md) section of the documentation.


About higan
-----------

As of v107,
higan has top-tier support for the following consoles:

  - Nintendo Super Famicom/Super Nintendo Entertainment System,
    including add-on hardware:
      - Super Game Boy
      - Sufami Turbo
  - Nintendo Game Boy Advance

It also includes some level of support for these consoles:

  - Satellaview add-on for the Super Famicom
  - Nintendo Famicom/Nintendo Entertainment System
  - Nintendo Game Boy
  - Nintendo Game Boy Color
  - Sega Master System
  - Sega Game Gear
  - Sega Mega Drive/Genesis
  - NEC PC Engine/TurboGrafx 16 (but not the CD-ROM² System/TurboGrafx-CD)
  - NEC SuperGrafx
  - Bandai WonderSwan
  - Bandai WonderSwan Color
  - Pocket Challenge v2

**Note:** Some consoles were released under different names
in different geographic regions.
To avoid listing all possible names
every time such a console is mentioned,
higan uses the name from the console's region of origin.
In practice,
that means Japanese names:
"Famicom" and "Super Famicom" instead of "NES" and "SNES",
"Mega Drive" instead of "Genesis",
"PC Engine" instead of "TurboGrafx-16".

higan is actively supported on
FreeBSD 10 and above, and
Microsoft Windows 7 and above.
It also includes some level of support
for GNU/Linux and macOS.

higan is officially spelled with a lowercase "h", not a capital.

About this document
-------------------

This is the semi-official higan manual,
a community-maintained introduction and reference.
It may be out of date
by the time you read this,
and it may contain errors or omissions.
If you find something that's wrong,
or you have a suggestion,
post a message on the official higan forum.

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
    See [Using video shaders][shaders] below for details.
  - [Mercurial Magic](https://github.com/hex-usr/Mercurial-Magic/)
    is a tool for converting MSU-1 games and mods into a format
    higan can use.
    See [Importing MSU-1 games][msu1] for details.

[shaders]: guides/shaders.md
[msu1]: guides/import.md#msu-1-games

There are also other projects
based on current or older versions of higan,
in whole or in part,
that you might want to check out.

  - [Mednafen](https://mednafen.github.io/)
    is another multi-system emulator.
    Its Super Famicom emulation is based on bsnes v059,
    from the time before bsnes was renamed to higan.
  - [BizHawk](http://tasvideos.org/BizHawk.html)
    is another multi-system emulator,
    specialising in the creation of
    tool-assisted speed-runs.
    Its Super Famicom emulation is based on bsnes v087.
  - [bsnes-plus](https://github.com/devinacker/bsnes-plus)
    is a fork of bsnes v073
    that adds improved support for debugging Super Famicom software.
  - [bsnes-mercury](https://github.com/libretro/bsnes-mercury/)
    is a fork of bsnes v094
    adapted to work as a
    [libretro](https://www.libretro.com/) emulation core.
  - [nSide](https://github.com/hex-usr/nSide)
    is a fork of higan that greatly enhances
    its NES emulation support,
    and adds minor features to the other cores too.
    It also restores the "balanced" Super Famicom emulation core
    that was removed from higan in v099,
    which is less CPU intensive
    than the current accuracy-focused core.
