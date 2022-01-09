#include <ruby/ruby.hpp>
using namespace ruby;
extern Video video;
extern Audio audio;
extern Input input;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>
extern unique_pointer<Emulator::Interface> emulator;

#include <filter/filter.hpp>
#include <lzma/lzma.hpp>

#include <nall/instance.hpp>
#include <nall/decode/rle.hpp>
#include <nall/decode/zip.hpp>
#include <nall/encode/rle.hpp>
#include <nall/encode/zip.hpp>
#include <nall/hash/crc16.hpp>
#include <nall/thread.hpp>
#include <nall/atomic-queue.hpp>
#include <nall/http/server.hpp>

#include "program/program.hpp"
#include "input/input.hpp"
#include "presentation/presentation.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"
#include "resource/resource.hpp"

auto locate(string name) -> string;
