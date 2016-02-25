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

#include <sfc/sfc.hpp>
namespace SFC = SuperFamicom;

auto locate(string name) -> string;

#include "program/program.hpp"
#include "terminal/terminal.hpp"
#include "presentation/presentation.hpp"
