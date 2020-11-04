Override a game's memory map
============================

In order to correctly emulate
how a real Super Nintendo would play a particular game,
bsnes [needs to know](https://byuu.net/cartridges/boards/)
how that game's data would be arranged
in the system's memory map,
and any extra hardware that might have been present in the game's cartridge.
This extra data describing a game
is called a "manifest".
When searching for manifest data,
bsnes uses the first source it finds
in the following list:

 1. The [SNES Preservation Project][snespres] database provided with bsnes
      - This provides accurate data for most commercially released games
 1. A manifest file stored beside the ROM file
      - Allows you to control the exact memory map the emulated game will see
 1. Heuristic analysis of the ROM file
      - Works for all commercially released games,
        but often broken for prototypes, betas, mods, and homebrew games

[snespres]: https://byuu.org/preservation

Where to get a manifest
-----------------------

The easiest place to get a working manifest
is from the included game database.
Open the [Database/Super Famicom.bml][gamedb] file,
search for the name of a game that's similar
to the one you want to get working,
then copy the entire "game" block.

[gamedb]: https://github.com/bsnes-emu/bsnes/blob/master/bsnes/Database/Super%20Famicom.bml

You should wind up with something that looks like this:

    game
      sha256:   12b77c4bc9c1832cee8881244659065ee1d84c70c3d29e6eaf92e6798cc2ca72
      label:    Super Metroid
      name:     Super Metroid
      region:   SNS-RI-USA
      revision: SHVC-RI-0
      board:    SHVC-BA3M-01
        memory
          type: ROM
          size: 0x300000
          content: Program
        memory
          type: RAM
          size: 0x2000
          content: Save

Alternatively,
if you don't know the name of a similar game,
or if the same game has multiple different entries
(for example, a USA and a Europe release),
you can use the Manifest Viewer
to copy the manifest from a working game.

Load up the working game in bsnes,
then from the Tools menu,
choose "Manifest Viewer",
and you'll see the complete manifest bsnes is using.
Again, you can copy the "game" block
(ignore the separate "board" block below it).

How to modify it
----------------

You should paste the manifest
into a "plain text" editor to modify it,
like Notepad++ on Windows,
or TextEdit (in plain-text mode) on macOS.
Regular old Notepad may work if you're using a version
that can save files with Unix (LF) line-endings.
Don't use a word processor that can format text,
like Microsoft Word or Wordpad,
since bsnes doesn't understand formatting codes
and it will get confused.

You can remove the lines beginning with `sha256:`, `name:`, and `revision:`,
if present,
since they're only useful for tracking
which games are included in the official database.
The text after `label:` controls
the name that bsnes will display in the title-bar
when the game is loaded.
The text after `region:` tells bsnes
whether the game expects to be run on an NTSC system
(Japan, USA)
or a PAL system (Europe, Australia).
In the game database,
this field holds the publisher's unique identifier
from which bsnes infers the correct region,
but you can just write "NTSC" or "PAL".

You should also check the "memory" block with `type: ROM`.
The `size:` field describes the expected size
of the ROM image, in bytes.
In our example above,
the size is 0x300000 in hexadecimal,
which is 3,145,728 in decimal,
which is the exact number of bytes in the Super Metroid ROM image.
If the manifest you're using as an example
mentions a *different* size
than the ROM image you're trying to get working,
it's probably the wrong example
and you should choose a different one.

Where to put it
---------------

Make sure you save your customised manifest
as a plain text file,
using Unix (LF) line endings,
and the UTF-8 text encoding.

If your ROM image is in a higan-style game folder
(a directory with the `.sfc` extension,
containing a `program.rom` file),
save the manifest in that directory,
named `manifest.bml`.

If your ROM image is a single file
(probably with the `.sfc` extension),
save the manifest beside it with the `.bml` extension.
For example,
if the ROM you're trying to get working is called `myhomebrew.sfc`,
save the manifest beside it as `myhomebrew.bml`.

Troubleshooting
---------------

If your ROM now loads correctly, congratulations!

If it's still broken,
the easiest way to verify that bsnes is reading your manifest
is to open the Manifest Viewer from the Tools menu.
If you don't see the manifest data as you created it,
make sure the manifest data is saved in the correct location,
with the correct name,
and in the correct file format.

Also,
note that bsnes ignores external manifests
for games that appear in its game database.

If your manifest is being read correctly,
but the game still doesn't work,
you should try basing your manifest off a different game.
