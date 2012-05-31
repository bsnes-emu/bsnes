#include <emulator/emulator.hpp>

#include <nall/platform.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dsp.hpp>
#include <nall/map.hpp>
#include <nall/stream/file.hpp>
#include <nall/stream/memory.hpp>
#include <nall/stream/mmap.hpp>
#include <nall/stream/vector.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <ruby/ruby.hpp>
using namespace ruby;

#include "configuration/configuration.hpp"
#include "interface/interface.hpp"
#include "utility/utility.hpp"
#include "input/input.hpp"
#include "window/window.hpp"
#include "general/general.hpp"
#include "settings/settings.hpp"
#include "tools/tools.hpp"

Emulator::Interface& system();

struct Application {
  vector<Emulator::Interface*> emulator;
  Emulator::Interface *active;

  bool quit;
  bool pause;
  bool autopause;
  unsigned depth;

  string basepath;
  string userpath;

  string normalFont;
  string boldFont;
  string titleFont;
  string monospaceFont;

  bool focused();
  string path(const string &filename);
  void commandLineLoad(string pathname);
  void run();
  void bootstrap();
  Application(int argc, char **argv);
  ~Application();
};

extern Application *application;
extern DSP dspaudio;
