bsnes user interface
====================

This directory contains the code for
the normal bsnes user interface.

  - [bsnes.cpp](./bsnes.cpp)
    contains the `nall::main()` entry point
  - [input](./input/)
    contains the glue logic
    that maps incoming controller inputs
    into emulated controller inputs and hotkey signals
  - [presentation](./presentation/)
    contains the code that sets up the main window
    (menu bar, status bar, etc.)
  - [program](./program/)
    contains the glue logic
    that connects the UI to the emulation core,
    generating manifests,
    loading games,
    displaying video and playing audio, etc.
  - [resource](./resource/)
    contains all the data bsnes uses at runtime,
    like icons,
    the about box logos,
    the SNES board database
  - [settings](./settings/)
    contains Settings window stuff
  - [tools](./tools/)
    contains Tools window stuff

