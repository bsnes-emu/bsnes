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

  - Satellaview addon for the Super Famicom
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

**Note:** Some consoles were released under different names
in different geographic regions.
To avoid listing all possible names
every time such a console is mentioned,
higan uses the name from the console's region of origin.
In practice,
that means Japanese names:
"Famicom" and "Super Famicom" instead of NES and SNES,
"Mega Drive" instead of "Genesis",
"PC Engine" instead of "TurboGrafx-16".

higan is actively supported on
FreeBSD 10 and above, and
Microsoft Windows 7 and above.
It also includes some level of support
for GNU/Linux and macOS.

If you want to install higan and try it out,
see the [Quick Start](#quick-start) section below.

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
    Its Super Famicom emulation is based on bsnes v059,
    from the time before bsnes was renamed to higan.
  - [BizHawk](http://tasvideos.org/BizHawk.html)
    is another multi-system emulator,
    specialising in the creation of
    tool-assisted speedruns.
    Its Super Famicom emulation is based on bsnes v087.
  - [nSide](https://github.com/hex-usr/nSide)
    is a fork of higan that greatly enhances
    its NES emulation support,
    and adds minor features to the other cores too.
    It also restores the "balanced" Super Famicom emulation core
    that was removed from higan in v099,
    which is less CPU intensive
    than the current accuracy-focussed core.
  - [bsnes-plus](https://github.com/devinacker/bsnes-plus)
    is a fork of bsnes v073
    that adds improved support for debugging Super Famicom software.

Quick Start
===========

TODO

- install
- configure inputs
- load a game
- connect a controller

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
extract the contents of the higan archive into a new folder.

When you're done,
the new folder should contain `higan.exe` and `icarus.exe`
along with other assorted files and directories
that describe the systems higan emulates.

You may put that folder wherever you like.

To run higan, open the `higan.exe` file.

Before you can actually play games,
you'll need to [import them](#the-game-library)
and [configure higan](#configuring-higan).
If you want to play Game Boy Advance games,
you will need [a GBA BIOS](#installing-the-gba-bios).

Uninstalling an official release on Windows
-------------------------------------------

Delete the folder containing `higan.exe`
and the other associated data from the original archive.

To remove higan's configuration:

 1. Press Win+R to open the Run dialog
 2. Type `%LOCALAPPDATA%` and press Enter
    to open the folder where higan's configuration data lives
 3. Delete the subdirectories named `icarus` and `higan`
    if they exist.

You might also want to remove the games imported into higan's library
(including in-game saves and save-states):

 1. Press Win+R to open the Run dialog
 2. Type `%USERPROFILE%` and press Enter
    to open the folder where higan keeps its game library
 3. Delete the folder named `Emulation` if it exists

Compiling from source on Windows
--------------------------------

You will need a copy of the higan source-code.
If you download an official release from the higan homepage,
you will need [7-zip][7z] or a compatible tool to extract it.
Alternatively,
you may obtain higan source code from
[the unofficial git repo](https://gitlab.com/higan/higan/)
using the Git source-code management tool,
or by clicking the download button on the right-hand side of the web-page
and choosing an archive format.

You will need a C++ compiler to compile higan.
We recommend installing [TDM64-GCC][tdm],
preferably the latest version
but anything newer than 4.9 should be fine.
higan does not support building with clang++
(Clang is still not quite there yet for Windows)
nor Microsoft Visual C++
(last we checked, it didn't support all the C++ features higan uses).

**Note:** Make sure you get TDM64-GCC,
not TDM-GCC.
When compiled in x86 (32-bit) mode,
higan may crash at startup
because gcc targeting x86 does not support
Windows' structured exception handling (SEH).
Also,
historically in x86 mode
gcc has miscompiled a part of the NES emulation core.
See the higan forum
[for](https://board.byuu.org/viewtopic.php?p=41977#p41977)
[details](https://board.byuu.org/viewtopic.php?p=42253#p42253).

Once you've installed mingw-w64,
open a command-prompt window,
type `g++ --version`
then press Enter
to check it's installed correctly.
You should see a message like

    g++ 1.2.3 20010101
    Copyright (C) 2001 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

...except it should mention the version of mingw that you installed
and the corresponding dates.
If you see an error message like "command not found"
or "bad command or filename",
you may need to add mingw's "bin" folder
to your computer's `%PATH%`.
See the mingw documentation for help with that.

Once mingw is installed and available from the command prompt:

 1. Put the higan source code in some convenient location,
    like `C:\higan-src`
 2. Open the command-prompt
 3. Type `cd C:\higan-src`
    (or wherever you put the higan source)
    and press Enter
 4. Type `mingw32-make -C icarus compiler=g++` and press Enter
    to build the icarus import tool
 5. Type `mingw32-make -C higan compiler=g++` and press Enter
    to build the main higan executable

[tdm]: http://tdm-gcc.tdragon.net/download

Installing a compiled build on Windows
--------------------------------------

 1. In Windows Explorer,
    create the folder where you want higan to live
 2. Assuming you built higan in `C:\higan-src`,
    copy `C:\higan-src\icarus\out\icarus.exe`
    into the new folder
 3. Copy `C:\higan-src\icarus\Database` and its contents
    into the new folder
 4. Copy `C:\higan-src\higan\out\higan.exe`
    into the new folder
 5. Copy all the `*.sys` directories
    in `C:\higan-src\higan\systems`
    into the new folder

The new folder should now contain
`icarus.exe`,
`higan.exe`,
a folder named `Database`,
and half a dozen folders named after the systems higan emulates
with `.sys` at the end.
This is what you would get by downloading an official build,
as described under
[Installing an official release on Windows][instwin]
above.

[instwin]: #installing-an-official-release-on-windows

Before you can actually play games,
you'll need to [import them](#the-game-library)
and [configure higan](#configuring-higan).
If you want to play Game Boy Advance games,
you will need [a GBA BIOS](#installing-the-gba-bios).

Uninstalling a compiled build on Windows
----------------------------------------

The process is the same as
[Uninstalling an official release on Windows][uninstwin]
above. You may also wish to delete the higan source folder.

[uninstwin]: #uninstalling-an-official-release-on-windows

Compiling from source on Linux
------------------------------

You will need a copy of the higan source-code.
If you download an official release from the higan homepage,
you will need [7-zip][7z] or a compatible tool to extract it.
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

Once you have all the dependencies installed,
you may build and install higan.

Note: Run these commands as yourself,
**do not run them as root**
(no `sudo`, no `su`, etc.),
because higan does not support
being installed system-wide.

 1. Put the higan source code in some convenient location,
    like `~/higan-src`
 2. Open a terminal window
 3. Type `cd ~/higan-src`
    (or wherever you put the higan source)
    and press Enter
 4. Type `make -C icarus compiler=g++` and press Enter
    to build the icarus import tool
 5. Type `make -C higan compiler=g++` and press Enter
    to build the main higan executable

Installing a compiled build on Linux
------------------------------------

Assuming you have successfully compiled higan
as described in the previous section:

 1. Open a terminal window
 2. Type `cd ~/higan-src`
    (or wherever you put the higan source)
    and press Enter
 3. Type `make -C icarus install` and press Enter
    to install icarus and its game database
 4. Type `make -C higan install` and press Enter
    to install higan and its supporting files

This installs higan and its associated data files
into the `~/.local` directory hierarchy.

To confirm higan is installed correctly,
type `higan` in a terminal and press Enter.
If the higan window appears,
everything is working.
On the other hand,
if you get an error message like "command not found",
you should double-check that the directory `~/.local/bin`
is included in your `$PATH` environment variable
by running the following command in a terminal:

    echo "$PATH" | tr ':' '\n' | grep ~/.local/bin

If the above command prints the full path of `~/.local/bin`
(for example: `/home/yourname/.local/bin`)
then you should be good.
If it prints nothing,
you need to add the following line to `~/.profile`:

    export PATH=~/.local/bin:$PATH

(this line must be in `~/.profile` because
most GUIs do not read any other files at login)

If you also have a `~/.bash_profile`,
make sure it reads the contents of `~/.profile`
with a line like this:

    source ~/.profile

You will need to log out and log back in
for changes to `~/.profile` or `~/.bash_profile`
to take effect.

Before you can actually play games,
you'll need to [import them](#the-game-library)
and [configure higan](#configuring-higan).
If you want to play Game Boy Advance games,
you will need [a GBA BIOS](#installing-the-gba-bios).

Uninstalling a compiled build on Linux
--------------------------------------

To uninstall higan,
as installed by the above instructions:

 1. Open a terminal window
 2. Type `cd ~/higan-src`
    (or wherever you put the higan source)
    and press Enter
 3. Type `make -C icarus uninstall` and press Enter
 4. Type `make -C higan uninstall` and press Enter

To remove higan's configuration,
delete the directory `~/.config/higan` as well.

To remove the games imported into higan's library
(including in-game saves and save-states),
delete the directory `~/Emulation`.

You may also wish to delete the higan source directory.

Installing the GBA BIOS
-----------------------

For most of the systems higan emulates,
the console itself contains (almost) no actual software,
so emulating the system does not require
infringing the copyright of the hardware manufacturer.
However,
the Game Boy Advance is different:
every device contains a standard library of software routines
for [common functions games require][bios],
often called a "BIOS"
by analogy with the Basic Input/Output System
used in IBM PC compatibles.

For the same legal reasons that commercial games
cannot be distributed with emulators,
the GBA BIOS cannot be distributed with higan,
but is required for GBA software to run.

If you have a real GBA and a flashcart,
the Internet contains many tools
that will extract the BIOS image so it can be copied
to your desktop computer.
The correct GBA BIOS file is exactly 16384 bytes long,
and has the SHA-256 hash
fd2547724b505f487e6dcb29ec2ecff3af35a841a77ab2e85fd87350abd36570.

Once you have the correct BIOS file:

 1. rename it to `bios.rom`
      - if you're using Windows,
        turn off "hide extensions for known file types"
        so you don't wind up with a file called
        `bios.rom.dat`
        or whatever the file's original extension was.
 2. Copy the file into higan's `Game Boy Advance.sys` directory,
    alongside the `manifest.bml` file that is already there.
      - In Windows,
        find `Game Boy Advance.sys` in the same folder
        as `higan.exe`
      - In Linux,
        find `Game Boy Advance.sys` in
        `~/.local/share/higan/`

**Note:**
If you upgrade this version of higan to a newer version,
make sure the `bios.rom` file
winds up in the `Game Boy Advance.sys` directory
of the new version.

[bios]: http://problemkaputt.de/gbatek.htm#biosfunctions

The higan interface
===================

When you launch higan,
the main window appears,
with a menu-bar across the top,
a status-bar across the bottom,
and a large area in the middle where the game's video output appears.

The Library menu
----------------

The Library menu allows you
to import games into higan's game library,
and to load games from the library.
higan organises the games in your library
according to which console they were intended to run on.

To play a game for a particular console from your library,
click on the Library menu,
click on the console manufacturer submenu
(Nintendo for the Super Famicom,
Bandai for the WonderSwan,
etc.)
then click on the console menu item.
A window will appear listing all the games in your library
for that particular console.
Select the game you want to play
and click the Open button,
or just double-click the game,
and it will begin playing as though you'd just turned on the console.

To add a new game to your library,
choose "Load ROM File ..." from the Library menu.
A [filesystem browser](#the-filesystem-browser) will appear,
allowing you to pick any ROM image for any supported system,
with any of the most common file extensions.
It also allows loading ROM images from `.zip` archives,
if the archive contains a single ROM image.

**Note:** Some games require extra steps to import correctly;
see [the Game Library](#the-game-library) for details.

To add many games at once,
run icarus,
or choose "Import ROM Files ..." from the Library menu
(which just runs icarus anyway).
See [the icarus interface](#the-icarus-interface)
for more information about bulk-importing.

For more information about the higan game library,
see [The Game Library](#the-game-library) below.

The console menu
---------------

**Note:**
The console menu does not appear
until a game is loaded.
Also,
it's not named "console",
it's named for the kind of console
the loaded game runs on.
For example,
when playing a Game Boy game,
you will have a "Game Boy" menu.

The console menu contains commands relevant
to the particular console being emulated.
All consoles will have some of the following items,
but few consoles have all of them.

  - **Controller Port 1**
    allows you
    to connect different emulated controllers
    to the first controller port,
    if there is one.
      - See [the Configuration dialog](#the-configuration-dialog)
        for information about configuring
        which host controller inputs are used
        for the emulated controllers.
      - This menu appears for the Famicom,
        even though the Famicom did not support alternate controllers,
        because the Famicom emulation core also emulates the NES,
        which did.
  - **Controller Port 2**
    allows you
    to connect different emulated controllers
    to the second controller port,
    if there is one.
      - See [the Configuration dialog](#the-configuration-dialog)
        for information about configuring
        which host controller inputs are used
        for the emulated controllers.
      - This menu appears for the Famicom,
        even though the Famicom did not support alternate controllers,
        because the Famicom emulation core also emulates the NES,
        which did.
  - **Expansion Port**
    allows you
    to connect different emulated devices
    to the console's expansion port,
    if there is one.
      - For the Super Famicom,
        the [21fx][21fx] is a homebrew device
        that allows a program running on a PC
        to control a physical Super Famicom (or SNES).
        This option allows the same program
        to control the emulated SNES,
        for development or testing.
  - **Power Cycle**
    restarts the loaded game
    as though the emulated console were switched off and on again.
  - **Unload**
    stops the current game,
    as though the emulated console were switched off.
    You can load the same or a different game
    from [the Library menu](#the-library-menu).

[21fx]: https://github.com/defparam/21FX

The Settings menu
-----------------

The Settings menu allows you to configure things
that aren't specific to any particular console.

  - **Video Scale** determines the size and shape
    of the emulated console's video output
    in windowed mode
    (as opposed to fullscreen).
  - **Video Emulation** applies various effects
    to the emulated console's video output
    to reproduce some behaviours
    that aren't technically part of the console itself.
      - "Blurring" simulates the limited horizontal resolution
        of standard-definition TVs
        by blurring together horizontally-adjacent pixels.
        Games like Jurassic Park for the Super Famicom
        depend on this to emulate a transparency effect.
        For hand-held consoles like the Game Boy Advance,
        this simulates the slow response time
        of the cheap LCD screens these consoles used
        by blurring each output frame with the previous one.
      - "Colors" simulates the way a console's display device
        differs from modern computer monitor's colour reproduction.
        In particular,
        it simulates the slightly-different gamma correction
        used by the Super Famicom,
        the dim, washed out colours of the original Game Boy Advance,
        and the pea-green display of the original Game Boy.
  - **Video Shader** controls
    how the low-resolution video output of the emulated console
    is scaled up to suit modern high-resolution displays.
    The availability of items in this submenu depends on
    which video driver higan is using,
    so see [Drivers](#drivers) for more information.
      - "None" draws each output pixel according to
        the colour of the single nearest input pixel,
        sometimes called "nearest neighbour" scaling.
        This produces unnaturally crisp and blocky images.
      - "Blur" draws each output pixel by
        averaging the colours of the four nearest input pixels,
        sometimes called "bilinear" scaling.
        This produces unnaturally blurry images.
      - When using the OpenGL [driver](#drivers),
        an additional item appears in this menu for
        each installed Quark shader.
        See [Installing custom shaders](#installing-custom-shaders)
        for details.
  - **Synchronize Audio**
    causes higan to wait for audio playback to complete
    before resuming emulation.
    This should reduce popping and glitching noises,
    but if your PC's video refresh-rate and audio-playback rate
    are in a different ratio than the emulated console,
    this may increase them.
    If your PC cannot emulate at full-speed,
    (60fps for most consoles, 75fps for WonderSwan)
    this has no noticable effect.
  - **Mute Audio**
    causes higan to not output sound from the emulated console.
    The sound hardware is still emulated.
  - **Show Status Bar**
    causes higan to show or hide the status bar
    at the bottom of the window.
    This option has no effect in full-screen mode.
    See [The status bar](#the-status-bar) for more information.
  - **Configuration ...**
    opens [the Configuration dialog](#the-configuration-dialog)

The Tools menu
--------------

The Tools menu
contains features for manipulating the emulated console.

  - **Save Quick State**
    stores the current state of the emulated console
    into one of the quick state slots.
    See [Save States](#save-states) for more information.
  - **Load Quick State**
    restores the emulated console to
    a state previously saved to one of the quick state slots.
    See [Save States](#save-states) for more information.
  - **Cheat Editor**
    opens [the Cheat Editor window](#the-cheat-editor)
  - **State Manager**
    opens [the State Manager window](#the-state-manager)
  - **Manifest Viewer**
    opens [the Manifest Viewer window](#the-manifest-viewer)

The Help menu
-------------

The Help menu contains information about higan itself.

  - **Documentation**
    loads the official higan documentation
    in your web-browser.
  - **About**
    opens the About dialog,
    which displays basic information about higan,
    including the version number.

The status bar
--------------

The status bar appears
at the bottom of the main higan window,
while "Show Status Bar" is ticked in [the Settings menu](#the-settings-menu).

Before any game is loaded,
the status bar displays "No cartridge loaded".

When a game is loaded and running,
the status bar displays the current emulation speeed
in frames-per-second.
For PAL-based consoles,
this should be around 50 FPS for "full speed" emulation,
for NTSC and most portable consoles the ideal speed is 60 FPS,
but the WonderSwan runs at 75 FPS.
If the number is too low,
you may need a faster computer,
or a faster [video driver](#drivers).
If the number is too high,
you may need to [Synchronize Audio](#the-settings-menu),
or you may have pressed the "turbo" [hotkey](#the-configuration-dialog).

The status bar displays "Paused"
if you have pressed the "pause" [hotkey](#the-configuration-dialog),
or if "When focus is lost: Pause Emulation" is ticked
in [the Input tab of the Configuration dialog](#the-configuration-dialog)
and the main higan window is not the foreground window.
To resume emulation,
make sure the main higan window is in the foreground,
and/or press the "pause" hotkey.

The status bar briefly displays "Selected quick state slot X"
(where X is one of the Quick State slot numbers)
when you press the "Increment Quick State"
or "Decrement Quick State"
hotkeys,
to show which Quick State slot will be used
the next time you press the "Save Quick State"
or "Load Quick State" hotkeys.

The status bar briefly displays "Slot X quick state does not exist"
(where X is one of the Quick State slot numbers)
when you choose a slot from the
[Tools](#the-tools-menu) → "Load Quick State"
sub-menu that has not had a save-state saved to it,
or when you press the "Load Quick State" hotkey
while the current Quick State slot has not had a save-state saved to it,

The status bar briefly displays "Power cycled"
when you choose "Power Cycle" from [the console menu](#the-console menu),
or press the "Power Cycle" hotkey.

The status bar briefly displays "Display rotation not supported"
when you press the "Rotate Display" hotkey
while the emulated console does not support display rotation.

The Configuration dialog
-----------------------

The Configuration dialog
contains less-frequently-modified configuration options.
Most of these can be safely ignored,
or set once and never changed again.

The dialog has a tab for each main category of options:

  - **Video**: This tab contains options that affect
    how higan displays
    the emulated console's video output.
      - "Saturation" adjusts the vibrancy of colours displayed,
        where 0% makes things pure grey,
        100% is normal,
        and 200% is garishly brightly coloured.
      - "Gamma" adjusts how bright mid-range colours are
        compared to the brightest colours,
        where 100% is normal,
        and 200% makes mid-range colours much darker.
      - "Luminance" adjusts the overall brightness,
        where 100% is normal,
        and 0% is totally black.
      - "Overscan Mask" hides parts of
        the video output that would have been hidden
        by the bezel around the edge of
        a standard-definition television screen.
        Some games (particularly on the Famicom)
        displayed random glitchy output in this area,
        which can be distracting.
        The units are "pixels in the emulated console's standard video-mode".
        For example, setting "Horizontal" to 8
        will clip 8/256ths from the left and right sides
        of the Super Famicom's video output,
        whether the Super Famicom is in
        lo-res (256px) or hi-res (512px)
        mode.
      - "Aspect Correction"
        (in both Windowed Mode and Fullscreen Mode)
        stretches the image to match the aspect ratio
        produced by the original console hardware,
        but can cause a "ripple" effect,
        due to rounding errors.
      - "Resize Window to Viewport"
        (under "Windowed mode")
        causes higan to resize its window
        to fit snugly around the emulated console's video
        whenever it changes size:
        because a game was loaded for a different console
        with a different display size or aspect ratio,
        because the "Overscan Mask" controls were adjusted,
        because the game switched to a different video mode,
        because the user pressed the "Rotate Display" hotkey,
        etc.
        When this option is disabled,
        the higan window stays at a fixed size,
        large enough to contain the video for any supported console,
        padded with black borders for all smaller video modes.
      - "Resize Viewport to Window"
        (under "Fullscreen mode")
        causes higan to stretch the emulated console's video output
        to touch the edges of the screen.
        Since most screens are not an exact multiple
        of the size of all emulated consoles,
        this may cause a "ripple" effect,
        due to rounding errors.
        When this option is disabled,
        higan stretches the emulated console's video output
        to the largest exact multiple
        of the emulated console's video output
        that is smaller than or equal to the screen size.
      - TODO: Update this to match 103r11, or whatever the latest version is.
  - **Audio**: This tab contains options that affect
    how higan reproduces
    the emulated console's audio output.
      - "Latency" controls how much audio output higan calculates in advance.
        Higher values reduce the chance of
        "popping" or "glitching" noises,
        but increase the delay between an action occurring on-screen
        and the corresponding sound-effect being played.
      - "Frequency" controls the sample-rate that higan will use
        when generating audio.
        If your PC's audio hardware has a "native" sample-rate
        and you know what it is,
        pick that.
        Otherwise,
        44.1kHz or 48kHz should be fine.
      - "Resampler" selects the algorithm higan uses
        to convert the console's native audio sample-rate
        to the value selected for "Frequency" above.
      - "Exclusive Mode" appears
        if the current audio driver
        allows higan to take exclusive control of your PC's audio output,
        so no other applications can play sounds.
        This can improve audio quality,
        and lower the effective audio latency.
      - "Volume" controls the overall loudness of
        the emulated console's audio,
        where 100% is normal volume,
        and 0% is complete silence.
      - "Balance" controls the relative loudness of
        the left and right speakers,
        where 0% means only the left speaker produces sound,
        50% means both speakers produce sound equally,
        and 100% means only the right speaker produces sound.
      - "Reverb" adds a slight reverberation effect
        to the emulated console's audio output,
        as though the console were in a tunnel or small room.
  - **Input**: This tab controls which PC inputs
    are used for which emulated controllers.
    The exact PC inputs that can be mapped
    depend on [the input driver](#drivers).
      - "Pause Emulation" automatically pauses emulation
        when the main higan window
        is not the current foreground window.
      - "Allow Input" can be ticked
        when "Pause Emulation" is *not* ticked,
        and allows configured inputs to keep affecting higan
        even when higan is running in the background.
        This is particularly relevant if
        you configure your PC keyboard to control higan:
        if you tick this box,
        and switch to a different application
        leaving higan running in the background,
        typing in that other application may affect
        the emulated game running in higan
        even though you can't see it!
      - The console selector chooses which console's inputs
        to display in the mapping list below.
      - The port selector chooses which port of the selected console
        to display in the mapping list below.
      - The controller selector chooses which controller
        associated with the given console and port
        to display in the mapping list below.
      - The mapping list includes
        every button and axis on the selected controller,
        and the PC inputs that are mapped to it
        when it is connected to the selected port of the selected console.
      - To map
        a keyboard or gamepad button on your PC to
        a controller button,
        double-click the controller button in the list,
        or select it and press Enter.
        The window will grey out,
        and a message will appear in the bottom left:
        "Press a key or button to map [the button]".
        Press the key or button you want to map,
        and it should appear in the list
        next to the controller button it is mapped to.
      - To map
        a mouse button on your PC to
        a controller button,
        select the controller button in the list,
        then click one of the "Mouse Left",
        "Mouse Middle",
        or "Mouse Right" buttons in the bottom-left of the window.
      - To map
        a joystick axis on your PC to
        a controller axis,
        double-click the axis in the list,
        or select it and press Enter.
        The window will grey out,
        and a message will appear in the bottom left:
        "Press a key or button to map [the axis]".
        Press the joystick in the direction you want to map,
        and it should appear in the list
        next to the controller button it is mapped to.
      - To map
        a mouse axis on your PC to
        a controller axis,
        select the axis in the list,
        then click one of the
        "Mouse X-axis",
        or "Mouse Y-axis"
        buttons in the bottom-left of the window.
      - The "Rumble" setting
        for the Game Boy Advance is treated like a button,
        and can be mapped to a PC gamepad.
        When the emulated Game Boy Advance
        tries to use the rumble feature
        of the Game Boy Player,
        higan will turn on the force-feedback
        of whatever gamepad the mapped button is part of.
      - If you start mapping a button or axis,
        but decide you don't want to,
        you can press Escape
        to exit the "Press a key or button to map..." mode
        without actually mapping anything.
      - "Erase" removes the mapping
        for the selected button or axis.
      - "Reset" removes all the mappings currently in the list.
  - **Hotkeys**: This tab is like "Inputs" above,
    except it contains controls for higan itself,
    instead of for the emulated console.
      - "Toggle Fullscreen" puts higan into fullscreen mode,
        where the menu and status bar are hidden,
        and the emulated console's video output
        is enlarged to cover the entire screen.
        Toggling fullscreen also automatically captures the mouse.
      - "Toggle Mouse Capture" hides the usual mouse-cursor,
        and captures the mouse so it cannot leave the higan window.
        This is useful when the mouse is being used to emulate
        a light-gun controller like the Super Scope.
      - "Save Quick State" saves the current state of the emulated console
        to the currently-selected Quick State slot.
      - "Load Quick State" restores the emulated console
        to the state saved in the currently-selected Quick State slot.
      - "Decrement Quick State" selects the previous Quick State slot.
        The status bar will briefly display the new current slot number.
      - "Increment Quick State" selects the next Quick State slot.
        The status bar will briefly display the new current slot number.
      - "Pause Emulation" pauses the emulated console
        until the Pause Emulation hotkey is pressed a second time.
      - "Fast Forward" disables audio and video synchronisation
        for as long as it's held down,
        so emulation proceeds as quickly as possible.
        If your PC struggles to hit "real time"
        (60fps for most emulated consoles),
        this likely won't have any effect.
      - "Power Cycle" turns the emulated console off and back on,
        (a "hard reset"),
        just like the "Power Cycle" menu item
        in [the console menu](#the-console-menu).
      - "Rotate Display" will toggle the display
        of the Game Boy Advance
        and WonderSwan (Color)
        between the usual landscape orientation
        and a portrait orientation (90° counter-clockwise).
        These consoles have games
        that expect the player to hold the console
        in a different way.
  - **Advanced**: This tab contains all the settings
    that didn't fit into one of the other categories.
      - "Video" controls how higan will draw
        the emulated console's video output
        to the PC screen.
        "None" means no video will be drawn.
        See [Drivers](#drivers) for details.
      - "Audio" controls how higan will present
        the emulated console's audio output.
        "None" means no audio will be played.
        See [Drivers](#drivers) for details.
      - "Input" controls how higan checks for input
        from the PC's input devices.
        "None" means the emulated console cannot be controlled.
        See [Drivers](#drivers) for details.
      - "Location" selects where the [Game Library](#the-game-library)
        looks for games to load.
        See [Moving the Game Library](#moving-the-game-library)
        for more information.
      - "Ignore Manifests" makes higan ignore the manifest file
        in the a loaded game's [game folder](#why-game-folders)
        in favour of asking icarus
        to guess a manifest on the fly.
        This means that incompatible or incorrect manifests
        generated by old versions of icarus
        won't cause problems,
        but means you can't fix incorrect manifests
        generated by the current version of icarus.
        See also the "Create Manifests" option in
        [the icarus Settings dialog](#the-icarus-settings-dialog).

The Cheat Editor
----------------

For some consoles,
higan supports applying temporary changes to the code of a running game.
For example,
you could disable the code that registers when the player takes damage,
resulting in an "invulnerability" mode.
Currently,
higan supports cheats for the following consoles:

  - Famicom
  - Super Famicom
  - Game Boy
  - Master System
  - PC Engine
  - Wonder Swan

A cheat code of the format `addr=data`
will cause the emulated console to obtain `data`
whenever it reads from memory address `addr`.
A cheat code of the format `addr=comp?data`
will cause reads from `addr` to obtain `data`,
but only if the true value at `addr` is `comp`.
In both formats,
`data` is a single byte expressed as two hexadecimal digits,
`comp` is also a single byte expressed as two hexadecimal digits,
and `addr` is a memory address in the emulated console,
expressed as however many hexadecimal digits are required
for the console in question
(typically 4 for 8-bit CPUs,
6 for 16-bit CPUs,
and 8 for 32-bit CPUs).

For compatibility with older versions of higan,
the older syntaxes of `addr/data` and `addr/comp/data`
are still supported.

For cheats that require more than a single-byte change,
higan allows multiple codes to be combined with `+`
so that all of them can have a single description
and be toggled with a single click.
For example,
in Super Mario World,
you can lock the time to 999 with these codes:
`7e0f31=09+7e0f32=09+7e0f33=09`.

Changes made in the Cheat Editor are saved to disk
when the game is unloaded,
or when higan exits.
higan stores the known cheats for a particular game
in `higan/cheats.bml`
inside the corresponding game folder
in [the Game Library](#the-game-library).

If your copy of higan includes a cheat database
(a file named `cheats.bml`
in the same directory as `Super Famicom.sys`
and the other `*.sys` directories),
you can click the "Find Codes ..." button in the bottom left
to load all known cheats for the currently-running game.

To add a new cheat,
select an unused row in the list,
then type the relevant codes in the "Code(s)" field at the bottom,
and a description in the "Description" field.

To enable or disable an existing cheat,
tick the checkbox in the first column of the list.
The code should take effect immediately.

To clear out an existing cheat,
select it from the list
and click the "Erase" button in the bottom right,
or just manually delete
the contents of the "Code(s)" and "Description" fields.

To clear out all existing cheats,
click the "Reset" button in the bottom right.

The State Manager
-----------------

The State Manager allows you to create,
load,
and remove Manager states.
For more information on Manager states,
quick states,
saved games
and how they compare,
see [Save States](#save-states).

To create a new manager state,
or to replace an existing one,
select the slot in the list
then click "Save" in the bottom-left corner.
You can then type a description in the "Description" field,
to help you find the state again later.

To rename a state,
select the slot in the list
and edit the "Description" field.

To load a state,
select the slot in the list
and click "Load" in the bottom-left corner,
or just double-click it.

To clear the state out of a slot,
select the slot in the list
and click "Erase" in the bottom-right corner.

To clear all the slots at once,
click "Reset" in the bottom-right corner.

The Manifest Viewer
-------------------

As mentioned in
[Why game folders?](#why-game-folders),
a game cartridge contains
more than just the raw data of the game.

higan uses a "manifest" to
describe how the various parts of a game cartridge
are wired up together,
and the Manifest Viewer lets you examine
the configuration higan is using for the currently-running game.

For some games,
an actual cartridge has been taken apart and carefully examined
and its configuration has been recorded in icarus' database,
so the manifest icarus produces
is guaranteed accurate.
For games that do not exist in icarus' database,
icarus will make a reasonable guess.
This is enough to get the game running,
but does not necessarily reflect the original cartridge.

The Filesystem Browser
----------------------

Sometimes higan will need
to ask you to choose a file or folder.
For this, it uses a special Filesystem Browser dialog.
Although many operating systems provide a native filesystem browser,
they do not all allow the same customizations.
Therefore,
higan provides its own filesystem browser
that works the same way on every platform.

The filesystem browser shows the contents of some particular folder,
and allows you to select one of those items.

Across the top of the window,
a text-box displays the path of the current folder.
If you want to browse a specific path,
you may edit the contents of this box
and press Enter to switch to the new location.

The button with two blue arrows at the top-right is "Refresh".
Pressing this button will check for
added (or removed) items in the current folder,
and add (or remove) them to (or from) the list.

The button with the house at the top-right is "Home".
Pressing this button will switch to your home folder.

The button with the green up-arrow at the top right is "Parent".
Pressing this button will
switch to the parent of the current folder.

Most of the filesystem browser lists the contents
of the current directory.
Double-clicking a folder,
or selecting it and pressing Enter,
will switch to showing the contents of that directory.
If the list has keyboard focus,
typing any text will jump to the first inem in the list
whose name begins with the text you typed.

If a drop-down list appears in the bottom-left,
it allows you to choose which files appear in the list,
based on file-extension.

If this filesystem browser is asking for a file,
you can choose one
by double-clicking it,
by selecting it and pressing Enter,
or by selecting it and clicking the "Select" button in the bottom-right.

If this filesystem browser is asking for a directory,
you can choose one
by selecting it and clicking the "Select" button in the bottom-right.
Double-clicking
or selecting and pressing Enter don't work,
they just switch to viewing that directory.

The "Cancel" button in the bottom-right
closes the file-system browser without selecting anything.

The icarus interface
--------------------

When launching icarus,
directly or by picking "Import ROM Files ..."
from higan's [Library menu](#the-library-menu),
the main icarus window appears.
This is [a filesystem browser](#the-filesystem-browser),
with customisations:

  - The filesystem browser only lists
    files with extensions typically used for ROM dumps from
    consoles higan emulates,
    plus `.zip` files since ROM dumps are often compressed.
  - Each matching file has a check-box next to it.
  - You can tick the check-box next to every file at once
    by pressing "Select All" in the bottom-left.
  - You can un-tick all the check-boxes
    by pressing "Unselect All" in the bottom-left.

Pressing "Settings ..." in the bottom-right
opens [the icarus Settings dialog](#the-icarus-settings-dialog).

Pressing "Import ..." in the bottom-right
will close the filesystem browser
then try to import all the files
whose check-boxes are ticked
into [the Game Library](#the-game-library).
icarus displays a progress dialog during the import process.

**Note:** Some games require extra steps to import correctly;
see [the Game Library](#the-game-library) for details.

The icarus Settings dialog
--------------------------

The icarus Settings dialog contains the following settings:

  - **Library Location** determines
    where icarus puts the games it imports.
    See [Moving the Game Library](#moving-the-game-library)
    for details.
  - **Create Manifests** causes icarus
    to write out a manifest file describing
    each imported game
    to that game's [game folder](#whats-in-a-game-folder).
    This means that higan doesn't have to regenerate
    the manifest each time an imported game is loaded,
    but it means that a future version of higan
    with an incompatible manifest format
    may be unable to play these games.
    Note that higan also has an "Ignore Manifests" option
    in the Advanced tab of
    [its Configuration dialog](#the-configuration-dialog).
  - **Use Database** causes icarus to use manifest information
    from its database of known-good manifests,
    if it's importing a game it recognises.
    For unrecognised games,
    and for all games if this box is unticked,
    icarus gueses the manifest data.
    This option is still relevant when "Create Manifests" is unticked:
    higan uses icarus to generate a manifest when a game is loaded,
    not just at import-time.

The Game Library
================

higan maintains a "game library"
containing all the games you've played.

  - In Windows,
    the default location of
    the game library is the `Emulation` folder
    inside your profile folder
    (To find your profile folder,
    press `Win+R` to open the Run dialog,
    then type `%USERPROFILE%` and press Enter).
  - In Linux,
    the default location of
    the game library is the `Emulation` directory
    inside your home directory.
  - On all platforms,
    the game library location can be configured.
    See [Moving the Game Library](#moving-the-game-library)
    below.

Inside the library directory there is a subdirectory for each system,
and inside each system directory are the game folders
for each imported game.

For more information about game folders,
see [Why game folders?](#why-game-folders)
and [What's in a game folder?](#whats-in-a-game-folder)
below.

Importing and playing regular games
-----------------------------------

icarus supports importing games
in the most commonly-used formats
for each supported console,
and also those same formats inside `.zip` files.
More advanced compression formats
like RAR or 7-zip are not supported.

For most games
that do not use special chips or co-processors,
importing a game is straight-forward.
From [the Library menu](#the-library-menu)
choose "Load ROM File ..."
to open [a filesystem browser](#the-filesystem-browser),
choose the game you want to play,
and it will be imported into the library and loaded.

To play the game again
select the console the game runs on from
[the Library menu](#the-library-menu)
to open another [filesystem browser](#the-filesystem-browser)
that lists all the previously-imported games for that platform.

Importing and playing games with co-processor firmware
------------------------------------------------------

Many games included extra chips inside the game cartridge,
to provide enhanced capabilities of one kind or another.
Sometimes,
those extra chips were separate CPUs
running their own separate firmware,
and for those cases
higan requires a copy of the co-processor firmware
as well as the actual game.
Unfortunately,
like games themselves,
co-processor firmware cannot legally be distributed,
so you'll need to obtain
copies of the relevant firmware data
yourself.

To import a game that requires co-processor firmware,
you must copy the required firmware files
beside the game you want to import.
For example,
if you want to import Megaman X2,
which is stored in the file `mmx2.sfc`,
the file `cx4.data.rom`
must be placed in the same folder
for the import to succeed.

Wikipedia [lists which Super Famicom games use which co-processors][wpec],
although not all co-processors require separate firmware.
Once you've figured out which co-processor
(if any)
is used by the game you want to import,
here's the firmware files you'll need:

[wpec]: https://en.wikipedia.org/wiki/List_of_Super_NES_enhancement_chips#List_of_Super_NES_games_that_use_enhancement_chips

<table>
    <thead>
        <tr>
            <th>Co-processor</th>
            <th>Filename</th>
            <th>Size (bytes)</th>
            <th>SHA256</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <th scope="row">CX4</th>
            <td><code>cx4.data.rom</code></td>
            <td>3072</td>
            <td><code>ae8d4d1961b93421ff00b3caa1d0f0ce7783e749772a3369c36b3dbf0d37ef18</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan=2>DSP1/1A<br><sup>See Note 1</sup></th>
            <td><code>dsp1.data.rom</code></td>
            <td>2048</td>
            <td><code>0b5da6533e55852ee8fc397977ec5576c5b9f1fb2e05656d8f87123a121b076e</code></td>
        </tr>
        <tr>
            <td><code>dsp1.program.rom</code></td>
            <td>6144</td>
            <td><code>269584b347a22953a2989494c850a7c1c027f4ca5add517a60e0c7d8833d0fac</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan=2>DSP1B<br><sup>See Note 2</sup></th>
            <td><code>dsp1b.data.rom</code></td>
            <td>2048</td>
            <td><code>8546cbac530830446bb8a277f6b139d4ad64d650bdbac7e4e150e2f095665049</code></td>
        </tr>
        <tr>
            <td><code>dsp1b.program.rom</code></td>
            <td>6144</td>
            <td><code>2eccb54a8f89374911f7e2db48f1b4cde855655e28103f7bda2982a5b418a187</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan=2>DSP2</th>
            <td><code>dsp2.data.rom</code></td>
            <td>2048</td>
            <td><code>3beef9bffdc1e84c9f99f3301d8bd3e520d2e62909a995320f9faeae8f46ec11</code></td>
        </tr>
        <tr>
            <td><code>dsp2.program.rom</code></td>
            <td>6144</td>
            <td><code>62a2ef8d2d7db638f4ec0fbcebf0e5bf18a75ee95be06e885d9519a10487f0da</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">DSP3</th>
            <td><code>dsp3.data.rom</code></td>
            <td>2048</td>
            <td><code>7fe51796e9c97fee1fa2aab40592b7c78997f67dd00333e69d0f79a12f3cb69f</code></td>
        </tr>
        <tr>
            <td><code>dsp3.program.rom</code></td>
            <td>6144</td>
            <td><code>aea7b622e7c1de346cb15d16afcbedf92b6798507e179f83ed2a4cff40d0e663</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">DSP4</th>
            <td><code>dsp4.data.rom</code></td>
            <td>2048</td>
            <td><code>ef3ffb4256dd896a60213269b4e2d3bdd120c97e2fd623bddabbf43c2be422af</code></td>
        </tr>
        <tr>
            <td><code>dsp4.program.rom</code></td>
            <td>6144</td>
            <td><code>89b1826e6038be3a0ea0f923e85d589ff6f02dc1a1819fb2ec8c0cea5b333dcd</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">ST010</th>
            <td><code>st010.data.rom</code></td>
            <td>4096</td>
            <td><code>dc7056a51b53993d7a8ba5bacf9501f785d2fce5e5be748e9ff2737c5938d4a5</code></td>
        </tr>
        <tr>
            <td><code>st010.program.rom</code></td>
            <td>49152</td>
            <td><code>2c1f74bb5f466d81c64c326e71ac054489efe1abc9a5d6f91aac7747f2ddab67</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">ST011</th>
            <td><code>st011.data.rom</code></td>
            <td>4096</td>
            <td><code>b5377d1bebe8adc507a024b6e2b9b8fdf4877e451da84fbad05dff4e4a70311e</code></td>
        </tr>
        <tr>
            <td><code>st011.program.rom</code></td>
            <td>49152</td>
            <td><code>d90a5cda380e81cb9ba11a9da7539b173c49b31bedc7a3ac9c3c8b3f97e89e14</code></td>
        </tr>
        <tr>
            <th scope="row" rowspan="2">ST018</th>
            <td><code>st018.data.rom</code></td>
            <td>32768</td>
            <td><code>b5377d1bebe8adc507a024b6e2b9b8fdf4877e451da84fbad05dff4e4a70311e</code></td>
        </tr>
        <tr>
            <td><code>st018.program.rom</code></td>
            <td>131072</td>
            <td><code>d90a5cda380e81cb9ba11a9da7539b173c49b31bedc7a3ac9c3c8b3f97e89e14</code></td>
        </tr>
    </tbody>
</table>

**Note 1:**
The DSP1 and DSP1A are physically different,
but the firmware inside is identical.

**Note 2:**
The DSP1B is very similar to the DSP1A,
but has some bugs fixed.
Note that icarus' heuristics cannot distinguish between
a game that uses DSP1
and one that uses DSP1B,
so if it cannot find your game in its manifest database,
it will assume it uses DSP1B.
Many games work just as well with either DSP1 or DSP1B,
but Pilotwings is a notable exception.

If you try to import a game
using the "Import ROM Files ..." option
in [the Library menu](#the-library-menu)
(or using icarus directly)
but do not have the required firmware files
in the correct place,
a window will appear saying
"Import completed, but with 1 errors. View log?"
(or howevery many games were lacking the correct firmware).
If you press "Yes",
a new window will appear listing the games that couldn't be imported,
and at least one firmware file that was missing or incorrect, like this:

> [smk.zip] firmware (dsp1b.program.rom) missing or invalid

If you try to import a game
using the "Load ROM File ..." option
in [the Library menu](#the-library-menu)
but do not have the required firmware files
in the correct place,
nothing will happen,
and higan will just sit there
with "No cartridge loaded" in
[the status bar](#the-status-bar).

Once a game with co-processor firmware is imported,
you can play it just like any [regular game](#importing-and-playing-regular-games).

Importing and playing Satellaview games
--------------------------------

The [Satellaview][wpbsx]
was a satellite modem peripheral
released for the Super Famicom in Japan.
As well as the actual modem
(designed to sit underneath the Super Famicom),
it also included a cartridge
with software to control the modem,
browse online services,
and download games and data.
This control cartridge was called
*BS-X Sore wa Namae o Nusumareta Machi no Monogatari*,
which translates as
*BS-X The Story of The Town Whose Name Was Stolen*.

[wpbsx]: https://en.wikipedia.org/wiki/Satellaview

The control cartridge had a slot that accepted
rewritable "memory paks",
so that people could store the games and data they downloaded.
A small number of games that did not use the Satellaview modem
also had a memory pak slot,
so the game's publishers could
publish extra content for the game
via the Satellaview service
after the game's release.
For the benefit of people who didn't own a Satellaview
some read-only memory paks
were sold in retail stores
containing extra content for specific games.

Importing a game that has a slot for a memory pak
is just like [importing a regular game](#importing-and-playing-regular-games).

Importing a memory pak is like importing a regular game,
but the name of the memory pak file *must* end in `.bs`
(if it's in a `.zip` file,
that's OK,
but the name *inside* the `.zip` file
must end in `.bs`)
in order for it to be successfully imported.
Sometimes memory pak filenames end in `(BSROM).sfc`,
which will make higan try to import them as
regular Super Famicom games,
and fail miserably.
Rename the file and it should work beautifully.

Playing a game that has a slot for a memory pak
is just like playing a regular game,
but after you have selected which game you want to play
higan will open another
[filesystem browser](#the-filesystem-browser)
to let you pick which previously-imported memory pak
you want to insert into the game.
If you press "Cancel" at this point,
the game will load without any cartridge in its memory pak slot.

Note that although you can load
the control cartridge into higan,
higan does not emulate the Satellaview modem itself,
so it's not very useful.
Even if higan did emulate it,
the Satellaview online service
went offline years ago,
so there'd be nothing for the emulated modem to talk to.

Playing a memory pak on its own doesn't make much sense,
it's not a standalone cartridge.
Play a game with a memory pak slot,
and choose which memory pak you want when higan asks for it.

Importing and playing Sufami Turbo games
----------------------------------------

Importing and playing Super Game Boy games
------------------------------------------

Importing and playing MSU-1 games
---------------------------------

Importing and playing Game Boy Color games
------------------------------------------

Importing and playing Game Boy Advance games
--------------------------------------------

Moving the game library
-----------------------

Moving the game library is a little complicated,
because there's two parts to it:
telling icarus where to put imported games,
and telling higan where to find them.

 1. If necessary,
    create the folder you want higan to use
    as its game library.
 1. Launch icarus,
    then click the "Settings ..." button in the lower-right,
    to open the Settings dialog.
 1. Click the "Change ..." button on the right.
    A [filesystem browser](#the-filesystem-browser) window will appear,
    allowing you to choose
    where imported games will be stored.
 1. Launch higan,
    then from the Settings menu,
    choose "Configuration ..."
    to open [the Configuration dialog](#the-configuration-dialog).
 1. Click the Advanced tab
    then click the "Change ..." button.
    A [filesystem browser](#the-filesystem-browser) will appear,
    allowing you to choose the same directory again.

Why game folders?
-----------------

A game is more than just
the raw data originally encased in a game's ROM chip.
If a game allows you to save your progress,
that information needs to be stored somewhere.
If you use an emulator's [save states](#save-states),
those save states need to be stored somewhere.
If you use Game Genie or Pro Action Replay codes,
information about what codes exist,
what codes are enabled,
and what they do
needs to be stored somewhere.

On the technical side,
a physical game cartridge contains a circuit board
that makes the game data available to the console,
and different games used circuit boards that work differently.
That circuit-layout information needs to be stored somewhere.
Some games included custom processors
to do calculations the base console could not do quickly enough
(like the SuperFX chip used in _StarFox_ for the Super Famicom)
and information about extra chips needs to be stored somewhere.
Some of those custom processors require extra data to work
that's not part of the main game data
(like the DSP chip used in Super Mario Kart for the Super Famicom)
and that data needs to be stored somewhere too.

higan keeps all this game-related information together
in a single place:
a game folder in the higan library.

For a more detailed motivation for game folders,
see [Game Paks on the higan website][gp]

[gp]: https://byuu.org/emulation/higan/game-paks

What's in a game folder?
------------------------

TODO

Configuring higan
=================

TODO

Installing custom shaders
-------------------------

Controls
--------

  - mapping PC inputs to emulated controllers
  - configuring which emulated controllers
    are connected to the emulated system

Drivers
=======

TODO

Note that when changing a driver,
you must restart higan for the change to take effect.

Video
-----

TODO

The best option is "OpenGL"
(since it allows you to use
[custom shaders](#installing-custom-shaders)),
and the safest is "Direct Draw" (for Windows)
or SDL (for Linux).

Audio
-----

TODO

On Linux,
"PulseAudioSimple" is the most likely to work.

On Windows, "DirectSound" is probably what you want.
"XAudio" targets XAudio 2.7,
(the last version to work on Windows 7),
so it requires the latest (June 2010) version
of the [DirectX 9 End-User Runtime][dx9]
to be installed.

[dx9]: https://www.microsoft.com/en-us/download/details.aspx?id=35

Input
-----

TODO

On Linux,
"udev" is the most flexble,
but requires a modern Linux system,
while "Xlib" should work on other Unix-like OSs
but only supports a mouse and keyboard.

On Windows,
"Windows" is the only input driver available,
and automatically uses RawInput for keyboard/mouse,
XInput for Xbox controllers,
and DirectInput for other controllers.

Save States
===========

TODO

Save states versus in-game saves
--------------------------------

Quick states
------------

Manager states
--------------

Notes on specific emulation cores
=================================

TODO

The WonderSwan rotates!

Shaders look weird on Super Famicom because of interlace/hi-res

GBA in-game saves are Difficult

PSG volume for the Mega Drive
(see https://board.byuu.org/viewtopic.php?p=42158#p42158 for info)

Frequently Asked Questions
==========================

TODO

where is the vsync option?
https://board.byuu.org/viewtopic.php?p=43305#p43305

exclusive full-screen?

phones and tablets
