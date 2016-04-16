# Change Log

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
 * Sameboy will not create save files for ROMs without cartridge RAM or RTC, even if they report having a battery, preventing 0-bytes save files

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