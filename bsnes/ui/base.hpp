#include <nes/nes.hpp>
#include <snes/snes.hpp>
#include <gameboy/gameboy.hpp>

#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dsp.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/input.hpp>
#include <nall/bps/patch.hpp>
#include <nall/snes/cartridge.hpp>
#include <nall/gameboy/cartridge.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <ruby/ruby.hpp>
using namespace ruby;

#include "interface/interface.hpp"
#include "utility/utility.hpp"
#include "general/general.hpp"

struct Application {
  bool quit;

  string title;
  string normalFont;
  string boldFont;

  void run();
  Application(int argc, char **argv);
  ~Application();
};

extern Application *application;
extern nall::DSP dspaudio;
