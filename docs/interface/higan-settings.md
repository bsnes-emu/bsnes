The Settings window
appears when you choose
one of the items at the bottom of
[the Settings menu](higan.md#the-settings-menu),
and contains less-frequently-modified settings.
Most of these can be safely ignored,
or set once and never changed again.

This window has a tab for each main category of options.

Systems
=======

This tab configures the contents of
[the Systems menu](higan.md#the-systems-menu),
so you can make it easier to load the games you care about
and hide things that get in the way.

Each item in the list represents
a single item in the Systems menu.
If the box at the left is ticked,
that item will be included in the menu,
otherwise it will be hidden—but higan will remember its configuration
in case you want to show it again.

At the bottom left are
![up-arrow](up.png) and ![down-arrow](down.png) buttons.
These move the selected item
upward or downward in the list.

The **Append** button in the lower right
adds a new item to the end of the list.
It opens the [System Properties](#system-properties) dialog,
so you can enter the details of the new item.
If you don't want the new item to be at the end,
you can use the up and down buttons
in the lower left
to move it to its intended location.

The **Modify** button in the lower right
opens the [System Properties](#system-properties) dialog
for the selected item,
so you can make changes.

The **Remove** button in the lower right
removes the selected item from the list entirely.
Unlike hiding the item,
this forgets whatever configuration the item had.

System Properties
-----------------

This dialog appears when clicking "Append" or "Modify"
in the [Systems](#systems) tab.
It allows you to configure a new ("Append") or existing ("Modify") entry
in the [Systems menu](higan.md#the-systems-menu).

  - **System** controls which console will be emulated
    when this menu-item is chosen.
  - **Load** controls what game will be loaded
    into the emulated console
    when this menu-item is chosen.
      - If left blank,
        higan will open [a filesystem browser](common.md#the-filesystem-browser)
        allowing you to pick a previously-imported game from
        the [game library](../concepts/game-library.md).
      - If you choose a particular game,
        higan will immediately load it
        when the menu-item is chosen.
        If the game requires additional data
        (for example, the Super Game Boy requires a Game Boy cartridge)
        higan will prompt for it.
  - **Alias** controls the name of this item,
    as displayed in the Systems menu.
  - **Append** (present in "Append" mode) closes the dialog
    and adds a new item with this configuration
    to the list.
  - **Modify** (present in "Modify" mode) closes the dialog
    and updates the configuration of
    the item being modified.
  - **Cancel** closes the dialog without making any changes.

Video
=====

This tab contains options that affect
how higan displays
the emulated console's video output.

**Color Adjustment**
settings adjust the colour and brightness
of the emulated console's video output:

  - **Saturation** adjusts the vibrancy of colours displayed,
    where 0% makes things black-and-white,
    100% is normal,
    and 200% is garishly exaggerated colour.
  - **Gamma** adjusts how bright mid-range colours are
    compared to the brightest colours,
    where 100% is normal,
    and 200% makes mid-range colours much darker.
    This is in addition to
    any adjustment applied by
    the "Colors" option
    in the "Emulation" sub-menu
    of the [Settings menu](higan.md#the-settings-menu).
  - **Luminance** adjusts the overall brightness,
    where 100% is normal,
    and 0% is totally black.

**Overscan Area**
controls what parts of the video output are hidden
when "Show Overscan Area" is disabled
in the "Output" sub-menu of
the [Settings menu](higan.md#the-settings-menu).
On a standard-definition television,
the outermost edges of the emulated console's video output
would have been hidden by the bezel,
so some games (particularly on the Famicom)
allowed random glitchy output to appear there,
assuming it wouldn't be visible.

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

**Fullscreen**
settings apply
when higan is running fullscreen,
because it was started with the `--fullscreen`
[command-line option](higan-cli.md)
or because the user pressed
the Toggle Fullscreen [hotkey](higan-settings.md#hotkeys).

  - **Exclusive Mode**
    requests exclusive access
    to the computer's video output
    when higan enters fullscreen mode.
    This prevents other applications
    or the operating system itself
    from drawing anything,
    and may also temporarily disable any kind of compositing,
    reducing video latency.
    As of v107,
    only the Direct3D video driver is capable of exclusive mode;
    with other drivers this option does nothing.

Audio
=====

This tab contains options that affect
how higan reproduces
the emulated console's audio output.

**Driver**
settings affect
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

Input
=====

This tab controls
how higan handles input for the emulated consoles.

**When focus is lost**
controls what happens when a game is loaded,
but higan is not the current foreground window.

  - **Pause Emulation** automatically pauses emulation.
  - **Block Input** allows emulation to keep running,
    but higan will ignore all configured button presses.
    If you're using the keyboard to emulate a controller,
    this prevents typing in other applications
    from messing with higan,
    but music will keep playing.
  - **Allow Input** allows emulation to continue as normal.
    This allows somebody to play higan with a controller
    in one window,
    while somebody else types into another application
    in another window.

The rest of this tab configures
the mapping from PC inputs to emulated controllers.
The exact PC inputs that can be mapped
depend on [the input driver](../guides/drivers.md#input).

To choose which of the possible controllers to configure:

  - The first drop-down list controls
    which console's ports appear in the second list.
  - The second drop-down list controls
    which port's compatible controllers appear in the third list.
  - The third drop-down list controls
    which controller's inputs are shown
    in the mapping list below.

Note that some consoles only allow particular controllers
to be used in a particular port.
For example,
the Super Scope controller for the Super Famicom
only works in Controller Port 2.

To configure the selected controller:

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

**Note:**
To use an controller axis mapped to a mouse axis,
higan will need to be in fullscreen mode,
or you'll need to press
the key mapped to "Toggle Mouse Capture"
on the [Hotkeys tab](#hotkeys).

If you start mapping a button or axis,
but decide you don't want to,
you can press Escape
to exit the "Press a key or button to map..." mode
without actually mapping anything.

**Note:**
The Game Boy and Game Boy Color consoles
have a "Cartridge" port with controllers
that are not really controllers:

  - The "MBC5" controller is automatically used for
    games whose cartridge includes the MBC5 memory-mapper
    and a rumble motor,
    like *Pokémon Pinball*.
    See [Rumble Compatibility for Game Boy (Color)][gbcrumble]
    for details.
  - The "MBC7" controller is automatically used for
    games whose cartridge includes the MBC7 memory-mapper
    and an accelerometer,
    like *Kirby Tilt 'n' Tumble*.

**Note:**
The Game Boy Advance console includes
a Rumble "input" which is really more of an output.
See [Rumble Compatibility for Game Boy Advance][gbarumble]
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
instead of for the emulated console.

  - **Toggle Fullscreen** puts higan into fullscreen mode,
    where the menu and status bar are hidden,
    and the emulated console's video output
    can cover the entire screen.
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
    until the Pause Emulation hotkey is pressed a second time,
    or "Pause Emulation" is chosen from
    [the Tools menu](higan.md#the-tools-menu)..
  - **Fast Forward** disables audio and video synchronisation
    for as long as it's held down,
    so emulation proceeds as quickly as possible.
    If your PC struggles to hit "real time"
    (60fps for most emulated consoles),
    this likely won't have any effect.
  - **Soft Reset** restarts the emulated console's CPU
    while leaving the console's memory untouched,
    just like the "Soft Reset" menu item
    in [the console menu](higan.md#the-console-menu).
    This hotkey does nothing
    when the "Soft Reset" item
    does not appear in the console menu.
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

  - **Location** tells higan where to look for games to load.
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

**Other**

  - **Auto-Save Memory Periodically** makes higan write
    [in-game saves](../concepts/save-states.md#save-states-versus-in-game-saves)
    to disk during gameplay,
    instead of only when higan exits.
    This may cause stuttering,
    but means that you haven't lost everything
    if higan crashes,
    or your computer loses power.
      - Note that this does not include
        [game notes](higan-tools.md#game-notes)
