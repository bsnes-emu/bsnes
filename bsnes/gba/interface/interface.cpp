#include <gba/gba.hpp>

namespace GameBoyAdvance {

Interface *interface = nullptr;

uint32_t Interface::videoColor(uint15_t source, uint16_t red, uint16_t green, uint16_t blue) {
  red >>= 8, green >>= 8, blue >>= 8;
  return red << 16 | green << 8 | blue << 0;
}

void Interface::videoRefresh(const uint32_t *data) {
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
