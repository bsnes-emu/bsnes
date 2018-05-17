Most of the consoles higan emulates
were designed for low resolution NTSC televisions,
and their video output is chunky and blocky
by today's standards.
Video shaders customise how a console's video output
is drawn to the computer screen,
and can clean up and smooth out the original video,
reproduce the scanlines and blurring of the original display,
or any other visual effect.

The available video shaders are listed in
the "Video Shaders" sub-menu of the [Settings menu].
Which shaders are available depends on
the [video driver](drivers.md#video) higan is configured to use.
Most drivers only support these shaders:

  - **None**
    draws each computer pixel according to
    the colour of the single nearest console pixel,
    sometimes called "nearest neighbour" scaling.
    This produces unnaturally crisp and blocky images.
      - If you use [aspect correction or non-integral scaling][ac],
        neighbouring console pixels may be drawn
        with a different number of computer pixels due to rounding errors,
        causing a distracting rippling effect.
  - **Blur**
    draws each computer pixel according to
    the weighted average colour
    of the four nearest console pixels,
    sometimes called "bilinear" scaling.
    This produces unnaturally blurry images.

[ac]: ../interface/higan-settings.md#video

In addition to those,
the OpenGL driver also supports custom shaders.

**Note:**
For technical reasons,
higan's emulation of certain consoles
can produce surprising behaviour
in certain shaders,
particularly shaders that compare each console pixel
with its neighbours.
See [Video Shaders and TV-based consoles][vstv] for details.

[vstv]: #video-shaders-and-tv-based-consoles

Where to get custom shaders
---------------------------

  - higan includes some simple example shaders.
    If your copy of higan did not come with shaders,
    you can get them from
    [the unofficial higan repository](https://gitlab.com/higan/higan/tree/master/shaders).
  - [quark-shaders](https://github.com/hizzlekizzle/quark-shaders)
    contains many high-quality shaders for use with higan.
  - You can write your own.

How to install custom shaders
-----------------------------

Make sure the shader you want to install
is in the correct format:
it should be a folder whose name ends in `.shader`,
it should contain a file named `manifest.bml`,
and probably some `*.fs` or `*.vs` files.

Place the shader folder inside
the `shaders` folder
of your higan installation.
If you don't have a `shaders` folder,
create it beside the `systems` folder
and `settings.bml`.

  - On Windows,
    this is probably the folder containing `higan.exe`
  - On Linux,
    this is probably `~/.local/share/higan`

Launch higan,
open the Settings menu,
and choose "Advanced ..."
to open [the Advanced tab](../interface/higan-settings.md#advanced)
of the Settings window.
Under "Driver Selection",
make sure "Video" is set to "OpenGL".
If it wasn't already set that way,
you'll need to restart higan
for the change to take effect.

Open the Settings menu again,
choose the "Video Shader" sub-menu,
and now the shaders you installed
should be listed at the bottom of the menu.

Load a game
(so you can see the results),
switch between shaders
to see what they do,
and pick your favourite!

Notable examples
----------------

The quark-shaders repository mentioned above
contains lots of carefully-crafted shaders,
but some are particularly noteworthy:

  - **AANN** implements "anti-aliased nearest neighbour" scaling.
    This uses anti-aliasing to hide
    the rounding errors often introduced by
    aspect ratio correction
    and non-integral scaling,
    producing an image nearly as crisp as the "None" shader,
    but without the distracting ripple effect.
  - **Gameboy** emulates the squarish aspect-ratio,
    greenish-colours,
    and limited palette
    of the original Game Boy.
    At larger scales,
    you can even see the fine gaps between each pixel,
    and the shadow that dark colours would cast
    on the LCD background.
  - **NTSC** performs NTSC encoding,
    bandwidth limiting,
    and NTSC decoding of the video image to recreate
    the colour fringing,
    blurring
    and shimmer
    that most game players would have seen
    on real televisions.
    Some games depend on NTSC artefacts
    to display colours outside the console's official palette
    or to create effects like transparency.

Video Shaders and TV-based consoles
-----------------------------------

Simple shaders
(like "None"
and the third-party "AANN" shader)
just blindly scale up the images they're given,
but sophisticated shaders
(such as the third-party "xBR" shader)
try to produce higher-quality output
by recognising particular patterns,
like taking three diagonal pixels
and turning that into a smooth diagonal line.

These shaders assume that
each pixel drawn by the game's artists
becomes a single pixel in the video output they analyze.
The hand-held consoles that higan emulates
(and also the Famicom)
can only output video at one specific resolution,
so this "one pixel equals one pixel" rule holds true,
and pattern-based shaders like "xBR" work just fine.
Unfortunately,
this is not true for most of the TV-based consoles
that higan supports.

The Super Famicom's "normal" video mode
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
and so it has the same problems with shaders as those consoles.

[Settings menu]: ../interface/higan.md#the-settings-menu
