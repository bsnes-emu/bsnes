When launching icarus,
directly or by picking "Import ROM Files ..."
from higan's [Library menu](higan.md#the-library-menu),
the main icarus window appears.
This window allows you to bulk-import ROM files
into [higan's game library][gamelib],
and also to access icarus' settings.

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
  - You can tick the check-box next to every file at once
    by pressing "Select All" in the bottom-left.
  - You can un-tick all the check-boxes
    by pressing "Unselect All" in the bottom-left.

Pressing "Import ..." in the bottom-right
will close the filesystem browser
then try to import all the files
whose check-boxes are ticked
into [the Game Library][gamelib].
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
  - **Create Manifests** causes icarus
    to include
    [a manifest file](../concepts/manifests.md)
    inside
    [the game folder](../concepts/game-folders.md)
    for each imported game.
    See [Ignoring manifests](../concepts/manifests.md#ignoring-manifests)
    for details.
  - **Use Database** causes icarus to use manifest information
    from its database of known-good manifests,
    if it's importing a game it recognises.
    For unrecognised games,
    and for all games if this box is unticked,
    icarus gueses the manifest data.
    This option is still relevant when "Create Manifests" is unticked:
    higan uses icarus to generate a manifest when a game is loaded,
    not just at import-time.

[gamelib]: ../concepts/game-library.md
[movgamelib]: ../concepts/game-library.md#moving-the-game-library
