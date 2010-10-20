#include <snes.hpp>

#include <nall/base64.hpp>
#include <nall/config.hpp>
#include <nall/input.hpp>
#include <nall/ups.hpp>
#include <nall/snes/info.hpp>
using namespace nall;

#include <ruby/ruby.hpp>
using namespace ruby;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include "interface.hpp"
#include "general/general.hpp"
#include "cartridge/cartridge.hpp"

struct Application {
  bool quit;
  void main(int argc, char **argv);
};

extern Application application;
