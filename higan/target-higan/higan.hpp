#include <nall/nall.hpp>
using namespace nall;

#include <ruby/ruby.hpp>
using namespace ruby;
extern unique_pointer<Video> video;
extern unique_pointer<Audio> audio;
extern unique_pointer<Input> input;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>
extern Emulator::Interface* emulator;

#include "program/program.hpp"
#include "input/input.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"
#include "presentation/presentation.hpp"
#include "resource/resource.hpp"

auto locate(string name) -> string;
