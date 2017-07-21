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
