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
