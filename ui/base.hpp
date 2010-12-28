#include <nall/file.hpp>
#include <nall/foreach.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include <gameboy.hpp>

#include "interface.hpp"

#include "general/main-window.hpp"

#include "utility/utility.hpp"

struct Application {
  bool quit;

  void main(int argc, char **argv);
};

extern Application application;
