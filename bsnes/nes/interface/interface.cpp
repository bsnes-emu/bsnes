#include <nes/nes.hpp>

namespace NES {

Interface *interface = nullptr;

void Interface::videoRefresh(const uint16_t *data) {
}

void Interface::audioSample(const int16_t sample) {
}

int16_t Interface::inputPoll(bool port, unsigned device, unsigned id) {
  return 0;
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
