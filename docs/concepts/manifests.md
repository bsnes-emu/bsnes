If a [game folder](game-folders.md) is
the emulation equivalent of
a physical game cartridge,
a manifest is like
the circuit board inside the cartridge:
it connects all the other parts together
and to the console itself.

If you load a game into higan,
you can look at the game's manifest
by opening [the Tools menu](../interface/higan.md#the-tools-menu)
and choosing [Manifest Viewer](../interface/higan-tools.md#manifest-viewer).

Why manifests?
--------------

For most consoles,
a manifest isn't strictly necessary:
the raw game data provides enough clues
for emulators to guess the correct circuit board configuration,
or at least
to guess a *reasonable* configuration.
However,
relying on such heuristics often leads to problems:

  - Inventing heuristics
    that correctly guess the configuration
    for a particular game is very difficult.
      - If you change the rules,
        you really need to re-test
        the console's entire game library
        to check you haven't broken anything.
  - Some games accidentally do the wrong thing.
      - The Mega Drive game *Warrior of Rome II*
        follows the rule for a European game running at 50fps,
        when it's really an American game designed for 60fps.
  - Some games deliberately do the wrong thing.
      - The Game Boy Advance game *Top Gun - Combat Zones*
        follows the rule for
        (several kinds of)
        save-game storage.
        If it detects any save-game storage,
        it assumes it's running in an emulator
        and disables all the main menu options.
  - Heuristics are difficult to reason about.
      - If a homebrew developer
        takes a game that almost does what they want
        and changes it slightly,
        it may cause the emulator
        to do something completely different
        because the modified game
        is now a closer match to some other game.
  - Heuristics can make sensible configurations impossible.
      - If a homebrew developer wants
        a particular hardware configuration
        that no official games happened to use,
        it may not be possible
        to trigger it heuristically.
  - It's not hardware accurate.
      - The original console did not use heuristics
        to guess a hardware configuration,
        it just used the actual configuration
        of the actual hardware.

Manifests provide a way to describe
*exactly* what hardware configuration to use,
no guessing required,
avoiding all these problems entirely.

Where do manifests come from?
-----------------------------

Ideally,
everybody who extracts the data from a game cartridge would
also record the board configuration in a manifest file.
Unfortunately,
manifests were invented long after cartridge extracting,
so there are a lot of previously extracted games
with no manifest attached.

If a game doesn't come with a manifest,
it may be possible to look up the correct manifest.
The
[SNES Preservation Project](https://preservation.byuu.org/)
intends to re-dump every Super Famicom game
and record its board configuration at the same time.
Given a game's data,
you can take the SHA256 fingerprint
and look it up in the Project's database
to find the correct manifest to use.

Unfortunately,
this doesn't help for any other console.
Even for the Super Famicom,
there will always be games
too rare to acquire for re-dumping,
and homebrew that was never on a physical cartridge to begin with.
For these games,
heuristics will always be needed as a fallback,
but at least if the heuristics are wrong
they can be overridden.

If you are a homebrew author
or have dumped a previously-unknown cartridge,
and want to write a manifest yourself,
you should read the [official manifest specification][manifest].

[manifest]: https://doc.byuu.org/higan/manifests/

Ignoring manifests
------------------

Occasionally,
a newly-dumped game will turn out to have
a configuration that can't be expressed
in the existing manifest file format,
and a new format must be designed.
If manifests were always written inside game folders,
games with old-format manifests
would break when played in emulators that supported the new format.

Therefore,
icarus [defaults](../interface/icarus.md#the-icarus-settings-dialog)
to not writing out manifests when it imports games,
and higan [defaults](../interface/higan-settings.md#advanced)
to ignoring manifests that are present.
Instead,
when higan loads a game,
it will ask icarus to generate a temporary manifest in the latest format,
based on the files present in the game folder
and how they are likely to go together.

If the manifest that icarus generates
for some particular game
is broken or buggy,
you'll need to switch the manifest machinery back on
so you can fix it:

  - Turn on "Create manifests" in icarus' settings
  - Re-import the game in question
    to store the broken manifest
    as `manifest.bml` in the game folder.
  - Edit `manifest.bml` as you see fit
  - Turn off "Ignore manifests" in higan's settings
  - Load the game in higan to check that your changes fixed the problem

If the manifest format changes in the future,
you'll have to repeat this process
to create a fixed manifest in the new format.
