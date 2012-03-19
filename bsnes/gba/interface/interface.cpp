#include <gba/gba.hpp>

namespace GBA {

Interface *interface = nullptr;

void Interface::videoRefresh(const uint16_t *data) {
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
}

bool Interface::inputPoll(unsigned id) {
  return false;
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
