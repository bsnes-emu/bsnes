#include <nes/nes.hpp>

namespace Famicom {

Interface *interface = nullptr;

uint32_t Interface::videoColor(uint9_t source, uint16_t red, uint16_t green, uint16_t blue) {
  red >>= 8, green >>= 8, blue >>= 8;
  return red << 16 | green << 8 | blue << 0;
}

void Interface::videoRefresh(const uint32_t *data) {
}

void Interface::audioSample(const int16_t sample) {
}

int16_t Interface::inputPoll(bool port, unsigned device, unsigned id) {
  return 0;
}

void Interface::message(const string &text) {
  print(text, "\n");
}

void Interface::loadCartridge(const string &markup, const stream &memory) {
  cartridge.load(markup, memory.data(), memory.size());
}

}
