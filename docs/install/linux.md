Compiling from source on Linux
------------------------------

You will need a copy of the higan source-code.
If you download an official release from the higan homepage,
you will need [7-zip](http://www.7-zip.org)
or a compatible tool to extract it.
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

On a Debian-derived Linux distribution
(including Ubuntu and Mint),
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
 5. If the higan source includes a `shaders` subdirectory,
    type `make -C shaders install`
    to install the example
    [video shaders](../guides/shaders.md).

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
you should also delete the following directories
if they exist:

  - `~/.config/higan/`
  - `~/.config/hiro/`
  - `~/.local/share/higan/`
  - `~/.local/share/hiro/`

To remove the games imported into higan's library
(including in-game saves and save-states),
delete the directory `~/Emulation`.

You may also wish to delete the higan source directory.
