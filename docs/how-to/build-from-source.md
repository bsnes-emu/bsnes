Build From Source
=================

For details on how official builds are made,
see the [GitHub Actions configuration][CI].
If the official builds work for you,
you should use them.
If they don't work for you,
or you want to modify bsnes,
you'll need to build it yourself.

[CI]: https://github.com/bsnes-emu/bsnes/blob/master/.github/workflows/build.yml

Requirements
------------

On a Debian-Linux like system,
you'll need the following packages installed for a default build:

  - `build-essential`
  - `libsdl2-dev`
  - `libgtk-3-dev`
  - `gtksourceview-3.0`
  - `libao-dev`
  - `libopenal-dev`

On FreeBSD,
you'll need these packages:

  - `gmake`
  - `gdb`
  - `gcc14`
  - `pkgconf`
  - `sdl2`
  - `openal-soft`
  - `gtk3`
  - `gtksourceview3`
  - `libXv`
  - `zip`

On Windows,
you should install [MSYS2][],
and from there install `make` and `mingw-w64-ucrt-x86_64-gcc`:

    pacman -S make mingw-w64-ucrt-x86_64-gcc

[MSYS2]: https://www.msys2.org/

On macOS,
installing the Xcode command-line tools should be enough.

Building
--------

Once the requirements are installed,
you should be able to build bsnes by opening a terminal
(On Windows, it should be a MinGW-W64 terminal),
changing to the directory where the source code is checked out,
and running:

    make -C bsnes

Additional configuration options can be provided,
see [Build Configuration](../reference/build-config.md) for details.

This produces an executable in the `bsnes/out/` directory.

Running
-------

On macOS,
the resulting executable has everything it needs built-in,
so you can just run it directly.

On Windows and Linux,
bsnes needs additional resources like the game database,
shaders, and firmware files.
On Linux or FreeBSD,
you probably also want icons and a `.desktop` file.
To copy all these files into the right places in your home directory,
run:

    make -C bsnes install

**Note:** On Linux or FreeBSD, do *not* run `make install` as root.
It sets up the files bsnes needs in *your* home directory,
and you don't want to wind up with a bunch of root-owned files.
bsnes does not support being installed system-wide.

On Linux or FreeBSD,
these auxilliary files are installed to `~/.local/`
while on Windows they're installed to `%%LOCALAPPDATA%%`.

It is possible to make a "portable" distribution
that does not require installation,
by putting the required files beside the executable
just as the official downloads do.
