bsnes source code
=================

A guide to what all these directories are for:

  - [Database](./Database/)
    contains the databases bsnes uses
    to figure out what circuit board a game expects,
    and also the database of pre-made game cheats
  - [emulator](./emulator/)
    contains the interface
    that the emulation core in [sfc](./sfc/) implements
      - It comes from higan v106,
        which has many emulation cores
        that all implement the same interface —
        bsnes only has one,
        but the interface is still a good abstraction,
        so it's still here.
  - [filter](./filter/)
    contains classic CPU-based image upscaling filters,
    like HQ2x and Super Eagle
  - [gb](./gb/)
    contains the SameBoy emulator code
    used for Super Game Boy emulation
  - [heuristics](./heuristics/)
    contains the logic that guesses
    which memory map a particular game expects
    and what extra hardware it assumes is present,
    when a game cannot be found in the database
  - [Locale](./Locale/)
    contains translation databases
    that bsnes can use to display its user interface
    in a different language
  - [lzma](./lzma/)
    contains the 7-Zip SDK
    allowing bsnes to understad `.7z` archives
  - [processor](./processor/)
    contains all the CPU emulation cores
    used by the hardware bsnes emulates
      - Another holdover from higan v106,
        where different supported systems
        happen to use the same model CPU,
        so the CPU emulation cores
        are not specific to a system
  - [sfc](./sfc/)
    contains Super Famicom (SNES) emulation code
  - [target-bsnes](./target-bsnes/)
    contains the normal bsnes user interface
  - [target-libretro](./target-libretro/)
    implements the "libretro" API
    on top of bsnes' native
    [emulator](./emulator/) API,
    so bsnes can be used with libretro-based frontends
