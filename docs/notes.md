The consoles that higan emulates
are similar in many ways,
but some of them do have particular quirks
that you should be aware of.

Video Shaders and the Super Famicom (and Mega Drive)
----------------------------------------------------

Shaders look weird on Super Famicom because of interlace/hi-res

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
is a single pixel in the video output they analyze.

Most of the consoles higan emulates
can only output video at one specific resolution,
so this "one pixel equals one pixel" rule holds true,
and shaders like "xBR" work just fine.
Unfortunately,
this is not the case for the Super Famicom.

The "normal" video mode for the Super Famicom
draws 256 pixels across the width of the screen,
but it also supports a "hi-res" mode
that draws 512 pixels on each line.
Since Super Famicom games can enable hi-res mode at any time,
even halfway through a frame,
higan always renders Super Famicom video output at 512 pixels wide,
just in case hi-res mode is enabled later on.
This means that in "normal" mode,
each pixel drawn by the game's artists
becomes two pixels in the video output,
breaking the assumption pattern-based shaders are based on.

The Super Famicom has a similar issue in the vertical direction:
normally,
an NTSC-based Super Famicom draws 238 rows of output,
~60 times per second.
This is sometimes referred to as "240p" video.
When a game turns on "interlaced" mode,
it draws ~480 lines of output,
~30 times a second;
first the ~240 odd-numbered lines,
then going back to the top and drawing the ~240 even-numbered lines.
This is sometimes referred to as "480i" video.

TODO: https://board.byuu.org/viewtopic.php?p=44758#p44758

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
typically have filenames ending in `*.gb`.

The Game Boy Color played all the original Game Boy games,
but extended the hardware to support colour graphics.
Games that required
the extra hardware in the Game Boy Color
came in transparent cartridges.
ROM files for these games
typically have filenames ending in `*.gbc`.

TODO: Rename files before import!
Or afterward.

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

