
# Synopsis

> higan [*\-\-fullscreen*] [*GAME* [*SUBGAME1* [*SUBGAME2*]]]

# Description

When launched with `--fullscreen`,
higan will automatically enter fullscreen mode
when it starts,
otherwise it starts in windowed mode.
This is not much use unless you also specify `GAME`,
because you won't be able to load a game
until you exit fullscreen mode
by pressing the "Toggle Fullscreen"
[hotkey](higan-settings.md#hotkeys).

When `GAME` is not given,
higan starts with no game loaded.

When `GAME` is the path to an existing
[game folder](../concepts/game-folders.md)
for any supported console,
that game will automatically be loaded
after higan starts.

When `GAME` is the path to a ROM file
for any supported console,
it will be imported into a game folder in
[the Game Library](../concepts/game-library.md),
and loaded from there after higan starts.

When `GAME` is of the form `REGION|PATH`
(that is,
a region code,
a vertical bar (`|`),
and a path to a game folder or ROM file)
higan will load the game at `PATH`
into the `REGION` variant of the appropriate console,
rather than detecting which variant the game expects.
This is the command-line equivalent
of choosing a region from
the drop-down in the bottom-right of the filesystem browser window
when you [load a game from the game library](../guides/import.md#regular-games).
The list of region codes differs from console to console,
so load a game from the library
and check the list
before loading it from the command-line.

When `GAME` refers to a game with
a [Satellaview](../guides/import.md#satellaview-games) memory pak slot,
`SUBGAME1` should be
the path to a game folder or ROM file
representing the memory pak to insert into the slot.
If `SUBGAME1` is not supplied,
higan will prompt for one.
`SUBGAME2` is ignored.

When `GAME` refers to
the [Super Game Boy](../guides/import.md#super-game-boy-games),
`SUBGAME1` should be
the path to a game folder or ROM file
representing a Game Boy game to insert into the slot.
If `SUBGAME1` is not supplied,
higan will prompt for one.
`SUBGAME2` is ignored.

When `GAME` refers to
the [Sufami Turbo](../guides/import.md#sufami-turbo-games),
then `SUBGAME1` should be
the path to a game folder or ROM file
representing the mini-cartridge to be inserted into
slot A of the Sufami Turbo base cartridge,
and `SUBGAME2` is the same thing for slot B.
higan will prompt for
any mini-cartridges not supplied on the command-line.

When `GAME` refers to any other game,
both `SUBGAME1` and `SUBGAME2` are ignored.

# Examples

Play a previously-imported copy of Super Mario World
in fullscreen (assuming Linux defaults):

```sh
higan --fullscreen "$HOME/Emulation/Super Famicom/Super Mario World.sfc"
```

Play a previously-imported copy of Sonic the Hedgehog 3,
forcing higan to emulate a Japanese Mega Drive:

```sh
higan "NTSC-J|$HOME/Emulation/Mega Drive/Sonic the Hedgehog 3.md"
```
