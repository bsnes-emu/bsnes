#include "../ethos.hpp"
Interface *interface = nullptr;

uint32_t Interface::videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue) {
  red >>= 8, green >>= 8, blue >>= 8;
  return red << 16 | green << 8 | blue << 0;
}

void Interface::videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
  uint32_t *output;
  unsigned outputPitch;

  if(video.lock(output, outputPitch, width, height)) {
    pitch >>= 2, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      memcpy(output + y * outputPitch, data + y * pitch, 4 * width);
    }

    video.unlock();
    video.refresh();
  }
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
  audio.sample(lsample, rsample);
}

int16_t Interface::inputPoll(unsigned port, unsigned device, unsigned id) {
  using nall::Keyboard;
  static int16_t table[Scancode::Limit];
  if(id == 0) input.poll(table);

  switch(id) {
  case 0: return table[keyboard(0)[Keyboard::X]];           //A
  case 1: return table[keyboard(0)[Keyboard::Z]];           //B
  case 2: return table[keyboard(0)[Keyboard::Apostrophe]];  //Select
  case 3: return table[keyboard(0)[Keyboard::Return]];      //Start
  case 4: return table[keyboard(0)[Keyboard::Right]];       //Right
  case 5: return table[keyboard(0)[Keyboard::Left]];        //Left
  case 6: return table[keyboard(0)[Keyboard::Up]];          //Up
  case 7: return table[keyboard(0)[Keyboard::Down]];        //Down
  case 8: return table[keyboard(0)[Keyboard::R]];           //R
  case 9: return table[keyboard(0)[Keyboard::L]];           //L
  }

  return 0;
}
