The consoles that higan emulates
are similar in many ways,
but some of them do have particular quirks
that you should be aware of.

Video Shaders and TV-based consoles
-----------------------------------

[Video Shaders](guides/shaders.md)
customize how higan scales
the low-resolution video of the emulated console
up to the high-resolution of the computer display.
Simple shaders
(like "None"
and the third-party "AANN" shader)
just blindly scale up the images they're given,
but sophisticated shaders
(such as the third-party "xBR" shader)
try to produce higher-quality output
by recognising particular patterns of pixel,
like taking three diagonal pixels
and turning that into a smooth diagonal line.

These shaders assume that
each pixel drawn by the game's artists
becomes a single pixel in the video output they analyze.
Many of the consoles higan emulates
can only output video at one specific resolution,
so this "one pixel equals one pixel" rule holds true,
and pattern-based shaders like "xBR" work just fine.

Unfortunately,
this is not the case for the Super Famicom.
The "normal" video mode
draws 256 pixels across the width of the screen,
but the "high resolution" mode draws 512.
Since Super Famicom games can enable hi-res mode at any time
(even halfway through a frame),
higan always renders Super Famicom video output 512 pixels wide,
just in case.
This means that in "normal" mode,
each pixel drawn by the game's artists
becomes two pixels in the video output,
breaking the assumption
that pattern-based shaders are based on.

The Super Famicom has a similar issue in the vertical direction:
normally,
an NTSC-based Super Famicom draws about 240 rows of output every frame,
sometimes referred to as "240p" video.
When a game turns on "interlaced" mode,
it draws the 240 odd-numbered lines of one frame,
then the 240 even-numbered lines of the next,
and so forth.
This is sometimes referred to as "480i" video.
Although interlaced mode cannot be enabled mid-frame
like high-resolution mode,
resolution switching is still complex,
so higan always draws all 480 lines of video output.
This means for a normal, non-interlaced game,
each pixel drawn by the game's artists
becomes four pixels in the video output
(two horizontally and two vertically)
making pattern-based shaders even less useful.
It also breaks most scanline-emulation shaders,
since they typically draw a scanline
for each row of pixels in the video output.

The Mega Drive has similar problems
to the Super Famicom.
It has the same behaviour with interlacing,
but its high-resolution mode switches
from 256 pixels across to 320 pixels across.
Therefore in normal mode,
each pixel drawn by the game's artists
becomes five pixels in the video output,
while in high-resolution mode,
each pixel drawn by the game's artists
becomes four pixels in the video output
(or 10 and 8 pixels in non-interlaced mode).

The PC Engine does not support an interlaced mode,
but its horizontal resolution is much more flexible
than the Super Famicom or Mega Drive,
and so it has the same problems with shaders.

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

Playing Game Boy Colour games in Game Boy mode
----------------------------------------------

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

TODO: GBA in-game saves are Difficult

Rumble compatibility for Game Boy (Color)
-----------------------------------------

TODO: Test Pokémon Pinball

Rumble compatibility for Game Boy Advance
-----------------------------------------

TODO:
Some GBA carts contained an actual rumble feature (which?),
some only supported rumble via the GBA Player (which?).
Which does higan support?

WonderSwan rotation
-------------------

TODO: The WonderSwan rotates!

