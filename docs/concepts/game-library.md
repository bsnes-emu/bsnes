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


