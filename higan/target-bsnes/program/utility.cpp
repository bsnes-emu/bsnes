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
  if(message != presentation->statusLeft.text()) {
    presentation->statusLeft.setText(message);
  }

  string frameRate;
  if(!emulator->loaded()) {
    frameRate = tr("Unloaded");
  } else if(presentation->pauseEmulation.checked()) {
    frameRate = tr("Paused");
  } else if(!focused() && settingsWindow->configuration.pauseEmulation.checked()) {
    frameRate = tr("Paused");
  } else {
    frameRate = statusFrameRate;
  }
  if(frameRate != presentation->statusRight.text()) {
    presentation->statusRight.setText(frameRate);
  }
}

auto Program::captureScreenshot() -> bool {
  if(emulator->loaded() && screenshot.data) {
    if(auto filename = screenshotPath()) {
      if(Encode::BMP::create(filename,
        (const uint32_t*)screenshot.data, screenshot.pitch, screenshot.width, screenshot.height, false
      )) {
        showMessage({"Captured screenshot [", Location::file(filename), "]"});
        return true;
      }
    }
  }
  return false;
}

auto Program::paused() -> bool {
  if(!emulator->loaded()) return true;
  if(presentation->pauseEmulation.checked()) return true;
  if(!focused() && settingsWindow->configuration.pauseEmulation.checked()) return true;
  return false;
}

auto Program::focused() -> bool {
  //exclusive mode creates its own top-level window: presentation window will not have focus
  if(video && video.exclusive()) return true;
  if(presentation && presentation->focused()) return true;
  return false;
}
