When you launch higan
the main window appears,
with a menu-bar across the top,
a status-bar across the bottom,
and a large area in the middle that shows
the running game's video output.

The Library menu
----------------

Manufacturer sub-menus
allow you to play
games you've already imported
into higan's 
[game library](../concepts/game-library.md).
See [Importing and playing games](../guides/import.md).
    
**Load ROM File ...**
opens a [filesystem browser](common.md#the-filesystem-browser)
allowing you to choose a single ROM file.
It will be imported and immediately start playing.
See [Importing and playing games](../guides/import.md).
    
**Import ROM Files ...**
launches the icarus importing tool,
allowing you to bulk-import many ROM files at once.
See [the icarus documentation](icarus.md).
    

The console menu
---------------

**Note:**
The console menu does not appear
until a game is loaded.
Also,
it's not named "console",
it's named for the kind of console
the loaded game runs on.
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

**Power Cycle**
restarts the loaded game
as though the emulated console were switched off and on again.

**Unload**
stops the current game,
as though the emulated console were switched off.
You can load a new game
from [the Library menu](#the-library-menu).

[21fx]: https://github.com/defparam/21FX

The Settings menu
-----------------

The Settings menu allows you to configure things
that aren't specific to any particular console.


**Video Scale** determines the size
of the emulated console's video output
when higan is running in windowed mode
(as opposed to fullscreen).

**Video Emulation** applies various effects
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
        Jurassic Park for the Super Famicom
        or Chikyuu Kaihou Gun ZAS for the Game Boy
        depend on this to emulate a transparency effect.
  - **Colors**
    simulates the way a console's display device
    differs from modern computer monitor's colour reproduction.
    In particular,
    it simulates the slightly-different gamma correction
    used by the Super Famicom,
    the dim, washed out colours of the original Game Boy Advance,
    and the pea-green display of the original Game Boy.
    
**Video Shader** controls
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

**Cheat Editor**
opens [the Cheat Editor tab](higan-tools.md#the-cheat-editor)
of the Tools window.

**State Manager**
opens [the State Manager tab](higan-tools.md#the-state-manager)
of the Tools window.

**Manifest Viewer**
opens [the Manifest Viewer tab](higan-tools.md#the-manifest-viewer)
of the Tools window.

The Help menu
-------------

The Help menu contains information about higan itself.

**Documentation**
loads the official higan documentation
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
the status bar displays "No cartridge loaded".

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
or if "When focus is lost: Pause Emulation" is ticked
in [higan's Input settings](higan-settings.md#input)
and the main higan window is not the foreground window.
To resume emulation,
make sure the main higan window is in the foreground,
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

The status bar briefly displays "Power cycled"
when you choose "Power Cycle" from [the console menu](#the-console-menu),
or press the "Power Cycle" hotkey.

The status bar briefly displays "Display rotation not supported"
when you press the "Rotate Display" hotkey
while the emulated console does not support display rotation.
