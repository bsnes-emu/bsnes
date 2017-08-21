If a [game folder](game-folders.md) is
the emulation equivalent of
a physical game cartridge,
a manifest is like
the circuit board inside the cartridge:
it controls how all the other parts of the cartridge
talk to one another and to the console itself.

If you load a game into higan,
you can look at the game's manifest
by opening [the Tools menu](../interface/higan.md#the-tools-menu)
and choosing [Manifest Viewer](../interface/higan-tools.md#the-manifest-viewer).

Why manifests?
--------------

Any given console might support
hundreds or thousands of different games,
but the vast majority of those games
use one of a few different circuit boards.
For most consoles,
the raw game data provides enough clues
to guess a circuit board configuration
that will get the game running,
but relying on heuristics often leads to problems:

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
        it can result in the emulator
        doing something completely different
        because it now matches
        some completely different game.
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

Manifests provide a way to *tell* the emulator
what hardware configuration to use,
avoiding all these problems entirely.

Where do manifests come from?
-----------------------------

Ideally,
everybody who extracts the data from a game cartridge would
(at the same time)
record the board configuration in a manifest file.
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
and homebrew that was never on a cartridge to begin with.
For these games,
heuristics will always be needed as a fallback,
but at least if the heuristics are wrong
the can be overridden.

Manifests can be found inside
[game folders](game-folders.md),
but usually aren't.
Occasionally,
a newly-dumped game will turn out to have
a configuration that can't be expressed
in the existing manifest file format,
and a new format must be designed.
If manifests were always written inside game folders,
games with old-format manifests
would break when played in emulators that supported the new format.

Therefore,
icarus [defaults](#the-icarus-settings-dialog)
to not writing out manifests when it imports games,
and higan [defaults](#the-configuration-dialog)
to ignoring manifests that are present.
Instead,
when higan loads a game,
it will ask icarus to generate a temporary manifest in the latest format,
based on the files present in the game folder
and how they are likely to go together.
