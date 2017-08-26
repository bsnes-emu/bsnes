The consoles that higan emulates
are similar in many ways,
but some of them do have particular quirks
that you should be aware of.

Music and Sound Effect Volume on the Mega Drive
-----------------------------------------------

The Mega Drive has two different audio-generating chips:

  - the SN76489 or "PSG" chip,
    inherited from the Master System,
    mostly used for sound-effects
    like Sonic picking up rings
  - the YM2612 or "FM" chip,
    mostly used for music

With two different sound sources,
it's important that they have similar volumes,
or the sound-effects will drown out the music,
or vice-versa.
Sega did *not* do this,
and different hardware revisions
used different relative volumes.

higan currently
sets the PSG volume to [125% of the FM volume][vol],
based on [a Sega Genesis model 1 VA6][va6] that byuu owns.
If you feel sound-effects in higan's Mega Drive core
are too loud or too quiet,
you may be comparing it
to a Mega Drive calibrated to a different scale
(or to an emulator tweaked to match such a Mega Drive).

[vol]: https://board.byuu.org/viewtopic.php?p=42482#p42482
[va6]: https://board.byuu.org/viewtopic.php?p=42195#p42195

Playing Game Boy Color games in Game Boy mode
---------------------------------------------

Games for the original Game Boy
came in solid grey cartridges,
and only supported four-shade greyscale graphics.
ROM files for these games
typically have filenames ending in `.gb`.

The Game Boy Color played all the original Game Boy games,
but extended the hardware to support colour graphics.
Games that required
the extra hardware in the Game Boy Color
came in transparent cartridges,
and had a slightly different shape
to prevent them from being used in original Game Boys..
ROM files for these games
typically have filenames ending in `.gbc`.

However,
there were also some games
that could use colour if it was available,
but would stick to greyscale if it wasn't.
These games came in black cartridges.
ROM files for these games
typically have filenames ending in `.gbc`
(since they are genuinely designed for the Game Boy Color)
or `.gbb`.

Sometimes people ask
for higan to include these backwards-compatible Game Boy Color games
when asking for a Game Boy game to load.
However,
this would make higan much more complex
for not much benefit:
it's just the same game as in Color mode,
but with bits chopped off.
You might as well play backward-compatible games
in Game Boy Color mode
and get the full experience the developers intended.

If you really, really want to see
what a particular game's backward-compatible mode looked like,
change the filename to end with `.gb`
(instead of `.gbc` or `.gbb`)
before [importing it](guides/import.md).
If you want to experiment
with loading in-game saves from colour-mode in monochrome mode
or vice-versa,
you can import the game once with `.gb`
and once with `.gbc`,
then manually copy files between the
[game folders](concepts/game-folders.md)
in the "Game Boy" and "Game Boy Color" sub-folders
of the [Game Library](concepts/game-library.md) folder.

Do not expect save-states to be compatible between
Game Boy and Game Boy Color.

In-Game Saves and the Game Boy Advance
--------------------------------------

For most of the consoles that higan emulates,
in-game saves are simple:
the cartridge contains some battery-backed RAM
that the game accesses like any other memory,
and the game's internal header usually contains some hint
about where in memory the save data appears
and how large it is.

The Game Boy Advance is different.
By the time of the GBA,
many save-storage technologies were available,
most with a more complex interface than plain memory.
Frustratingly, the GBA's internal header
does *not* describe which storage variant the game expects.
Therefore,
when importing a GBA game,
higan must guess which storage type to use
and sometimes it guesses incorrectly.

If higan guesses incorrectly for a game you want to play,
you will need to turn on
"Create manifests" in
[the Icarus settings dialog](interface/icarus.md#the-icarus-settings-dialog),
turn off
"Ignore manifests" in
[higan's Advanced settings](interface/higan-settings.md#advanced),
re-import the game,
and edit `manifest.bml` in
[the game folder](concepts/game-folders.md)
to describe the correct storage type.
Try importing other GBA games to see what save types they use.

For more discussion of the GBA save type mess,
see [What's the deal with... GBA save files?][gbasaves]

[gbasaves]: http://zork.net/~st/jottings/GBA_saves.html

Rumble compatibility for Game Boy (Color)
-----------------------------------------

The Game Boy and Game Boy Color did not natively support
any kind of rumble or force-feedback system,
but some game cartridges (such as Pokémon Pinball)
included a rumble motor within the cartridge itself.

Because higan does not currently support
game-specific controller features,
to experience the rumble effect in higan
you'll need to configure the console itself:

  - Open
    [higan's Input settings](interface/higan-settings.md#input)
  - In the list of consoles,
    select Game Boy, or Game Boy Color
    depending on which console you want to use to play the game
  - In the list of inputs,
    double-click "Rumble"
    or select it and press Enter
  - Press any button on the gamepad that should shake
    when the game turns on the rumble effect.

Rumble compatibility for Game Boy Advance
-----------------------------------------

The original Game Boy Advance
and the Game Boy Advance SP
did not support any kind of rumble or force-feedback system,
but the Game Boy Player addon for the Gamecube
allowed Game Boy Advance games
to use the rumble feature in Gamecube controllers.

Because rumble is a feature of the Game Boy Player,
to experience the rumble effect in higan
you'll need to configure the console itself:

  - Open
    [higan's Input settings](interface/higan-settings.md#input)
  - In the list of consoles,
    select Game Boy Advance
  - In the list of inputs,
    double-click "Rumble"
    or select it and press Enter
  - Press any button on the gamepad that should shake
    when the game turns on the rumble effect.

As well as the Game Boy Player rumble feature,
some Game Boy Advance cartridges
included a rumble motor within the cartridge itself.
higan does not support this rumble technology,
but that's not a big deal:
the only two such games are *Drill Dozer*,
which can use Game Boy Player rumble,
and *WarioWare: Twisted*,
which doesn't work anyway
because it requires gyroscope hardware
that higan does not yet emulate.

Game Boy Advance rotation
-------------------------

Some Game Boy Advance homebrew games,
as well as a bonus mode in *Dr Mario + Puzzle League*,
expect the player to physically rotate the device
so the screen is tall rather than wide.
higan supports this feature with
a Rotate [hotkey](interface/higan-settings.md#hotkeys).

When the user presses the Rotate hotkey,
the console's video output is rotated 90° anti-clockwise,
and the directional-pad controls are also rotated
so that (for example) pushing the button for "up"
sends the signal "right" to the emulated console,
so that the player character moves "up" on the rotated screen.

WonderSwan rotation
-------------------

The WonderSwan hardware
included multiple sets of buttons
so the player could hold the device
vertically or horizontally.
*Makaimura for WonderSwan* includes a level
that requires the player to repeatedly rotate
the device as they play.
higan supports this feature with
a Rotate [hotkey](interface/higan-settings.md#hotkeys).

When the user presses the Rotate hotkey,
the console's video output is rotated 90° anti-clockwise,
and the X and Y button cluster mappings
are adjusted to match.
