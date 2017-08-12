When you launch higan,
the main window appears,
with a menu-bar across the top,
a status-bar across the bottom,
and a large area in the middle where the game's video output appears.

The Library menu
----------------

The Library menu allows you
to import games into higan's game library,
and to load games from the library.
higan organises the games in your library
according to which console they were intended to run on.

To play a game for a particular console from your library,
click on the Library menu,
click on the console manufacturer submenu
(Nintendo for the Super Famicom,
Bandai for the WonderSwan,
etc.)
then click on the console menu item.
A window will appear listing all the games in your library
for that particular console.
Select the game you want to play
and click the Open button,
or just double-click the game,
and it will begin playing as though you'd just turned on the console.

**Note:**
Sometimes
the NTSC and PAL variants of a particular console
behave differently,
or the Japanese and American variants.
When opening a game from the Game Library,
a drop-down list in the bottom-right of the window
allows you to choose which regional variation
of the console it should emulate.
For most consoles,
higan can reliably guess which variant to use,
and the list defaults to "Auto".

To add a new game to your library,
choose "Load ROM File ..." from the Library menu.
A [filesystem browser](#the-filesystem-browser) will appear,
allowing you to pick any ROM image for any supported system,
with any of the most common file extensions.
It also allows loading ROM images from `.zip` archives,
if the archive contains a single ROM image.

**Note:** Some games require extra steps to import correctly;
see [the Game Library](#the-game-library) for details.

To add many games at once,
run icarus,
or choose "Import ROM Files ..." from the Library menu
(which just runs icarus anyway).
See [the icarus interface](#the-icarus-interface)
for more information about bulk-importing.

For more information about the higan game library,
see [The Game Library](#the-game-library) below.

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

  - **Controller Port 1**
    allows you
    to connect different emulated controllers
    to the first controller port,
    if there is one.
      - See [the Configuration dialog](#the-configuration-dialog)
        for information about configuring
        which host controller inputs are used
        for the emulated controllers.
      - This menu appears for the Famicom,
        even though the Famicom did not support alternate controllers,
        because the Famicom emulation core also emulates the NES,
        which did.
  - **Controller Port 2**
    allows you
    to connect different emulated controllers
    to the second controller port,
    if there is one.
      - See [the Configuration dialog](#the-configuration-dialog)
        for information about configuring
        which host controller inputs are used
        for the emulated controllers.
      - This menu appears for the Famicom,
        even though the Famicom did not support alternate controllers,
        because the Famicom emulation core also emulates the NES,
        which did.
  - **Expansion Port**
    allows you
    to connect different emulated devices
    to the console's expansion port,
    if there is one.
      - For the Super Famicom,
        the [21fx][21fx] is a homebrew device
        that allows a program running on a PC
        to control a physical Super Famicom (or SNES).
        This option allows the same program
        to control the emulated SNES,
        for development or testing.
  - **Power Cycle**
    restarts the loaded game
    as though the emulated console were switched off and on again.
  - **Unload**
    stops the current game,
    as though the emulated console were switched off.
    You can load the same or a different game
    from [the Library menu](#the-library-menu).

[21fx]: https://github.com/defparam/21FX

The Settings menu
-----------------

The Settings menu allows you to configure things
that aren't specific to any particular console.

  - **Video Scale** determines the size and shape
    of the emulated console's video output
    in windowed mode
    (as opposed to fullscreen).
  - **Video Emulation** applies various effects
    to the emulated console's video output
    to reproduce some behaviours
    that aren't technically part of the console itself.
      - "Blurring" simulates the limited horizontal resolution
        of standard-definition TVs
        by blurring together horizontally-adjacent pixels.
        Games like Jurassic Park for the Super Famicom
        depend on this to emulate a transparency effect.
        For hand-held consoles like the Game Boy Advance,
        this simulates the slow response time
        of the cheap LCD screens these consoles used
        by blurring each output frame with the previous one.
      - "Colors" simulates the way a console's display device
        differs from modern computer monitor's colour reproduction.
        In particular,
        it simulates the slightly-different gamma correction
        used by the Super Famicom,
        the dim, washed out colours of the original Game Boy Advance,
        and the pea-green display of the original Game Boy.
  - **Video Shader** controls
    how the low-resolution video output of the emulated console
    is scaled up to suit modern high-resolution displays.
    The availability of items in this submenu depends on
    which video driver higan is using,
    so see [Drivers](#drivers) for more information.
      - "None" draws each output pixel according to
        the colour of the single nearest input pixel,
        sometimes called "nearest neighbour" scaling.
        This produces unnaturally crisp and blocky images.
      - "Blur" draws each output pixel by
        averaging the colours of the four nearest input pixels,
        sometimes called "bilinear" scaling.
        This produces unnaturally blurry images.
      - When using the OpenGL [driver](#drivers),
        an additional item appears in this menu for
        each installed Quark shader.
        See [Installing custom shaders](#installing-custom-shaders)
        for details.
  - **Synchronize Audio**
    causes higan to wait for audio playback to complete
    before resuming emulation.
    This should reduce popping and glitching noises,
    and slows the emulation down to approximately the correct speed.
    If your PC cannot emulate at full-speed,
    (60fps for most consoles, 75fps for WonderSwan)
    this has no noticable effect.
  - **Mute Audio**
    causes higan to not output sound from the emulated console.
    The sound hardware is still emulated.
  - **Show Status Bar**
    causes higan to show or hide the status bar
    at the bottom of the window.
    This option has no effect in full-screen mode.
    See [The status bar](#the-status-bar) for more information.
  - **Video ...**
    opens the Video tab of [the Configuration dialog][cfgdlg].
  - **Audio ...**
    opens the Audio tab of [the Configuration dialog][cfgdlg].
  - **Input ...**
    opens the Input tab of [the Configuration dialog][cfgdlg].
  - **Hotkey ...**
    opens the Hotkeys tab of [the Configuration dialog][cfgdlg].
  - **Advanced ...**
    opens the Advanced tab of [the Configuration dialog][cfgdlg].

[svsa]: #why-do-synchronize-video-and-synchronize-audio-conflict
[cfgdlg]: #the-configuration-dialog

The Tools menu
--------------

The Tools menu
contains features for manipulating the emulated console.

  - **Save Quick State**
    stores the current state of the emulated console
    into one of the quick state slots.
    See [Save States](#save-states) for more information.
  - **Load Quick State**
    restores the emulated console to
    a state previously saved to one of the quick state slots.
    See [Save States](#save-states) for more information.
  - **Cheat Editor**
    opens [the Cheat Editor window](#the-cheat-editor)
  - **State Manager**
    opens [the State Manager window](#the-state-manager)
  - **Manifest Viewer**
    opens [the Manifest Viewer window](#the-manifest-viewer)

The Help menu
-------------

The Help menu contains information about higan itself.

  - **Documentation**
    loads the official higan documentation
    in your web-browser.
  - **About**
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
the status bar displays the current emulation speeed
in frames-per-second.
For PAL-based consoles,
this should be around 50 FPS for "full speed" emulation,
for NTSC and most portable consoles the ideal speed is 60 FPS,
but the WonderSwan runs at 75 FPS.
If the number is too low,
you may need a faster computer,
or a faster [video driver](#drivers).
If the number is too high,
you may need to [Synchronize Audio](#the-settings-menu),
or you may have pressed the "turbo" [hotkey](#the-configuration-dialog).

The status bar displays "Paused"
if you have pressed the "pause" [hotkey](#the-configuration-dialog),
or if "When focus is lost: Pause Emulation" is ticked
in [the Input tab of the Configuration dialog](#the-configuration-dialog)
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
when you choose "Power Cycle" from [the console menu](#the-console menu),
or press the "Power Cycle" hotkey.

The status bar briefly displays "Display rotation not supported"
when you press the "Rotate Display" hotkey
while the emulated console does not support display rotation.
