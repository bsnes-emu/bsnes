The Configuration dialog
contains less-frequently-modified configuration options.
Most of these can be safely ignored,
or set once and never changed again.

TODO: Break this into separate sections for each tab,
like for the tools window.

The dialog has a tab for each main category of options:

  - **Video**: This tab contains options that affect
    how higan displays
    the emulated console's video output.
      - "Saturation" adjusts the vibrancy of colours displayed,
        where 0% makes things pure grey,
        100% is normal,
        and 200% is garishly brightly coloured.
      - "Gamma" adjusts how bright mid-range colours are
        compared to the brightest colours,
        where 100% is normal,
        and 200% makes mid-range colours much darker.
      - "Luminance" adjusts the overall brightness,
        where 100% is normal,
        and 0% is totally black.
      - "Overscan Mask" hides parts of
        the video output that would have been hidden
        by the bezel around the edge of
        a standard-definition television screen.
        Some games (particularly on the Famicom)
        displayed random glitchy output in this area,
        which can be distracting.
        The units are "pixels in the emulated console's standard video-mode".
        For example, setting "Horizontal" to 8
        will clip 8/256ths from the left and right sides
        of the Super Famicom's video output,
        whether the Super Famicom is in
        lo-res (256px) or hi-res (512px)
        mode.
      - "Aspect Correction"
        (in both Windowed Mode and Fullscreen Mode)
        stretches the image to match the aspect ratio
        produced by the original console hardware,
        but can cause a "ripple" effect,
        due to rounding errors.
      - "Resize Window to Viewport"
        (under "Windowed mode")
        causes higan to resize its window
        to fit snugly around the emulated console's video
        whenever it changes size:
        because a game was loaded for a different console
        with a different display size or aspect ratio,
        because the "Overscan Mask" controls were adjusted,
        because the game switched to a different video mode,
        because the user pressed the "Rotate Display" hotkey,
        etc.
        When this option is disabled,
        the higan window stays at a fixed size,
        large enough to contain the video for any supported console,
        padded with black borders for all smaller video modes.
      - "Resize Viewport to Window"
        (under "Fullscreen mode")
        causes higan to stretch the emulated console's video output
        to touch the edges of the screen.
        Since most screens are not an exact multiple
        of the size of all emulated consoles,
        this may cause a "ripple" effect,
        due to rounding errors.
        When this option is disabled,
        higan stretches the emulated console's video output
        to the largest exact multiple
        of the emulated console's video output
        that is smaller than or equal to the screen size.
      - TODO: Update this to match 103r11, or whatever the latest version is.
  - **Audio**: This tab contains options that affect
    how higan reproduces
    the emulated console's audio output.
      - "Device" allows you to choose
        which audio device higan sends
        the emulated game's audio to.
      - "Frequency" controls the sample-rate that higan will use
        when generating audio.
        If your PC's audio hardware has a "native" sample-rate
        and you know what it is,
        pick that.
        Otherwise,
        44.1kHz or 48kHz should be fine.
      - "Latency" controls how much audio output higan calculates in advance.
        Higher values reduce the chance of
        "popping" or "glitching" noises,
        but increase the delay between an action occurring on-screen
        and the corresponding sound-effect being played.
      - "Exclusive Mode" appears
        if the current audio driver
        allows higan to take exclusive control of your PC's audio output,
        so no other applications can play sounds.
        This can improve audio quality,
        and lower the effective audio latency.
      - "Volume" controls the overall loudness of
        the emulated console's audio,
        where 100% is normal volume,
        and 0% is complete silence.
      - "Balance" controls the relative loudness of
        the left and right speakers,
        where 0% means only the left speaker produces sound,
        50% means both speakers produce sound equally,
        and 100% means only the right speaker produces sound.
      - "Reverb" adds a slight reverberation effect
        to the emulated console's audio output,
        as though the console were in a tunnel or small room.
  - **Input**: This tab controls which PC inputs
    are used for which emulated controllers.
    The exact PC inputs that can be mapped
    depend on [the input driver](#drivers).
      - "Pause Emulation" automatically pauses emulation
        when the main higan window
        is not the current foreground window.
      - "Allow Input" can be ticked
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
      - The console selector chooses which console's inputs
        to display in the mapping list below.
      - The port selector chooses which port of the selected console
        to display in the mapping list below.
      - The controller selector chooses which controller
        associated with the given console and port
        to display in the mapping list below.
      - The mapping list includes
        every button and axis on the selected controller,
        and the PC inputs that are mapped to it
        when it is connected to the selected port of the selected console.
      - To map
        a keyboard or gamepad button on your PC to
        a controller button,
        double-click the controller button in the list,
        or select it and press Enter.
        The window will grey out,
        and a message will appear in the bottom left:
        "Press a key or button to map [the button]".
        Press the key or button you want to map,
        and it should appear in the list
        next to the controller button it is mapped to.
      - To map
        a mouse button on your PC to
        a controller button,
        select the controller button in the list,
        then click one of the "Mouse Left",
        "Mouse Middle",
        or "Mouse Right" buttons in the bottom-left of the window.
      - To map
        a joystick axis on your PC to
        a controller axis,
        double-click the axis in the list,
        or select it and press Enter.
        The window will grey out,
        and a message will appear in the bottom left:
        "Press a key or button to map [the axis]".
        Press the joystick in the direction you want to map,
        and it should appear in the list
        next to the controller button it is mapped to.
      - To map
        a mouse axis on your PC to
        a controller axis,
        select the axis in the list,
        then click one of the
        "Mouse X-axis",
        or "Mouse Y-axis"
        buttons in the bottom-left of the window.
      - The "Rumble" setting
        for the Game Boy Advance is treated like a button,
        and can be mapped to a PC gamepad.
        When the emulated Game Boy Advance
        tries to use the rumble feature
        of the Game Boy Player,
        higan will turn on the force-feedback
        of whatever gamepad the mapped button is part of.
      - If you start mapping a button or axis,
        but decide you don't want to,
        you can press Escape
        to exit the "Press a key or button to map..." mode
        without actually mapping anything.
      - "Erase" removes the mapping
        for the selected button or axis.
      - "Reset" removes all the mappings currently in the list.
      - TODO: Mention that controllers must be connected
        in the console menu
        before they can be used.
  - **Hotkeys**: This tab is like "Inputs" above,
    except it contains controls for higan itself,
    instead of for the emulated console.
      - "Toggle Fullscreen" puts higan into fullscreen mode,
        where the menu and status bar are hidden,
        and the emulated console's video output
        is enlarged to cover the entire screen.
        Toggling fullscreen also automatically captures the mouse.
      - "Toggle Mouse Capture" hides the usual mouse-cursor,
        and captures the mouse so it cannot leave the higan window.
        This is useful when the mouse is being used to emulate
        a light-gun controller like the Super Scope.
      - "Save Quick State" saves the current state of the emulated console
        to the currently-selected Quick State slot.
      - "Load Quick State" restores the emulated console
        to the state saved in the currently-selected Quick State slot.
      - "Decrement Quick State" selects the previous Quick State slot.
        The status bar will briefly display the new current slot number.
      - "Increment Quick State" selects the next Quick State slot.
        The status bar will briefly display the new current slot number.
      - "Pause Emulation" pauses the emulated console
        until the Pause Emulation hotkey is pressed a second time.
      - "Fast Forward" disables audio and video synchronisation
        for as long as it's held down,
        so emulation proceeds as quickly as possible.
        If your PC struggles to hit "real time"
        (60fps for most emulated consoles),
        this likely won't have any effect.
      - "Power Cycle" turns the emulated console off and back on,
        (a "hard reset"),
        just like the "Power Cycle" menu item
        in [the console menu](#the-console-menu).
      - "Rotate Display" will toggle the display
        of the Game Boy Advance
        and WonderSwan (Color)
        between the usual landscape orientation
        and a portrait orientation (90° counter-clockwise).
        These consoles have games
        that expect the player to hold the console
        in a different way.
  - **Advanced**: This tab contains all the settings
    that didn't fit into one of the other categories.
      - "Video" controls how higan will draw
        the emulated console's video output
        to the PC screen.
        "None" means no video will be drawn.
        See [Drivers](#drivers) for details.
      - "Audio" controls how higan will present
        the emulated console's audio output.
        "None" means no audio will be played.
        See [Drivers](#drivers) for details.
      - "Input" controls how higan checks for input
        from the PC's input devices.
        "None" means the emulated console cannot be controlled.
        See [Drivers](#drivers) for details.
      - "Location" selects where the [Game Library](#the-game-library)
        looks for games to load.
        See [Moving the Game Library](#moving-the-game-library)
        for more information.
      - "Ignore Manifests" makes higan ignore the manifest file
        in the a loaded game's [game folder](#why-game-folders)
        in favour of asking icarus
        to guess a manifest on the fly.
        This means that incompatible or incorrect manifests
        generated by old versions of icarus
        won't cause problems,
        but means you can't fix incorrect manifests
        generated by the current version of icarus.
        See also the "Create Manifests" option in
        [the icarus Settings dialog](#the-icarus-settings-dialog).
