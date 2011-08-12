Palette palette;
Filter filter;
Interface interface;

const uint8_t Palette::gammaRamp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};

uint8_t Palette::contrastAdjust(uint8_t input) {
  signed contrast = config.video.contrast - 100;
  signed result = input - contrast + (2 * contrast * input + 127) / 255;
  return max(0, min(255, result));
}

uint8_t Palette::brightnessAdjust(uint8_t input) {
  signed brightness = config.video.brightness - 100;
  signed result = input + brightness;
  return max(0, min(255, result));
}

uint8_t Palette::gammaAdjust(uint8_t input) {
  signed result = (signed)(pow(((double)input / 255.0), (double)config.video.gamma / 100.0) * 255.0 + 0.5);
  return max(0, min(255, result));
}

void Palette::update() {
  for(unsigned i = 0; i < 32768; i++) {
    unsigned r = (i >> 10) & 31;
    unsigned g = (i >>  5) & 31;
    unsigned b = (i >>  0) & 31;

    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);

    if(config.video.useGammaRamp) {
      r = gammaRamp[r >> 3];
      g = gammaRamp[g >> 3];
      b = gammaRamp[b >> 3];
    }

    if(config.video.contrast != 100) {
      r = contrastAdjust(r);
      g = contrastAdjust(g);
      b = contrastAdjust(b);
    }

    if(config.video.brightness != 100) {
      r = brightnessAdjust(r);
      g = brightnessAdjust(g);
      b = brightnessAdjust(b);
    }

    if(config.video.gamma != 100) {
      r = gammaAdjust(r);
      g = gammaAdjust(g);
      b = gammaAdjust(b);
    }

    color[i] = (r << 16) | (g << 8) | (b << 0);
  }
}

void Filter::size(unsigned &width, unsigned &height) {
  if(opened() && dl_size) return dl_size(width, height);
}

void Filter::render(uint32_t *output, unsigned outpitch, const uint16_t *input, unsigned pitch, unsigned width, unsigned height) {
  if(opened() && dl_render) return dl_render(palette.color, output, outpitch, input, pitch, width, height);

  for(unsigned y = 0; y < height; y++) {
    uint32_t *outputLine = output + y * (outpitch >> 2);
    const uint16_t *inputLine = input + y * (pitch >> 1);
    for(unsigned x = 0; x < width; x++) *outputLine++ = palette.color[*inputLine++];
  }
}

//data is a 512x512x16bpp buffer, broken in two-scanline pairs (for interlace):
//  0 -   7 = front porch
//        8 = empty scanline 0
//  9 - 232 = standard scanlines 1 - 224
//233 - 247 = overscan scanlines 225-239
//248 - 255 = back porch

void Interface::video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {
  unsigned width = hires ? 512 : 256;
  unsigned height = config.video.region == 0 ? 224 : 239;
  if(interlace) height <<= 1;
  unsigned inpitch = interlace ? 1024 : 2048;

  if(config.video.region == 0) {
    if(overscan == false) data +=  9 * 1024;  // 0 + 224 +  0
    if(overscan == true ) data += 16 * 1024;  //-7 + 224 + -7
  }

  if(config.video.region == 1) {
    if(overscan == false) data +=  1 * 1024;  // 8 + 224 +  7
    if(overscan == true ) data +=  9 * 1024;  // 0 + 239 +  0
  }

  unsigned outwidth = width, outheight = height;
  filter.size(outwidth, outheight);

  uint32_t *buffer;
  unsigned outpitch;
  if(video.lock(buffer, outpitch, outwidth, outheight)) {
    filter.render(buffer, outpitch, data, inpitch, width, height);
    video.unlock();
    video.refresh();
  }

  static unsigned frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    utility.setStatus({ "FPS: ", frameCounter });
    frameCounter = 0;
    previous = current;
  }

  if(captureScreenshot) {
    captureScreenshot = false;
    time_t currentTime = time(0);
    tm *info = localtime(&currentTime);
    string filename = { "-",
      decimal<4, '0'>(info->tm_year + 1900), "-", decimal<2, '0'>(info->tm_mon + 1), "-", decimal<2, '0'>(info->tm_mday), " ",
      decimal<2, '0'>(info->tm_hour), ":", decimal<2, '0'>(info->tm_min), ":", decimal<2, '0'>(info->tm_sec), ".bmp"
    };
    bmp::write(path(utility.slotPath(), filename), buffer, outwidth, outheight, outpitch, false);
    utility.showMessage("Screenshot captured");
  }
}

void Interface::audio_sample(uint16_t left, uint16_t right) {
  if(config.audio.mute) left = right = 0;
  audio.sample(left, right);
}

int16_t Interface::input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(config.settings.focusPolicy == 1 && mainWindow.focused() == false) return 0;
  return inputMapper.poll(port, device, index, id);
}

void Interface::message(const string &text) {
  MessageWindow::information(mainWindow, text);
}

string Interface::path(SNES::Cartridge::Slot slot, const string &hint) {
  return ::path.load(slot, hint);
}

Interface::Interface() {
  captureScreenshot = false;
}
