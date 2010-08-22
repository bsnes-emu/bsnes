Interface interface;

void Interface::video_refresh(const uint16_t *data, unsigned width, unsigned height) {
  bool interlace = (height >= 240);
  bool overscan = (height == 239 || height == 478);
  unsigned pitch = interlace ? 1024 : 2048;

  //TV resolution and overscan simulation
  if(config().video.context->region == 0) {
    //NTSC
    height = 224;
    if(interlace) height <<= 1;
    if(overscan) data += 7 * 1024;
  } else {
    //PAL
    height = 239;
    if(interlace) height <<= 1;
    if(!overscan) data -= 7 * 1024;
  }

  //scale display.crop* values from percentage-based (0-100%) to exact pixel sizes (width, height)
  unsigned cropLeft = (double)display.cropLeft / 100.0 * width;
  unsigned cropTop = (double)display.cropTop / 100.0 * height;
  unsigned cropRight = (double)display.cropRight / 100.0 * width;
  unsigned cropBottom = (double)display.cropBottom / 100.0 * height;

  width -= (cropLeft + cropRight);
  height -= (cropTop + cropBottom);

  uint32_t *output;
  unsigned outwidth, outheight, outpitch;
  filter.size(outwidth, outheight, width, height);

  if(video.lock(output, outpitch, outwidth, outheight) == true) {
    data += cropTop * (pitch >> 1) + cropLeft;
    filter.render(output, outpitch, data, pitch, width, height);
    video.unlock();
    video.refresh();
    if(saveScreenshot == true) captureScreenshot(output, outpitch, outwidth, outheight);
  }

  state.frame();
  #if defined(DEBUGGER)
  debugger->frameTick();
  #endif

  //frame counter
  static signed frameCount = 0;
  static time_t prev, curr;
  frameCount++;

  time(&curr);
  if(curr != prev) {
    framesUpdated = true;
    framesExecuted = frameCount;
    frameCount = 0;
    prev = curr;
  }
}

void Interface::audio_sample(uint16_t left, uint16_t right) {
  if(config().audio.mute) left = right = 0;
  audio.sample(left, right);
}

void Interface::input_poll() {
  mapper().cache();
}

int16_t Interface::input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  return mapper().status(port, device, index, id);
}

void Interface::captureScreenshot(uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
  saveScreenshot = false;
  QImage image((const unsigned char*)data, width, height, pitch, QImage::Format_RGB32);

  string filename = nall::basename(cartridge.fileName);
  time_t systemTime = time(0);
  tm *currentTime = localtime(&systemTime);
  char t[512];
  sprintf(t, "%.4u%.2u%.2u-%.2u%.2u%.2u",
    1900 + currentTime->tm_year, 1 + currentTime->tm_mon, currentTime->tm_mday,
    currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec
  );
  filename << "-" << t << ".png";

  image.save(filepath(filename, config().path.data));
  utility.showMessage("Screenshot saved.");
}

Interface::Interface() {
  saveScreenshot = false;
}
