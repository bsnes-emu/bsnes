#if !defined(PROFILE_ACCURACY)
  #error "debugger is only compatible with accuracy profile"
#endif

#include <snes/snes.hpp>

#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dsp.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/snes/cartridge.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <ruby/ruby.hpp>
using namespace ruby;

#include "interface/interface.hpp"
#include "debugger/debugger.hpp"
#include "tracer/tracer.hpp"
#include "console/console.hpp"
#include "video/video.hpp"
#include "cpu/cpu.hpp"
#include "smp/smp.hpp"
#include "memory/memory.hpp"
#include "breakpoint/breakpoint.hpp"
extern uint8_t laevateinnLogo[121905];

struct Application {
  bool quit;

  string basepath;
  string userpath;

  string proportionalFont;
  string proportionalFontBold;
  string monospaceFont;

  Application(int argc, char **argv);
  ~Application();
};

extern Application *application;
