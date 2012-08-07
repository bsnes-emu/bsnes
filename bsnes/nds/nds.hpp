#ifndef NDS_HPP
#define NDS_HPP

// dasShiny - Nintendo DS emulator
//   Copyright (c) 2012 Cydrak
//   License: GPLv3

#include <emulator/emulator.hpp>
#include <libco/libco.h>

namespace NintendoDS {
  enum : unsigned {
    Byte = 8, Half = 16, Word = 32
  };
  
  #include <nds/interface/interface.hpp>
  #include <nds/memory/memory.hpp>
  #include <nds/system/system.hpp>
  #include <nds/cpu/cpu.hpp>
  #include <nds/apu/apu.hpp>
  #include <nds/ppu/ppu.hpp>
  #include <nds/gpu/gpu.hpp>
  #include <nds/video/video.hpp>
  #include <nds/slot1/slot1.hpp>
  #include <nds/slot2/slot2.hpp>
  #include <nds/wifi/wifi.hpp>
}

#endif
