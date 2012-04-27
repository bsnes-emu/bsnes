#include <gb/gb.hpp>

namespace GameBoy {

Interface *interface = nullptr;

void Interface::lcdScanline() {
}

void Interface::joypWrite(bool p15, bool p14) {
}

uint32_t Interface::videoColor(uint15_t source, uint16_t red, uint16_t green, uint16_t blue) {
  red >>= 8, green >>= 8, blue >>= 8;
  return red << 16 | green << 8 | blue << 0;
}

void Interface::videoRefresh(const uint32_t *data) {
}

void Interface::audioSample(int16_t center, int16_t left, int16_t right) {
}

bool Interface::inputPoll(unsigned id) {
  return false;
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
