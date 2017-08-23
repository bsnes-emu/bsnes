The game library
is the folder where all the
[game folders](game-folders.md) go.
When [icarus](../interface/icarus.md) imports a game,
it creates or updates
the corresponding game folder in the game library.
When you use the console menu items
in [higan's Library menu](../interface/higan.md#the-library-menu),
it higan shows you the games for that console
already in the library.

Inside the game library folder
there is a subfolder for each system.
Inside each system folder
are the game folders
for each imported game.

Why a game library?
-------------------

higan [requires game folders](game-folders.md#why-game-folders),
but it also invented game folders,
so existing games need
to be converted to game folder format
before higan can play them.
The converted copy of a game needs to go *somewhere*.

Also,
for people who have a huge collection of games,
the game library only lists the games they actually play,
rather than every possible game,
making it easier to find the games they like.


Where is the game library?
--------------------------

In Windows,
the default location of
the game library is the `Emulation` folder
inside your profile folder
(To find your profile folder,
press `Win+R` to open the Run dialog,
then type `%USERPROFILE%` and press Enter).

In Linux,
the default location of
the game library is the `Emulation` directory
inside your home directory.

On all platforms,
the game library location can be configured.
See [Moving the Game Library](#moving-the-game-library)
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
    to open
    [the Settings dialog][icsettings].
 1. Click the "Change ..." button on the right.
    A [filesystem browser][fsbrowser] window will appear,
    allowing you to choose
    where imported games will be stored.
 1. Launch higan,
    then from [the Settings menu][settingsmenu],
    choose "Advanced..."
    to open [higan's Advanced settings][advsettings].
 1. Click the "Change ..." button on the right.
    A [filesystem browser][fsbrowser] will appear,
    allowing you to choose the same folder again.

[icsettings]: ../interface/icarus.md#the-icarus-settings-dialog
[fsbrowser]: ../interface/common.md#the-filesystem-browser
[settingsmenu]: ../interface/higan.md#the-settings-menu
[advsettings]: ../interface/higan-config.md#advanced
