icarus is a separate tool
bundled with higan
that allows you to bulk-import ROM files
into higan's [game library].

Bulk importing ROM files
------------------------

icarus' main window
is [a filesystem browser](common.md#the-filesystem-browser),
with customisations:

  - The filesystem browser only lists
    files with extensions typically used for ROM dumps from
    consoles higan emulates,
    plus `.zip` files since ROM dumps are often compressed.
  - Each matching file has a check-box next to it.
  - You can tick the check-box next to every listed file at once
    by pressing "Select All" in the bottom-left.
  - You can un-tick all the check-boxes
    by pressing "Unselect All" in the bottom-left.

Pressing "Import ..." in the bottom-right
will close the filesystem browser
then try to import all the files
whose check-boxes are ticked
into the [game library].
icarus displays a progress dialog during the import process,
and a result window if any errors occurred.

**Note:** Some games require extra steps to import correctly;
see [Importing and playing games](../guides/import.md) for details.

The icarus Settings dialog
--------------------------

Pressing "Settings ..."
in the bottom-right corner of the main icarus window
opens the settings dialog.

The icarus Settings dialog contains the following settings:

  - **Library Location** determines
    where icarus puts the games it imports.
    See [Moving the Game Library][movgamelib]
    for details.
  - **Create Manifests** causes icarus to
    include a [manifest] file
    inside the [game folder] for each imported game.
    See [Ignoring manifests](../concepts/manifests.md#ignoring-manifests)
    for details.
  - **Use Database** causes icarus to use manifest information
    from its database of known-good manifests,
    if it's importing a game it recognises.
    For unrecognised games,
    and for all games if this box is unticked,
    icarus guesses the manifest data.
    This option is still relevant when "Create Manifests" is unticked:
    higan uses icarus to generate a manifest when a game is loaded,
    not just at import-time.

Command line
------------

icarus can be launched with any of the following command-line arguments:

> icarus
>
> icarus \-\-import *FILE*
>
> icarus \-\-manifest *GAME*

When run without arguments,
icarus runs interactively
as described under [Bulk importing ROM files](#bulk-importing-rom-files) above.

When run with the `--import` flag,
`FILE` should be the path to a ROM file
for one of the consoles higan supports,
or a `.zip` file containing such a ROM file.
icarus will import it into the [game library]
just as it would if running interactively,
and the full path to the ressulting game folder
is printed to icarus' standard output.

If the game cannot be imported correctly
due to missing firmware,
icarus prints no output.

When run with the `--manifest` flag,
`GAME` should be the path to a [game folder],
such as a game previously imported into the [game library].
icarus will examine the game,
come up with a [manifest] describing the game's memory layout,
and print it to standard output.

[game library]: ../concepts/game-library.md
[movgamelib]: ../concepts/game-library.md#moving-the-game-library
[game folder]: ../concepts/game-folders.md
[manifest]: ../concepts/manifests.md
