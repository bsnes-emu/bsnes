
# Synopsis

> higan [*\-\-fullscreen*] [*PATH*]

# Description

When launched with `--fullscreen`,
higan will automatically enter full-screen mode
when it starts.
This is not much use unless you also specify `PATH`,
because you won't be able to load a game
until you exit full-screen mode
by pressing the "Toggle Fullscreen"
[hotkey](higan-settings.md#hotkeys).

When `PATH` is the path to an existing
[game folder](../concepts/game-folders.md)
for any supported console,
that game will be loaded when higan starts.

When `PATH` is the path to a ROM file
for any supported console,
it will be imported into a game folder in
[the Game Library](../concepts/game-library.md),
and then loaded from there when higan starts.

When loading a game from the Game Library
via higan's GUI,
you can force higan to emulate a particular regional variant
of the relevant console by
choosing a region code from the drop-down list
in the lower-right of the filesystem browser.
To achieve the same effect from the command-line,
`PATH` should be set to the region code used in the GUI,
followed by a vertical bar (`|`),
followed by actual filesystem path
to the game folder or ROM file to load.

# Examples

Play a previously-imported copy of Super Mario World
in full-screen (assuming Linux defaults):

```sh
higan --fullscreen ~/Emulation/"Super Famicom"/"Super Mario World.sfc"
```

Play a previously-imported copy of Sonic the Hedgehog 3,
forcing higan to emulate a Japanese Mega Drive:

```sh
higan "NTSC-J|$HOME/Emulation/Mega Drive/Sonic the Hedgehog 3.md"
```
