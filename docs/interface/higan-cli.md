
# Synopsis

> higan [*\-\-fullscreen*] [*PATH*]

# Description

TODO: Put `NTSC-J|`, `NTSC-U|` or `PAL|`
at the beginning of the path
to force a region
for consoles where it can't be detected.

When launched with `--fullscreen`,
higan will automatically enter full-screen mode
when it starts.
This is not much use unless you also specify `PATH`,
because you won't be able to load a game
until you exit full-screen mode
by pressing the "Toggle Fullscreen"
[hotkey](higan-config.md#hotkeys).

When `PATH` is the path to an existing
[game folder](../concepts/game-folders.md)
for any supported console,
that game will be loaded when higan starts.

When `PATH` is the path to a ROM file
for any supported console,
it will be imported into a game folder in
[the Game Library](../concepts/game-library.md),
and then loaded from there when higan starts.

# Examples

Play a previously-imported copy of Super Mario World
in full-screen (assuming Linux defaults):

```sh
higan --fullscreen ~/Emulation/"Super Famicom"/"Super Mario World.sfc"
```
