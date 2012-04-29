#include <sfc/sfc.hpp>

namespace SuperFamicom {

Interface *interface = nullptr;

uint32_t Interface::videoColor(uint19_t source, uint16_t red, uint16_t green, uint16_t blue) {
  red >>= 8, green >>= 8, blue >>= 8;
  return red << 16 | green << 8 | blue << 0;
}

void Interface::videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
}

void Interface::audioSample(int16_t l_sample, int16_t r_sample) {
}

int16_t Interface::inputPoll(bool port, Input::Device device, unsigned index, unsigned id) {
  return 0;
}

void Interface::message(const string &text) {
  print(text, "\n");
}

}
