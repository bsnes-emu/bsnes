#pragma once

#include <nall/platform.hpp>
#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/function.hpp>
#include <nall/hid.hpp>
#include <nall/image.hpp>
#include <nall/matrix.hpp>
#include <nall/range.hpp>
#include <nall/set.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
#include <nall/dsp/resampler/cubic.hpp>
#include <nall/hash/crc32.hpp>

using nall::function;
using nall::shared_pointer;
using nall::string;
using nall::vector;

namespace ruby {

#include "video.hpp"
#include "audio.hpp"
#include "input.hpp"

}
