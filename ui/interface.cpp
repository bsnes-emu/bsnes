Interface interface;

void Interface::video_refresh(const uint8_t *data) {
}

void Interface::audio_sample(signed left, signed right) {
}

void Interface::input_poll() {
}

bool Interface::input_poll(unsigned id) {
  return false;
}

void Interface::message(const string &text) {
  MessageWindow::information(mainWindow, text);
}
