What is a game folder?
----------------------

A game folder
is higan's way of grouping all
the information and resources required
to properly emulate a particular game.
Other emulators group resources related to a game
by requiring that every file has the base name
but different file extensions.

For example,
if another emulator loaded the game `dkc3.sfc`
it might store the save data in `dkc3.srm`:

```text
+- Super Famicom
   |
   +- dkc3.sfc
   |
   +- dkc3.srm
```

higan would create a game folder named `dkc3.sfc`,
and inside it store the game data as `program.rom`
and the save data as `save.ram`:

```text
+- Super Famicom
   |
   +- dkc3.sfc
      |
      +- program.rom
      |
      +- save.ram
```

Why game folders?
-----------------

The traditional files-with-different-extensions system
works well enough when a game only requires
a small number of files,
because a file extension doesn't offer
much room for description.
Also,
since file extensions traditionally describe
the format of the file in question,
it also means a game can't use
two or more files in the same format.

Compared to other emulators,
higan tends to use a larger number of files per game.
For example,
higan's low-level emulation of Super Famicom co-processors
often requires [separate firmware files][firmware].
higan's [MSU-1 support][msu1]
requires up to 99 audio tracks per game,
and higan supports up to 133 save-states per game.
Therefore,
higan suffers from the limitations of name-based-grouping
more than most.

higan's game folders allow all a game's resources
to be given unique, descriptive filenames,
and to use the file-extension
that's most appropriate for that file.
They also allow emulator-specific data
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

As mentioned [above](#what-is-a-game-folder),
a game folder collects all the information relevant
to emulating a particular game.
Not all of the following files
are relevant to every emulated console,
or to every game on a given console,
but they may be present under particular circumstances.

All the files directly in the game folder
are expected to be useful
to all emulators that support them:

  - `manifest.bml`:
    The [manifest](#what-is-a-manifest)
    for this game folder.
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
    when higan exits gracefully,
    if higan crashes or is forced to quit,
    in-game saves may be lost.
    Other emulators sometimes call this an "SRAM file",
    even though the same filename is used
    for cartridges that use EEPROM or Flash storage,
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
are placed in a `higan` subdirectory:

  - `cheats.bml`:
    All information present in
    [the Cheat Editor](../interface/higan-tools.md#the-cheat-editor)
    is stored here.
  - `states/quick/slot-*.bst`:
    All [Quick States](save-states.md#quick-states) are stored here.
  - `states/managed/slot-*.bst`:
    All [Manager States](save-states.md#manager-states) are stored here.
