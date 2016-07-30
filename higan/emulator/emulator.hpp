#pragma once

#include <nall/nall.hpp>
#include <nall/vfs.hpp>
using namespace nall;

#include <libco/libco.h>
#include <audio/audio.hpp>
#include <video/video.hpp>
#include <resource/resource.hpp>

namespace Emulator {
  static const string Name    = "higan";
  static const string Version = "100.14";
  static const string Author  = "byuu";
  static const string License = "GPLv3";
  static const string Website = "http://byuu.org/";

  //incremented only when serialization format changes
  static const string SerializerVersion = "100";

  namespace Constants {
    namespace Time {
      static constexpr double Second      = 1.0;
      static constexpr double Millisecond = 1'000.0;
      static constexpr double Microsecond = 1'000'000.0;
      static constexpr double Nanosecond  = 1'000'000'000.0;
      static constexpr double Picosecond  = 1'000'000'000'000.0;
      static constexpr double Femtosecond = 1'000'000'000'000'000.0;
      static constexpr double Attosecond  = 1'000'000'000'000'000'000.0;
    }

    namespace Colorburst {
      static constexpr double NTSC = 315.0 / 88.0 * 1'000'000.0;
      static constexpr double PAL  = 283.75 * 15'625.0 + 25.0;
    }
  }
}

#include "interface.hpp"
#include "debugger.hpp"
