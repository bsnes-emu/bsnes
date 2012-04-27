bool InterfaceCore::loadFirmware(string filename, string keyname, uint8_t *targetdata, unsigned targetsize) {
  bool result = false;

  filename = application->path(filename);
  string markup;
  markup.readfile(filename);
  XML::Document document(markup);
  lstring keypart = keyname.split<1>(".");

  if(document[keypart[0]][keypart[1]].exists()) {
    auto &key = document[keypart[0]][keypart[1]];
    string firmware = key["firmware"].data;
    string hash = key["sha256"].data;

    if(auto memory = file::read({dir(filename),firmware})) {
      if(nall::sha256(memory.data(), memory.size()) == hash) {
        memcpy(targetdata, memory.data(), min(targetsize, memory.size()));
        result = true;
      } else {
        MessageWindow::information(Window::None, {"Warning: Firmware SHA256 sum is incorrect:\n\n", dir(filename), firmware});
      }
    }
  }

  return result;
}

/* 5-bit -> 8-bit
static const uint8_t gammaRamp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};*/

uint32_t InterfaceCore::color(uint16_t r, uint16_t g, uint16_t b) {
  auto gamma = [](uint16_t n) {
    double exponent = 1.0 + (double)config->video.gamma * 0.01;
    return n < 32768 ? 32767 * pow(((double)n / 32767), exponent) : n;
  };

  auto contrast = [](uint16_t n) {
    double contrast = config->video.contrast * 0.01;
    signed result = n * contrast;
    return max(0, min(65535, result));
  };

  auto brightness = [](uint16_t n) {
    signed brightness = (config->video.brightness - 100) * 256;
    signed result = n + brightness;
    return max(0, min(65535, result));
  };

  r = brightness(contrast(gamma(r)));
  g = brightness(contrast(gamma(g)));
  b = brightness(contrast(gamma(b)));

  if(application->depth == 30) { r >>= 6, g >>= 6, b >>= 6; return r << 20 | g << 10 | b <<  0; }
  if(application->depth == 24) { r >>= 8, g >>= 8, b >>= 8; return r << 16 | g <<  8 | b <<  0; }
  return 0u;
}
