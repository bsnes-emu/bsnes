#include <emulator/emulator.hpp>
#include <sfc/interface/interface.hpp>

#include <nall/platform.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dsp.hpp>
#include <nall/invoke.hpp>
#include <nall/map.hpp>
#include <nall/stream/file.hpp>
#include <nall/stream/memory.hpp>
#include <nall/stream/mmap.hpp>
#include <nall/stream/vector.hpp>
using namespace nall;

#include <ruby/ruby.hpp>
using namespace ruby;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include "interface/interface.hpp"
#include "presentation/presentation.hpp"
#include "terminal/terminal.hpp"
#include "resource/resource.hpp"

struct Program {
  string basepath;
  string userpath;
  string sharedpath;

  bool pause = true;

  string path(string name);
  void main();
  Program(string pathname);
};

extern Program* program;
extern DSP dspaudio;
