Unfortunately,
there's no standard for
displaying video,
playing audio,
and accepting input from game controllers
that works on every operating system.
Or rather,
there's many standards,
and different ones work best
on different computers.
Therefore,
higan comes with "drivers"
for video, audio and input,
so you can find the one that works best for you.
To see what drivers you're currently using,
or to choose different ones,
go to
[the Advanced tab](../interface/higan-config.md#advanced)
of the Settings window.

Here are the most notable drivers
for each platform
for each category.
If your copy of higan
includes a driver not listed here,
it's probably a reasonable choice,
so try it out and see how you like it.

**Note:** After changing any driver,
you must restart higan for the change to take effect.

**Note:**
Video, Audio and Input
all have a special driver named "None".
This is a dummy driver that does nothing
(draws no video,
plays no audio,
accepts no input),
so don't choose it unless you're
trying to diagnose problems with other drivers.

Video
-----

On Windows:

  - **OpenGL** is usually the best choice,
    since it supports [custom shaders](shaders.md),
    however it does require support for OpenGL 3.2
    which excludes some integrated graphics chipsets
    and old graphics cards.
  - **Direct3D** is a good choice
    if OpenGL is unavailable.
    It also allows
    [Exclusive fullscreen](../interface/higan-config.md#video),
    bypassing Windows' desktop compositor.
  - **GDI** is the safest choice,
    but performs very poorly at large sizes.

On Linux:

  - **OpenGL** is the best choice,
    since it's fast
    and it supports [custom shaders](shaders.md),
    but requires OpenGL 3.2.
    You can check what version of OpenGL
    your system supports by running
    `glxinfo | grep 'core profile version'`
    in a terminal.
  - **XVideo** is also fast,
    but may be low-quality,
    and generally only supports the "Blur" shader,
    not "None".
  - **XShm** is the safest choice,
    but performs very poorly at large sizes.

Audio
-----

On Windows:

  - **ASIO** offers the lowest possible latency,
    but is the least likely to work on any given computer.
  - **WASAPI** offers low latency,
    but is only slightly more likely to work.
    It also offers
    [Exclusive Mode](../interface/higan-config.md#audio),
    which can improve audio quality and lower latency,
    but may be better or worse than shared mode
    in practice.
  - **XAudio2** is a good choice,
    but it requires the latest (June 2010) version
    of the [DirectX 9 End-User Runtime][dx9]
    to be installed.
  - **DirectSound** is the safest choice,
    and not a bad one.

[dx9]: https://www.microsoft.com/en-us/download/details.aspx?id=35

On Linux:

  - **PulseAudio** or **PulseAudioSimple**
    are almost certainly the drivers to use,
    since almost every Linux distribution uses
    [PulseAudio](https://en.wikipedia.org/wiki/PulseAudio)
    to manage audio output.
    The two drivers should behave identically,
    but some users report one working better than the other.
  - **ALSA** is a good choice
    for Linux distributions that do not use PulseAudio.
    If PulseAudio is running,
    this will wind up using PulseAudio's ALSA emulation,
    or failing due to "sound card already in use",
    unless you use a tool like `pasuspender`.
  - **OSS** may be useful
    for non-Linux platforms,
    or for Linux systems with the third-party OSSv4
    kernel drivers installed.
    For most Linux systems,
    this will use ALSA's OSS emulation,
    PulseAudio's OSS emulation,
    or not work at all.

**Note:**
By default,
higan uses the audio driver to regulate its emulation speed.
If games run too faston your computer,
see [Games run too fast](../faq.md#games-run-too-fast) in the FAQ.

Input
-----

On Windows,
"Windows" is the only driver available,
and uses RawInput for keyboard and mouse input,
XInput for Xbox controllers,
and DirectInput for other controllers.

On Linux:

  - **udev** supports every input device,
    but requires a modern Linux system.
  - **Xlib** is the safest choice,
    but only supports keyboard and mouse input.
