Most of the consoles higan emulates
were designed for the low resolution of NTSC televisions,
and their video output is often chunky and blocky
by today's standards.
Shaders customise how a console's video output
is drawn to the computer screen,
and can apply just about any effect you can imagine.

Most [drivers](drivers.md)
only support these shaders
(some only support one or the other):

  - **None** draws each computer pixel
    in the same colour as the nearest console pixel.
    This is sometimes called "nearest neighbour scaling",
    and produces crisp, blocky output.
  - **Blur** draws each computer pixel
    as the weighted average colour
    of the four nearest console pixels.
    This is sometimes called "bilinear scaling",
    and hides some of the blockiness
    at the expense of blurring edges.

However,
the OpenGL driver supports custom shaders,
in addition to the above.

**Note:**
For technical reasons,
higan's emulation of certain consoles
can produce surprising behaviour
in certain shaders,
particularly shaders that compare each console pixel
with its neigbours.
See [Console-specific Notes](../notes.md) for details.

# Where to get shaders

  - higan includes some simple example shaders.
    If your copy of higan did not come with shaders,
    you can get them from
    [the unofficial higan repository](https://gitlab.com/higan/higan/tree/master/shaders).
  - [quark-shaders](https://github.com/hizzlekizzle/quark-shaders)
    contains many high-quality shaders for use with higan.
  - You can write your own.

# How to install shaders

Make sure the shader you want to install
is in the correct format:
it should be a folder whose name ends in `.shader`,
it should contain a file named `manifest.bml`,
and probably some `*.fs` or `*.vs` files.

Place the shader folder inside
the `Video Shaders` folder
of your higan installation.
If you don't have a `Video Shaders` folder,
create it beside the `*.sys` folders
like `Game Boy Advance.sys` and `Super Famicom.sys`.

  - On Windows,
    this is probably the folder containing `higan.exe`
  - On Linux,
    this is probably `~/.local/share/higan`

Launch higan,
open the Settings menu,
and choose "Advanced ..."
to open [the Advanced tab](../interface/higan-config.md#advanced)
of the Settings dialog.
Under "Driver Selection",
make sure "Video" is set to "OpenGL".
If you changed the video driver,
you'll need to restart higan
for the change to take effect.

Open the Settings menu again,
choose the "Video Shader" submenu,
and now the shaders you installed
should be listed at the bottom of the menu.

Load a game
(so you can see the results)
and switch between shaders
to see what they do
and pick your favourite!

# Notable examples

The quark-shaders repository
contains lots of carefully-crafted shaders,
but some are particularly noteworthy:

  - **AANN** implements "anti-aliased nearest neighbour" scaling.
    If the console's video is not displayed
    at an exact multple of the console's native resolution,
    rounding errors cause normal nearest-neighbour scaling
    to draw some rows and columns wider than others,
    which many people find ugly and distracting.
    This is very common when
    higan's aspect-ratio correction mode
    is enabled.
    AANN uses very slight anti-aliasing
    to hide the rounding errors,
    leaving the overall image as crisp as nearest-neighbour.
  - **Gameboy** emulates the squarish aspect-ratio
    greenish-colours
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
    This is important because
    some games depended on NTSC artifacts
    to display colours outside the console's official palette
    or to create effects like transparency.
