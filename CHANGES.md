# Change Log

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