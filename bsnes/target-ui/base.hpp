#include <nes/nes.hpp>
#include <snes/snes.hpp>
#include <gameboy/gameboy.hpp>
#include <gba/gba.hpp>

#include <nall/compositor.hpp>
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

#include "config/config.hpp"
#include "interface/interface.hpp"
#include "input/input.hpp"
#include "utility/utility.hpp"
#include "window/window.hpp"
#include "general/general.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"

struct Application {
  bool quit;
  bool pause;
  bool autopause;
  bool compositionEnable;
  unsigned depth;

  string basepath;
  string userpath;
  string path(const string &filename);

  string title;
  string normalFont;
  string boldFont;
  string titleFont;

  void run();
  Application(int argc, char **argv);
  ~Application();
};

extern Application *application;
extern nall::DSP dspaudio;
