When you launch higan
the main window appears,
with a menu-bar across the top,
a status-bar across the bottom,
and a large area in the middle that shows
the running game's video output.

The Systems menu
----------------

This menu lists the systems higan emulates.
Choosing any system from this menu allows you to play
games for that system that you've already imported
into higan's [game library](../concepts/game-library.md).
See [Importing and playing games](../guides/import.md).

You can customise this menu
in [higan's Systems settings](higan-settings.md#systems)
to hide systems you don't care about,
or add a specific cartridge for any supported system.
This makes it more convenient
to play games that involve mini-cartridges:
for example, you can
add the Sufami Turbo to the list
and load *SD Ultra Battle*
in two clicks instead of three.

**Load ROM File ...**
opens a [filesystem browser](common.md#the-filesystem-browser)
allowing you to choose a single ROM file.
It will be imported and immediately start playing.
See [Importing and playing games](../guides/import.md).

The console menu
---------------

**Note:**
The console menu does not appear
until a game is loaded.
Also,
it's not named "console",
it's named for the console that runs the loaded game.
For example,
when playing a Game Boy game,
you will have a "Game Boy" menu.

The console menu contains commands relevant
to the particular console being emulated.
All consoles will have some of the following items,
but few consoles have all of them.

**Controller Port 1**
allows you
to connect different emulated controllers
to the first controller port,
if there is one.
See [higan's Input settings](higan-settings.md#input)
for information about configuring
which PC controller inputs are used
for the emulated controllers.

This menu appears for the Famicom,
even though the Famicom did not support alternate controllers,
because the Famicom emulation core also emulates the NES,
which did.

**Controller Port 2**
allows you
to connect different emulated controllers
to the second controller port,
if there is one.
See [higan's Input settings](higan-settings.md#input)
for information about configuring
which PC controller inputs are used
for the emulated controllers.

This menu appears for the Famicom,
even though the Famicom did not support alternate controllers,
because the Famicom emulation core also emulates the NES,
which did.

**Controller**
is like "Controller Port 1"
for consoles that only have one controller port.

**Hardware**
appears for consoles with buttons on the main unit,
like the Game Boy,
or Master System.
It only allows the built-in controls to be used.

**Cartridge**
appears for the Game Boy and Game Boy Colour.
The options inside it do nothing.

**Expansion Port**
allows you
to connect different emulated devices
to the console's expansion port,
if there is one.

For the Super Famicom,
the [21fx][21fx] is a homebrew device
that allows a program running on a PC
to control a physical Super Famicom (or SNES).
This option allows the same program
to control the emulated SNES,
for development or testing.

**Extension Port**
is the name the Sega Mega Drive used for its expansion port.

**Soft Reset**
restarts the emulated console's CPU
while leaving the console's memory untouched,
like pressing the "reset" button
on a physical console.

This menu item does not appear
for consoles that did not have a "reset" button,
like hand-helds.

It also does not appear for the Sega Master System,
since that console's reset button is wired up like a controller
rather than directly attached to the CPU.
To reset the Master System,
bind a keyboard or joypad button
to the "Reset" function
on the "Controls" controller
in the "Hardware" port
of the Sega Master System
in [higan's Input settings](higan-settings.md#input).

**Power Cycle**
restarts the loaded game
as though the emulated console were switched off and on again.

**Unload**
stops the current game,
as though the emulated console were switched off.
You can load a new game
from [the Systems menu](#the-systems-menu).

[21fx]: https://github.com/defparam/21FX

The Settings menu
-----------------

The Settings menu allows you to configure things
that aren't specific to any particular console.

**Size**
determines the size
of the emulated console's video output
when higan is running in windowed mode
(as opposed to fullscreen).
The menu-items that indicate particular sizes
are only approximate, since
aspect correction can be applied,
different consoles have different native image sizes,
and some consoles can change the size of their output image dynamically.

  - **1x (240p)**
    resizes the higan window
    so that each pixel of the emulated console's video output
    is drawn as a single pixel on the computer screen.
  - **2x (480p)**
    resizes the higan window
    so that each pixel of the emulated console's video output
    is drawn as a 2×2 block of pixels on the computer screen.
  - **3x (720p)**
    resizes the higan window
    so that each pixel of the emulated console's video output
    is drawn as a 3×3 block of pixels on the computer screen.
  - **Shrink Window To Size**
    resizes the higan window to fit the emulated console's video output
    at its current scale,
    so there's no black padding between the image and the window border
    (some padding may remain
    if "Show Overscan Area" is enabled
    in the Output menu).
  - **Center Window**
    moves the higan window to the centre of the computer screen.

**Output**
controls how higan draws the emulated console's video output
into the space available,
in both windowed and fullscreen modes.

  - **Center**
    draws the emulated video
    at the largest integer multiple of the native size that will fit,
    centered in the space available.
    This gives the most crisp output,
    but often has black borders.
  - **Scale**
    draws the emulated video
    at the largest size that will fit,
    and which preserves the image's aspect ratio.
    This strikes a balance between
    displaying the video output as it was intended,
    and eliminating black borders.
  - **Stretch**
    draws the emulated video
    to cover the entire available output area,
    even if that distorts the image.
    This completely eliminates black borders,
    but can look very weird.
  - **Adaptive Sizing**
    allows higan to resize its window
    when the emulated console changes the resolution
    of its video output.
    This can avoid black borders,
    but the window resizing itself might be even more distracting.
  - **Aspect Correction**
    horizontally stretches the emulated video output
    to match the aspect ratio produced by the original console.
    It can make the output look more "lumpy",
    but is a more accurate representation
    of the original console's output.
  - **Show Overscan Area**
    controls whether the area defined by
    the "Overscan Area" sliders in
    the [Video settings](higan-settings.md#video)
    is clipped from the emulated video output
    or shown.

**Emulation** applies various effects
to the emulated console's video output
to reproduce some behaviours
that aren't technically part of the console itself:

  - **Blurring**
    simulates the limited horizontal resolution
    of standard-definition TVs
    by blurring together horizontally-adjacent pixels.
    For hand-held consoles,
    this simulates the slow response time
    of the cheap LCD screens these consoles used
    by blending each output frame with the previous one.
      - Games like
        *Jurassic Park* for the Super Famicom
        or *Chikyuu Kaihou Gun ZAS* for the Game Boy
        depend on this to emulate a transparency effect.
  - **Colors**
    simulates the way a console's display device
    differs from modern computer monitor's colour reproduction.
    In particular,
    it simulates the slightly-different gamma correction
    used by the Super Famicom,
    the dim, washed out colours of the original Game Boy Advance,
    and the pea-green display of the original Game Boy.

**Shader** controls
how the low-resolution video output of the emulated console
is scaled up to suit modern high-resolution displays.
[Using video shaders](../guides/shaders.md)
describes all the options in this sub-menu.

**Synchronize Audio**
causes higan to wait for audio playback to complete
before resuming emulation.
This should reduce popping and glitching noises,
and slows the emulation down to approximately the correct speed.
If your PC cannot emulate at full-speed,
(60fps for most consoles, 75fps for WonderSwan)
this has no noticeable effect.

**Mute Audio**
causes higan to not output sound from the emulated console.
The sound hardware is still emulated.

**Show Status Bar**
causes higan to show or hide the status bar
at the bottom of the window.
This option has no effect in fullscreen mode.
See [The status bar](#the-status-bar) for more information.

**Systems ...**
opens [higan's Systems settings](higan-settings.md#systems).

**Video ...**
opens [higan's Video settings](higan-settings.md#video).

**Audio ...**
opens [higan's Audio settings](higan-settings.md#audio).

**Input ...**
opens [higan's Input settings](higan-settings.md#input).

**Hotkeys ...**
opens [higan's Hotkeys settings](higan-settings.md#hotkeys).

**Advanced ...**
opens [higan's Advanced settings](higan-settings.md#advanced).

[svsa]: #why-do-synchronize-video-and-synchronize-audio-conflict

The Tools menu
--------------

The Tools menu
contains features for manipulating the emulated console.

**Save Quick State**
stores the current state of the emulated console
into one of the quick state slots.
See [Save States](../concepts/save-states.md) for more information.

**Load Quick State**
restores the emulated console to
a state previously saved to one of the quick state slots.
See [Save States](../concepts/save-states.md) for more information.

**Pause Emulation**
pauses the emulated console
until this menu-item is selected again.
This can also be triggered by
the [pause hotkey](higan-settings.md#hotkeys).

**Cheat Editor ...**
opens the [Cheat Editor tab](higan-tools.md#cheat-editor)
of the Tools window.

**State Manager ...**
opens the [State Manager tab](higan-tools.md#state-manager)
of the Tools window.

**Manifest Viewer ...**
opens the [Manifest Viewer tab](higan-tools.md#manifest-viewer)
of the Tools window.

**Game Notes ...**
opens [the Game Notes tab](higan-tools.md#game-notes)
of the Tools window.

The Help menu
-------------

The Help menu contains information about higan itself.

**Documentation**
loads the official higan documentation
in your web-browser.

**Credits**
loads a list of people who have contributed to higan
in your web-browser.

**About**
opens the About dialog,
which displays basic information about higan,
including the version number.

The status bar
--------------

The status bar appears
at the bottom of the main higan window,
while "Show Status Bar" is ticked in [the Settings menu](#the-settings-menu).

Before any game is loaded,
the status bar displays "Unloaded".

When a game is loaded and running,
the status bar displays the current emulation speed
in frames-per-second.
For PAL-based consoles,
this should be around 50 FPS for "full speed" emulation,
for NTSC and most portable consoles the ideal speed is 60 FPS,
but the WonderSwan runs at 75 FPS.
If the number is too low,
you may need a faster computer,
or a faster [video driver](../guides/drivers.md#video).
If the number is too high,
you may need to [Synchronize Audio](#the-settings-menu),
or you may have pressed the "turbo" [hotkey](higan-settings.md#hotkeys).

The status bar displays "Paused"
if you have pressed the "pause" [hotkey](higan-settings.md#hotkeys),
selected "Pause Emulation" from [the Tools menu](#the-tools-menu),
or if "When focus is lost: Pause Emulation" is ticked
in [higan's Input settings](higan-settings.md#input)
and the main higan window is not the foreground window.
To resume emulation,
make sure the main higan window is in the foreground,
select "Pause Emulation" from the Tools menu again,
and/or press the "pause" hotkey.

The status bar briefly displays "Selected quick state slot X"
(where X is one of the Quick State slot numbers)
when you press the "Increment Quick State"
or "Decrement Quick State"
hotkeys,
to show which Quick State slot will be used
the next time you press the "Save Quick State"
or "Load Quick State" hotkeys.

The status bar briefly displays "Slot X quick state does not exist"
(where X is one of the Quick State slot numbers)
when you choose a slot from the
[Tools](#the-tools-menu) → "Load Quick State"
sub-menu that has not had a save-state saved to it,
or when you press the "Load Quick State" hotkey
while the current Quick State slot has not had a save-state saved to it,

The status bar briefly displays "System has been power cycled"
when you choose "Power Cycle" from [the console menu](#the-console-menu),
or press the "Power Cycle" hotkey.

The status bar briefly displays "Display rotation not supported"
when you press the "Rotate Display" hotkey
while the emulated console does not support display rotation.
