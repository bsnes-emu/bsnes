#include <nall/nall.hpp>
#include <ruby/ruby.hpp>
#include <hiro/hiro.hpp>
using namespace nall;
using namespace ruby;
using namespace hiro;
extern Video* video;
extern Audio* audio;
extern Input* input;

#include <emulator/emulator.hpp>
extern Emulator::Interface* emulator;

#include "program/program.hpp"
#include "configuration/configuration.hpp"
#include "input/input.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"
#include "presentation/presentation.hpp"
