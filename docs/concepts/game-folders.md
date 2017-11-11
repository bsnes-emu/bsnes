A game folder
is higan's way of grouping all
the information and resources required
to properly emulate a particular game.
For example,
to represent a Super Famicom game named `dkc3`,
higan would create a game folder named `dkc3.sfc`,
and inside it store the game data as `program.rom`
and the save data as `save.ram`:

```python
+- Super Famicom
   |
   +- dkc3.sfc
      |
      +- program.rom
      |
      +- save.ram
```

In contrast,
other emulators typically
group resources related to a game
by requiring that every file has the same base name
but different file extensions.
For example,
if another emulator loaded the game `dkc3.sfc`
it might store the save data in `dkc3.srm`:

```python
+- Super Famicom
   |
   +- dkc3.sfc
   |
   +- dkc3.srm
```

Why game folders?
-----------------

A file extension doesn't offer
much room for description,
so the traditional name-based-grouping system
only really works when games use a small number of files,
Also,
since file extensions traditionally describe
the format of the file in question,
it also means a game can't use
two or more files in the same format.

Compared to other emulators,
higan can use a larger number of files per game.
For example,
higan's low-level emulation of Super Famicom co-processors
often requires [separate firmware files][firmware].
higan's [MSU-1 feature][msu1]
supports up to 99 audio tracks per game,
and higan supports up to 133 save-states per game.
Thus,
higan suffers from the limitations of name-based-grouping
more than most.

higan's game folders allow a game
to have unique, descriptive filenames
for all its resources,
and for each file to use the extension
that's most appropriate.
They also allow emulator-specific extras
like save-states and the cheat database
to be kept separate from the game's actual data,
by putting it in a sub-folder.

[msu1]: ../guides/import.md#msu-1-games
[firmware]: ../guides/import.md#games-with-co-processor-firmware

For a more detailed motivation for game folders,
see [Game Paks on the higan website][gp].

[gp]: https://byuu.org/emulation/higan/game-paks

What's in a game folder?
------------------------

A game folder collects all the information relevant
to emulating a particular game.
Not all of the following files
are relevant for every console,
or even for every game on a console,
but they may be present under particular circumstances.

All the files directly in the game folder
are expected to be useful
to all emulators that support them:

  - `manifest.bml`:
    The [manifest](manifests.md) for this game folder.
  - `program.rom`:
    For most consoles,
    this contains
    the executable instructions and graphics data
    from the cartridge's ROM chips.
    For the Famicom,
    this contains only the executable instructions.
  - `character.rom`:
    For the Famicom,
    this contains only the graphics data
    from the cartridge's ROM chips.
  - `ines.rom`:
    While other consoles typically include enough hints
    in `program.rom` for icarus to guess a working manifest,
    the Famicom does not.
    Famicom games not stored in game folders
    typically include an "iNES header" to store that information,
    which icarus preserves after import as `ines.rom`.
  - `save.ram`:
    Games that include a save feature
    will create this file.
    Note that it is only written to disk
    when higan exits gracefully;
    if higan crashes or is forced to quit,
    in-game saves may be lost.
    Other emulators sometimes call this an "SRAM file",
    even though the same filename is used
    with games that included EEPROM or Flash storage,
    not just battery-backed Static RAM.
  - `rtc.ram`:
    Games that include a calendar or real-time clock
    will create this file.
  - `*.data.rom`, `*.program.rom`:
    Files named like this are usually [co-processor firmware][firmware].
  - `msu1.rom`:
    Holds streamable data for [the MSU-1][msu1].
  - `track-*.pcm`:
    Holds streamable audio for [the MSU-1][msu1].

Files that are only useful to higan specifically
are placed in a `higan` sub-folder:

  - `cheats.bml`:
    All information present in
    [the Cheat Editor](../interface/higan-tools.md#the-cheat-editor)
    is stored here.
  - `states/quick/slot-*.bst`:
    All [Quick States](save-states.md#quick-states) are stored here.
  - `states/managed/slot-*.bst`:
    All [Manager States](save-states.md#manager-states) are stored here.
