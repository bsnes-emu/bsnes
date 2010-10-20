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
#include "settings/settings.hpp"
#include "utility/utility.hpp"
#include "cartridge/cartridge.hpp"

struct Application {
  Font font;
  bool quit;
  void main(int argc, char **argv);
};

extern Application application;

struct Style {
  enum : unsigned {
  #if defined(PHOENIX_WINDOWS)
    SliderHeight = 25,
  #elif defined(PHOENIX_GTK)
    SliderHeight = 22,
  #elif defined(PHOENIX_QT)
    SliderHeight = 22,
  #endif
  };
};
