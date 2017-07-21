TODO

Note that when changing a driver,
you must restart higan for the change to take effect.

Video
-----

TODO

The best option is "OpenGL"
(since it allows you to use
[custom shaders](#installing-custom-shaders)),
and the safest is "Direct Draw" (for Windows)
or SDL (for Linux).

Audio
-----

TODO

On Linux,
"PulseAudioSimple" is the most likely to work.

On Windows, "DirectSound" is probably what you want.
"XAudio" targets XAudio 2.7,
(the last version to work on Windows 7),
so it requires the latest (June 2010) version
of the [DirectX 9 End-User Runtime][dx9]
to be installed.

[dx9]: https://www.microsoft.com/en-us/download/details.aspx?id=35

Input
-----

TODO

On Linux,
"udev" is the most flexble,
but requires a modern Linux system,
while "Xlib" should work on other Unix-like OSs
but only supports a mouse and keyboard.

On Windows,
"Windows" is the only input driver available,
and automatically uses RawInput for keyboard/mouse,
XInput for Xbox controllers,
and DirectInput for other controllers.
