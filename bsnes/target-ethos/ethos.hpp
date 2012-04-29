#include <emulator/emulator.hpp>
#include <gba/interface/interface.hpp>

#include <nall/platform.hpp>
#include <nall/config.hpp>
#include <nall/stream/file.hpp>
#include <nall/stream/memory.hpp>
#include <nall/stream/vector.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <ruby/ruby.hpp>
using namespace ruby;

#include "interface/interface.hpp"
#include "general/general.hpp"

struct Application {
  vector<Emulator::Interface*> emulators;
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

  string path(const string &filename);
  void run();
  Application(int argc, char **argv);
  ~Application();
};

extern Application *application;
