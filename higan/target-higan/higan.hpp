#include <nall/nall.hpp>
#include <ruby/ruby.hpp>
#include <hiro/hiro.hpp>
using namespace nall;
using namespace ruby;
using namespace hiro;
extern unique_pointer<Video> video;
extern unique_pointer<Audio> audio;
extern unique_pointer<Input> input;

#include <emulator/emulator.hpp>
extern Emulator::Interface* emulator;

#include "program/program.hpp"
#include "configuration/configuration.hpp"
#include "input/input.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"
#include "presentation/presentation.hpp"

auto locate(string name) -> string;
