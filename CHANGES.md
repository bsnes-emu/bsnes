# Change Log

## Version 0.6

### New/Improved Features
 * Conditional r/w/rw debugger watchpoints
 * Added the "!=" operator to the debugger
 * Redefined the debugger input and output syntax, being more consistent with RGBDS
 * Debugger now reads .sym files (Tested with 3 popular formats). It shows symbols when possible, and allows using them in expressions
 * Debugger data types can now be either 16-bit values, or 25-bit full addresses that include a bank identifier
 * Debugger commands and expressions support full addresses as parameters. This allows, for example, breaking on an address on a specific ROM bank, or reading a value from a specific MBC RAM bank.
 * Some debugger commands may now run without breaking the debugger. (Similar to LLDB or GDB's async mode)
 * SDL port now supports save states
 * Improved performance by about 6% by enabling link-time optimizations
 * Reduced file size by stripping executables
 * Several Cocoa-only UX improvements:
   * Prevented being paused "twice" (Both by the pause option and the debugger)
   * Optional proportional resizing
   * Proper fullscreen support
   * Mouse cursor is now hidden while running

### Accuracy Improvements/Fixes
This version includes major accuracy improvements, which allow it to pass 54 tests out of [mooneye-gb](https://github.com/Gekkio/mooneye-gb)'s 58 acceptance tests (2 of which fail due to not including the original boot ROM); more than any other emulator.

 * Rewritten MBC support, with MBC1M support (Fixing some N-in-1 cartridges)
 * Major accuracy improvements to OAM DMA.
 * Corrected a lot of instruction memory-access timings
 * Corrected some IO register masks
 * Major accuracy improvements to timers (TIMA/DIV)

### Bug Fixes
 * Corrected operator priorities in the debugger
 * Fixed a bug where a breakpoint might have been ignored
 * Reduced CPU usage when running games with a real time clock
 * Handling ^C and ^D more sanely in SDL port
 * Fixed memory leak in Cocoa
 
### Misc Internal Changes
 * Added infrared API, but it is not actually used by any of the GUIs
 * Fixed build system bugs that caused needless dep file generation, and made the Cocoa build fail sometimes if SDL is not installed

## Version 0.5
This version is not compatible with save states of older versions.

### New/Improved Features
 * Updated save state format, should now be much more future compatible
 * Save state compatibility between 32 and 64 bit versions of SameBoy
 * Cocoa version is now using OpenGL 3
 * HQ2x filter added (Cocoa only)
 * A new, redesigned OmniScale filter; old filter is renamed to OmniScale Legacy (Cocoa only)
 * Cocoa port now "remembers" the last window size
 * Added boolean operators to the debugger
 * Added conditional breakpoints

### Accuracy Improvements/Fixes
 * Better emulation of certain behaviors of the SCX register
 * Fixed emulation of the STOP instruction
 * Minor fix to the accuracy of the JOYPAD register
 * Minor improvements to HDMA accuracy
 

### Bug Fixes
 * Improved concurrency in the Cocoa port
 * Fixed a bug where an emulator window in the Cocoa port will freeze until resized
 * Fixed incorrect disassembler outputs
 * Fixed a potential crash when deleting a breakpoint
 
### Misc Internal Changes
 * Large code refactoring, getting ready to stabilize API

## Version 0.4
This version is not compatible with save states of older versions.

### New/Improved Features
 * Cocoa port now includes several scaling algorithms, including a beta of the exclusive OmniScale algorithm
 * SDL port now includes sound support
 * SDL port now includes battery save support

### Accuracy Improvements/Fixes
 * APU emulation now includes stereo support
 * Improved PCM registers emulation
 * More accurate STAT interrupt emulation. This fixes Altered Space.
 
### Bug Fixes
 * The Cocoa port ignored some key presses if the user keyboard layout included non-ASCII characters
 * Fixed a bug that could cause a crash when adding or removing a breakpoint

## Version 0.3
### New/Improved Features
 * New debugger command: cartridge (alias: mbc) shows information about the cartridge and the current status of the MBC
 * Experimental debugger command: sld (Short for Stack Leak Detection) behaves similarly to finish, but stops if a stack leak or stack overflow is detected
 * The Cocoa port now allows configuring controls

### Accuracy Improvements/Fixes
 * ROM banks now wrap (Fixes game: Pocket Monster Adventures (Bootleg))
 * Fixed incorrect DMA behavior, DMA might fail for specific source addresses (Fixes game: The Smurfs 3)
 * Timer registers were counting too slow in CGB double speed mode
 * Corrected read behavior of the HDMA5 register (Fixes game: A Bug's Life in CGB mode)
 * Fixed a bug with the CGB boot ROM that prevented initialization of the wave RAM
 * The CGB boot ROM now loads the DMG tilemap for specific games, just like the original ROM (Fixes game: X (intro animation))
 * Fixed a bug where audio channel 3 was playing silently while it should have been muted.

### Bug Fixes
 * Debugger's finish command now behaves correctly when interrupts are involved
 * Corrected the description for the breakpoint command
 * SameBoy will not create save files for ROMs without cartridge RAM or RTC, even if they report having a battery, preventing 0-bytes save files

## Version 0.2
### New/Improved Features
 * Support for multiple breakpoints
 * Better debugger command style and error reporting; includes help command
 * In the SDL port, ^C in the terminal will break the debugger instead of quitting SameBoy
 * Cocoa port now saves preferences (Model and frame blending)
 * It is not possible to toggle console output and debugging in the Cocoa port

### Accuracy Improvements/Fixes
 * Read/write masks corrected for several registers
 * Corrected 144th OAM interrupt behavior
 * LCD operation moved to advance_cycles, fixing some obscure timing issues (Fixes game: X / Ekkusu)
 * More accurate emulation of the registers controlling DMG emulation on CGB
 * Emulation of PCM_12 and PCM_34 in the SDL port
 
### Bug Fixes
 * Debugger's next command can now exit a function

## Version 0.1
 * Initial public release