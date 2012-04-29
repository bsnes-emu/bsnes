#include <sfc/sfc.hpp>

namespace SuperFamicom {

Interface *interface = nullptr;

void Interface::message(const string &text) {
  print(text, "\n");
}

}
