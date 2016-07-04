#pragma once

#include <nall/nall.hpp>
#include <nall/vfs.hpp>
using namespace nall;

#include <libco/libco.h>
#include <audio/audio.hpp>
#include <video/video.hpp>
#include <resource/resource.hpp>

namespace Emulator {
  static const string Name = "higan";
  static const string Version = "099.16";
  static const string Author = "byuu";
  static const string License = "GPLv3";
  static const string Website = "http://byuu.org/";

  //incremented only when serialization format changes
  static const string SerializerVersion = "099.14";
}

#include "interface.hpp"

#if defined(DEBUGGER)
  #define privileged public
#else
  #define privileged private
#endif
