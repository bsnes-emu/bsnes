The Settings window
appears when you choose
one of the items at the bottom of
[the Settings menu](higan.md#the-settings-menu),
and contains less-frequently-modified settings.
Most of these can be safely ignored,
or set once and never changed again.

This window has a tab for each main category of options:

Video
=====

This tab contains options that affect
how higan displays
the emulated console's video output.

**Color Adjustment**
settings adjust the colour and brightness
of the emulated console's video output:

  - **Saturation** adjusts the vibrancy of colours displayed,
    where 0% makes things pure grey,
    100% is normal,
    and 200% is garishly brightly coloured.
  - **Gamma** adjusts how bright mid-range colours are
    compared to the brightest colours,
    where 100% is normal,
    and 200% makes mid-range colours much darker.
    This is in addition to
    any adjustment applied by
    the "Colors" option
    in the "Video Emulation" submenu
    of the [Settings menu](higan.md#the-settings-menu).
  - **Luminance** adjusts the overall brightness,
    where 100% is normal,
    and 0% is totally black.

**Overscan Mask**
removes parts of
the video output that would have been hidden
by the bezel around the edge of
a standard-definition television screen.
Some games (particularly on the Famicom)
displayed random glitchy output in this area,
which can be distracting.

  - **Horizontal**
    removes pixels from the left and right of the video output.
  - **Vertical**
    removes pixels from the top and bottom of the video output.

The units are "pixels in the emulated console's standard video-mode".
For example, setting "Horizontal" to 8
will clip 8/256ths from the left and right sides
of the Super Famicom's video output,
whether the Super Famicom is in
lo-res (256px) or hi-res (512px)
mode.

**Windowed Mode**
settings apply when higan is running
in a normal window.

  - **Aspect Correction**
    stretches the image to match the aspect ratio
    produced by the original console hardware,
    but can cause a "ripple" effect
    during horizontal scrolling
    due to rounding errors.
    [Video shaders](../guides/shaders.md)
    can reduce this effect.
  - **Integral Scaling**
    makes higan draw the emulated video output
    at a whole-number multiple of the original size,
    rather than completely filling the available space.
    This means that every game pixel
    uses the same number of computer pixels,
    and avoids graphics looking chunky and uneven.
    Note that Aspect Correction
    is applied after integral scaling,
    so some unevenness may be visible
    even with this option enabled.
  - **Adaptive Sizing**
    automatically resizes the higan window
    to fit snugly around the emulated video output
    whenever it changes size
    (because the user loaded a game for a different console,
    chose a different option from
    the [Video Scale submenu](higan.md#the-settings-menu),
    toggled Aspect Correction, etc.)
    When disabled,
    higan generally respects manual resizing.

**Fullscreen Mode**
settings apply
when higan is running fullscreen,
because it was started with the `--fullscreen`
[command-line option](higan-cli.md)
or because the user pressed
the Toggle Fullscreen [hotkey](higan-settings.md#hotkeys).

  - **Aspect Correction**
    behaves the same way as in Windowed mode above.
  - **Integral Scaling**
    behaves the same way as in Windowed mode above.
  - **Exclusive Mode**
    requests exclusive access
    to the computer's video output
    when higan enters fullscreen mode.
    This prevents other applications
    or the operating system itself
    from drawing anything,
    and may also temporarily disable any kind of compositing,
    reducing video latency.
    As of v104,
    only the Direct3D video driver is capable of exclusive mode;
    with other drivers this option does nothing.

Audio
=====

This tab contains options that affect
how higan reproduces
the emulated console's audio output.

**Driver**
settings affect the
the current [Audio driver](../guides/drivers.md).

  - **Device** allows you to choose
    which audio device higan sends
    the emulated game's audio to,
    if you have more than one.
  - **Frequency** controls the sample-rate that higan will use
    when generating audio.
    If your PC's audio hardware has a "native" sample-rate
    and you know what it is,
    pick that.
    Otherwise,
    44.1kHz or 48kHz should be fine.
  - **Latency** controls how much audio output higan calculates in advance.
    Higher values reduce the chance of
    "popping" or "glitching" noises,
    but increase the delay between an action occurring on-screen
    and the corresponding sound-effect being played.
  - **Exclusive Mode** appears
    if the current audio driver
    allows higan to take exclusive control of your PC's audio output,
    so no other applications can play sounds.
    This can improve audio quality,
    and lower the effective audio latency.

**Effects**
are applied to the emulated audio output
before it is sent to your computer's speakers.

  - **Volume** controls the overall loudness of
    the emulated console's audio,
    where 100% is normal volume,
    and 0% is complete silence.
  - **Balance** controls the relative loudness of
    the left and right speakers,
    where 0% means only the left speaker produces sound,
    50% means both speakers produce sound equally,
    and 100% means only the right speaker produces sound.
  - **Reverb** adds a slight reverberation effect
    to the emulated console's audio output,
    as though you were playing the game in a tunnel or small room.

Input
=====

This tab controls which PC inputs
are mapped to which emulated controllers.
The exact PC inputs that can be mapped
depend on [the input driver](../guides/drivers.md#input).

General input settings:

  - **Pause Emulation** automatically pauses emulation
    when the main higan window
    is not the current foreground window.
  - **Allow Input** can be ticked
    when "Pause Emulation" is *not* ticked,
    and allows configured inputs to keep affecting higan
    even when higan is running in the background.
    This is particularly relevant if
    you configure your PC keyboard to control higan:
    if you tick this box,
    and switch to a different application
    leaving higan running in the background,
    typing in that other application may affect
    the emulated game running in higan
    even though you can't see it!

Choosing which of the possible controllers to configure:

  - The console selector chooses which console's inputs
    to display in the mapping list below.
  - The port selector chooses which port of the selected console
    to display in the mapping list below.
  - The controller selector chooses which controller
    associated with the given console and port
    to display in the mapping list below.
    Note that some consoles only allow particular controllers
    to be used in a particular port.
    For example,
    the Super Scope controller for the Super Famicom
    only works in Controller Port 2.

Configuring the selected controller:

  - The mapping list includes
    every button and axis on the selected controller,
    and the PC inputs that will be mapped to it
    when it is connected to the selected port of the selected console.
  - **Erase** removes the mapping
    for the selected button or axis.
  - **Reset** removes all the mappings currently in the list.

To map
a keyboard or gamepad button on your PC to
a controller button,
double-click the controller button in the list,
or select it and press Enter.
The Settings window will grey out,
and a message will appear in the bottom left:
"Press a key or button to map [the button]".
Press the key or button you want to map,
and it should appear in the list
next to the controller button it is mapped to.

To map
a mouse button on your PC to
a controller button,
select the controller button in the list,
then click one of the "Mouse Left",
"Mouse Middle",
or "Mouse Right" buttons in the bottom-left of the Settings window.

To map
a joystick axis on your PC to
a controller axis,
double-click the axis in the list,
or select it and press Enter.
The Settings window will grey out,
and a message will appear in the bottom left:
"Press a key or button to map [the axis]".
Press the joystick in the direction you want to map,
and it should appear in the list
next to the controller axis it is mapped to.

To map
a mouse axis on your PC to
a controller axis,
select the axis in the list,
then click one of the
"Mouse X-axis",
or "Mouse Y-axis"
buttons in the bottom-left of the window.

If you start mapping a button or axis,
but decide you don't want to,
you can press Escape
to exit the "Press a key or button to map..." mode
without actually mapping anything.

**Note:**
Consoles in the Game Boy family include
a Rumble "input" which is really more of an output.
See [Rumble Compatibility for Game Boy (Color)][gbcrumble]
and [Rumble Compatibility for Game Boy Advance][gbarumble]
for details.

[gbcrumble]: ../notes.md#rumble-compatibility-for-game-boy-color
[gbarumble]: ../notes.md#rumble-compatibility-for-game-boy-advance

**Note:**
Once you've configured which computer inputs
higan should route to which controllers,
make sure the controller in question
is actually connected to the correct controller port
in [the console menu](higan.md#the-console-menu).

Hotkeys
=======

This tab is like "Inputs" above,
except it contains controls for higan itself
instead of the emulated console.

  - **Toggle Fullscreen** puts higan into fullscreen mode,
    where the menu and status bar are hidden,
    and the emulated console's video output
    is enlarged to cover the entire screen.
    Toggling fullscreen also automatically captures the mouse.
  - **Toggle Mouse Capture** hides the usual mouse-cursor,
    and captures the mouse so it cannot leave the higan window.
    This is useful when the mouse is being used to emulate
    a light-gun controller like the Super Scope.
  - **Save Quick State** saves the current state of the emulated console
    to the currently-selected [Quick State][qstates] slot.
  - **Load Quick State** restores the emulated console
    to the state saved in the currently-selected [Quick State][qstates] slot.
  - **Decrement Quick State** selects the previous [Quick State][qstates] slot.
    The status bar will briefly display the new current slot number.
  - **Increment Quick State** selects the next [Quick State][qstates] slot.
    The status bar will briefly display the new current slot number.
  - **Pause Emulation** pauses the emulated console
    until the Pause Emulation hotkey is pressed a second time.
  - **Fast Forward** disables audio and video synchronisation
    for as long as it's held down,
    so emulation proceeds as quickly as possible.
    If your PC struggles to hit "real time"
    (60fps for most emulated consoles),
    this likely won't have any effect.
  - **Power Cycle** turns the emulated console off and back on
    (a "hard reset"),
    just like the "Power Cycle" menu item
    in [the console menu](higan.md#the-console-menu).
  - **Rotate Display** will rotate the display
    of the Game Boy Advance
    and WonderSwan (Color).
    See [Game Boy Advance rotation](../notes.md#game-boy-advance-rotation)
    and [WonderSwan rotation](../notes.md#wonderswan-rotation)
    for details.

[qstates]: ../concepts/save-states.md#quick-states

Advanced
========

This tab contains all the settings
that didn't fit into one of the other categories.

**Driver Selection**
tells higan how to
accept input,
display video,
and play sound
on this computer.

  - **Video** controls how higan will draw
    the emulated console's video output
    to the PC screen.
    "None" means no video will be drawn.
  - **Audio** controls how higan will present
    the emulated console's audio output.
    "None" means no audio will be played.
  - **Input** controls how higan checks for input
    from the PC's input devices.
    "None" means the emulated console cannot be controlled.

See [Choosing drivers](../guides/drivers.md)
for help choosing which drivers you should use.

**Game Library**
configures how higan interacts
with the [Game Library](../concepts/game-library.md).

  - **Location** selects where higan
    looks for games to load.
    See [Moving the Game Library](../concepts/game-library.md#moving-the-game-library)
    for more information.
  - **Ignore Manifests** makes higan ignore
    the [manifest](../concepts/manifests.md) file
    in the loaded game's
    [game folder](../concepts/game-folders.md)
    in favour of asking icarus
    to guess a manifest on the fly.
    See [Ignoring manifests](../concepts/manifests.md#ignoring-manifests)
    for details.
