#include <nall/nall.hpp>
#include <ruby/ruby.hpp>
#include <hiro/hiro.hpp>
using namespace nall;
using namespace ruby;
using namespace hiro;
extern Video video;
extern Audio audio;
extern Input input;

#include <emulator/emulator.hpp>
extern unique_pointer<Emulator::Interface> emulator;

#include <nall/encode/rle.hpp>
#include <nall/decode/rle.hpp>

#include "program/program.hpp"
#include "input/input.hpp"
#include "presentation/presentation.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"
#include "resource/resource.hpp"

auto locate(string name) -> string;
