#pragma once

#include <nall/nall.hpp>
#include <nall/vfs.hpp>
using namespace nall;

#include "types.hpp"
#include <libco/libco.h>
#include <audio/audio.hpp>
#include <video/video.hpp>
#include <resource/resource.hpp>

namespace Emulator {
  static const string Name    = "higan";
  static const string Version = "106.13";
  static const string Author  = "byuu";
  static const string License = "GPLv3";
  static const string Website = "https://byuu.org/";

  //incremented only when serialization format changes
  static const string SerializerVersion = "104";

  namespace Constants {
    namespace Colorburst {
      static constexpr double NTSC = 315.0 / 88.0 * 1'000'000.0;
      static constexpr double PAL  = 283.75 * 15'625.0 + 25.0;
    }
  }

  //nall/vfs shorthand constants for open(), load()
  namespace File {
    static const auto Read = vfs::file::mode::read;
    static const auto Write = vfs::file::mode::write;
    static const auto Optional = false;
    static const auto Required = true;
  };
}

#include "platform.hpp"
#include "interface.hpp"
#include "game.hpp"
