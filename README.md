# SameBoy

SameBoy is an open source Gameboy (DMG) and Gameboy Color (CGB) emulator, written in portable C. It has a native Cocoa frontend for OS X, and an incomplete experimental SDL frontend for other operating systems. It also includes a text-based debugger with an expression evaluator.

## Features
Features common to both Cocoa and SDL versions:
 * Supports Gameboy (DMG) and Gameboy Color (CGB) emulation
 * Lets you choose the model you want to emulate regardless of ROM
 * Includes open source DMG and CGB boot ROMs:
   * Complete support for (and documentation of) *all* game-specific palettes in the CGB boot ROM, for accurate emulation of Gameboy games on a Gameboy Color
   * Supports manual palette selection with key combinations, with 4 additional new palettes (A + B + direction)
   * Supports palette selection in a CGB game, forcing it to run in 'paletted' DMG mode, if ROM allows doing so.
   * Support for games with a non-Nintendo logo in the header
   * No long animation in the DMG boot
 * Has a text-based debugger with an expression evaluator
 * Emulates [PCM_12 and PCM_34 registers](https://github.com/LIJI32/GBVisualizer)
 * Emulates LCD timing effects, supporting the Demotronic trick, [GBVideoPlayer](https://github.com/LIJI32/GBVideoPlayer) and other tech demos
 * Accurate instruction and memory timings
 * Real time clock emulation

Features currently supported only with the Cocoa version:
 * Native Cocoa interface, with support for all system-wide features, such as drag-and-drop and smart titlebars
 * Retina display support, allowing a wider range of scaling factors without artifacts
 * High quality 96KHz audio
 * Battery save support
 * Save states
 * Optional frame blending

## Compatibility
While SameBoy passes many of [blargg's test ROMs](http://gbdev.gg8.se/wiki/articles/Test_ROMs#Blargg.27s_tests), some games fail to run correctly. SameBoy is still relatively early in its development and accuracy and compatibility will be improved.

## Compilation
SameBoy requires the following tools and libraries to build:
 * clang
 * make
 * Cocoa port: OS X SDK and Xcode command line tools
 * SDL port: SDL.framework (OS X) or libsdl (Other platforms)
 * [rgbds](https://github.com/bentley/rgbds/releases/), for boot ROM compilation

SameBoy was compiled and tested on OS X and Ubuntu.