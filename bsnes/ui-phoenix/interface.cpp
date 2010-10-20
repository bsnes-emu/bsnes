Palette palette;
Interface interface;

const uint8_t Palette::gammaRamp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};

void Palette::update() {
  for(unsigned i = 0; i < 32768; i++) {
    unsigned r = gammaRamp[(i >> 10) & 31];
    unsigned g = gammaRamp[(i >>  5) & 31];
    unsigned b = gammaRamp[(i >>  0) & 31];
  //r = (r << 3) | (r >> 2);
  //g = (g << 3) | (g >> 2);
  //b = (b << 3) | (b >> 2);
    color[i] = (r << 16) | (g << 8) | (b << 0);
  }
}

Palette::Palette() {
  update();
}

void Interface::video_refresh(const uint16_t *data, unsigned width, unsigned height) {
  bool interlace = (height >= 240);
  bool overscan = (height == 239 || height == 478);
  unsigned inpitch = interlace ? 1024 : 2048;

  uint32_t *buffer;
  unsigned outpitch;

  if(video.lock(buffer, outpitch, width, height)) {
    for(unsigned y = 0; y < height; y++) {
      uint32_t *output = buffer + y * (outpitch >> 2);
      const uint16_t *input = data + y * (inpitch >> 1);
      for(unsigned x = 0; x < width; x++) *output++ = palette.color[*input++];
    }
    video.unlock();
    video.refresh();
  }

  static signed frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    mainWindow.setStatusText(string("FPS: ", frameCounter));
    frameCounter = 0;
    previous = current;
  }
}

void Interface::audio_sample(uint16_t left, uint16_t right) {
  audio.sample(left, right);
}

void Interface::input_poll() {
  input.poll(state);
}

int16_t Interface::input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(port == 0) {
    if(device == SNES::Input::Device::Joypad) {
      switch(id) {
        case SNES::Input::JoypadID::Up:     return state[keyboard(0)[Keyboard::Up]];
        case SNES::Input::JoypadID::Down:   return state[keyboard(0)[Keyboard::Down]];
        case SNES::Input::JoypadID::Left:   return state[keyboard(0)[Keyboard::Left]];
        case SNES::Input::JoypadID::Right:  return state[keyboard(0)[Keyboard::Right]];
        case SNES::Input::JoypadID::B:      return state[keyboard(0)[Keyboard::Z]];
        case SNES::Input::JoypadID::A:      return state[keyboard(0)[Keyboard::X]];
        case SNES::Input::JoypadID::Y:      return state[keyboard(0)[Keyboard::A]];
        case SNES::Input::JoypadID::X:      return state[keyboard(0)[Keyboard::S]];
        case SNES::Input::JoypadID::L:      return state[keyboard(0)[Keyboard::D]];
        case SNES::Input::JoypadID::R:      return state[keyboard(0)[Keyboard::C]];
        case SNES::Input::JoypadID::Select: return state[keyboard(0)[Keyboard::Apostrophe]];
        case SNES::Input::JoypadID::Start:  return state[keyboard(0)[Keyboard::Return]];
      }
    }
  }
  return 0;
}
