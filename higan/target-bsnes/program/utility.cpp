auto Program::showMessage(string text) -> void {
  statusTime = chrono::timestamp();
  statusMessage = text;
}

auto Program::showFrameRate(string text) -> void {
  statusFrameRate = text;
}

auto Program::updateStatus() -> void {
  string message;
  if(chrono::timestamp() - statusTime <= 2) {
    message = statusMessage;
  }
  if(message != presentation.statusLeft.text()) {
    presentation.statusLeft.setText(message);
  }

  string frameRate;
  if(!emulator->loaded()) {
    frameRate = tr("Unloaded");
  } else if(presentation.pauseEmulation.checked()) {
    frameRate = tr("Paused");
  } else if(!focused() && emulatorSettings.pauseEmulation.checked()) {
    frameRate = tr("Paused");
  } else {
    frameRate = statusFrameRate;
  }
  if(frameRate != presentation.statusRight.text()) {
    presentation.statusRight.setText(frameRate);
  }
}

auto Program::captureScreenshot() -> bool {
  if(emulator->loaded() && screenshot.data) {
    if(auto filename = screenshotPath()) {
      image capture;
      capture.allocate(screenshot.data, screenshot.pitch, screenshot.width, screenshot.height);

      //normalize pixel aspect ratio to 1:1
      if(capture.width() == 512 && capture.height() == 240) capture.scale(512, 480, false);  //hires
      if(capture.width() == 256 && capture.height() == 480) capture.scale(512, 480, false);  //interlace

      auto data = capture.data();
      auto pitch = capture.pitch();
      auto width = capture.width();
      auto height = capture.height();

      if(presentation.overscanCropping.checked()) {
        if(height == 240) data +=  8 * pitch, height -= 16;
        if(height == 480) data += 16 * pitch, height -= 32;
      }

      if(Encode::BMP::create(filename, data, width << 2, width, height, /* alpha = */ false)) {
        showMessage({"Captured screenshot [", Location::file(filename), "]"});
        return true;
      }
    }
  }
  return false;
}

auto Program::paused() -> bool {
  if(!emulator->loaded()) return true;
  if(presentation.pauseEmulation.checked()) return true;
  if(!focused() && emulatorSettings.pauseEmulation.checked()) return true;
  return false;
}

auto Program::focused() -> bool {
  //exclusive mode creates its own top-level window: presentation window will not have focus
  if(video && video.exclusive()) return true;
  if(presentation.focused()) return true;
  return false;
}
