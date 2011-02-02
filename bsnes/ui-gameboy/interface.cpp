Interface interface;

void Interface::video_refresh(const uint8_t *data) {
  uint32_t *buffer;
  unsigned pitch;
  if(video.lock(buffer, pitch, 160, 144)) {
    for(unsigned y = 0; y < 144; y++) {
      uint32_t *line = buffer + y * (pitch >> 2);
      const uint8_t *source = data + y * 160;
      for(unsigned x = 0; x < 160; x++) {
        uint32_t color = *source++;
        *line++ = (color << 16) | (color << 8) | (color << 0);
      }
    }
    video.unlock();
    video.refresh();
  }

  static unsigned frameCounter = 0;
  static time_t timeCounter = time(0);

  frameCounter++;
  time_t currentTime = time(0);
  if(currentTime != timeCounter) {
    timeCounter = currentTime;
    mainWindow.setStatusText({ "FPS: ", frameCounter });
    frameCounter = 0;
  }
}

void Interface::audio_sample(signed left, signed right) {
  audio.sample(left, right);
}

void Interface::input_poll() {
  input.poll(inputState);
}

bool Interface::input_poll(unsigned id) {
  switch(id) {
    case GameBoy::Input::Up: return inputState[keyboard(0)[Keyboard::Up]];
    case GameBoy::Input::Down: return inputState[keyboard(0)[Keyboard::Down]];
    case GameBoy::Input::Left: return inputState[keyboard(0)[Keyboard::Left]];
    case GameBoy::Input::Right: return inputState[keyboard(0)[Keyboard::Right]];
    case GameBoy::Input::B: return inputState[keyboard(0)[Keyboard::Z]];
    case GameBoy::Input::A: return inputState[keyboard(0)[Keyboard::X]];
    case GameBoy::Input::Select: return inputState[keyboard(0)[Keyboard::Apostrophe]];
    case GameBoy::Input::Start: return inputState[keyboard(0)[Keyboard::Return]];
  }

  return false;
}

void Interface::message(const string &text) {
  MessageWindow::information(mainWindow, text);
}
