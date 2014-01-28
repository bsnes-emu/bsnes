#include <emulator/emulator.hpp>

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

#include <phoenix/phoenix.hpp>
using namespace phoenix;

struct Program {
  string basepath;
  string userpath;
  string sharedpath;

  string path(string name);
  void main();
  Program(int argc, char** argv);
};

extern Program* program;
extern DSP dspaudio;
