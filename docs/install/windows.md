Installing an official release on Windows
-----------------------------------------

Official higan releases are distributed in
[7-zip](http://www.7-zip.org/)
archives.
You will need to install 7-zip
(or another compatible archiving tool)
to install higan.
Once you have a suitable archiving tool,
extract the contents of the higan archive into a new folder.

When you're done,
the new folder should contain `higan.exe` and `icarus.exe`
along with other assorted files and folders
that describe the systems higan emulates.
You may put that new folder wherever you like.

To run higan, open the `higan.exe` file.

Uninstalling an official release on Windows
-------------------------------------------

Delete the folder containing `higan.exe`
and the other associated data from the original archive.

To remove higan's configuration:

 1. Press Win+R to open the Run dialog
 2. Type `%LOCALAPPDATA%` and press Enter
    to open the folder where higan's configuration data lives
 3. Delete the sub-folders named `icarus` and `higan`
    if they exist.

You might also want to remove
[higan's game library](../concepts/game-library.md#where-is-the-game-library)
(including in-game saves and save-states).

Compiling from source on Windows
--------------------------------

You will need a copy of the higan source-code.
If you download an official release from the higan homepage,
you will need [7-zip](http://www.7-zip.org/)
or a compatible tool to extract it.
Alternatively,
you may obtain higan source code from
[the unofficial git repo](https://gitlab.com/higan/higan/)
using the Git source-code management tool,
or by clicking the download button on the right-hand side of the web-page
and choosing an archive format.

You will need a C++ compiler to compile higan.
We recommend installing the latest version of [MinGW-W64].
higan does not support building with clang++
(Clang is still not quite there yet for Windows)
nor Microsoft Visual C++
(last we checked, it didn't support all the C++ features higan uses).

[MinGW-W64]: https://mingw-w64.org/

MinGW-W64 is available in a number of variants,
and the installer should ask you which you want.

  - **Version:**
    Version 8.1.0 is known to work,
    later versions may work too.
  - **Architecture:**
    You *must* choose "x86_64", not "i686".
    When built with an i686 compiler,
    higan may crash at startup
    because gcc targeting x86 does not support
    Windows' structured exception handling (SEH).
    Also,
    historically in x86 mode
    gcc has miscompiled a part of the NES emulation core.
    See the archive of the official forum
    [for](https://helmet.kafuka.org/byuubackup2/viewtopic.php@f=4&t=1636&start=20.html#p41977)
    [details](https://helmet.kafuka.org/byuubackup2/viewtopic.php@f=4&t=1636&start=30.html#p42253).
  - **Threads:**
    Both options should work,
    but higan is developed with the "posix" model.
  - **Exception:**
    You *must* choose "seh",
    or higan may crash at startup.
    If "seh" is not an option,
    make sure "Architecture" is set to "x86_64".
  - **Build Revision:**
    Choose the largest number, whatever it is.

When the compiler is installed,
it adds a "Run terminal" shortcut to the Start menu
which opens a command-prompt
with all the compiler tools available.

To verify that the compiler is installed correctly,
launch the "Run Terminal" shortcut,
type `g++ --version`
then press Enter.
You should see a message like

```text
g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

...except it should mention the compiler version that you installed
and the corresponding dates.
If you see an error message like
"'g++' is not recognized as an internal or external command,
operable program or batch file",
make sure you're using the "Run terminal" shortcut,
or otherwise have MinGW-W64's "bin" directory in your `%PATH%`.
See the compiler's documentation for help with that.

Once the compiler is installed:

 1. Put the higan source code in some convenient location,
    like `C:\higan-src`
 2. Open the command-prompt
 3. Type `cd C:\higan-src`
    (or wherever you put the higan source)
    and press Enter
 4. Type `mingw32-make -C icarus` and press Enter
    to build the icarus import tool
 5. Type `mingw32-make -C higan target=higan` and press Enter
    to build the main higan executable

Installing a compiled build on Windows
--------------------------------------

 1. In Windows Explorer,
    create the folder where you want higan to live
 2. Assuming you built higan in `C:\higan-src`,
    copy `C:\higan-src\icarus\out\icarus.exe`
    into the new folder
 3. Copy `C:\higan-src\icarus\Database` and its contents
    into the new folder
 4. Copy `C:\higan-src\icarus\Firmware` and its contents
    into the new folder
 5. Copy `C:\higan-src\higan\out\higan.exe`
    into the new folder
 6. Copy `C:\higan-src\higan\systems`
    into the new folder
 7. If the higan source includes a `shaders` folder,
    copy it into the new folder too.

The new folder should now contain
`icarus.exe`,
`higan.exe`,
and folders named `Database`, `Firmware`, `systems`,
and possibly `shaders`.
This is what you would get by downloading an official build,
as described under
[Installing an official release on Windows][instwin]
above.

[instwin]: #installing-an-official-release-on-windows

Uninstalling a compiled build on Windows
----------------------------------------

The process is the same as
[Uninstalling an official release on Windows][uninstwin]
above. You may also wish to delete the higan source folder.

[uninstwin]: #uninstalling-an-official-release-on-windows
