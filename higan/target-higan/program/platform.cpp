auto Program::path(uint id) -> string {
  return gamePaths(id);
}

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  if(name == "manifest.bml" && !path(id).endsWith(".sys/")) {
    if(!file::exists({path(id), name}) || settings["Library/IgnoreManifests"].boolean()) {
      if(auto manifest = execute("icarus", "--manifest", path(id))) {
        return vfs::memory::file::open(manifest.output.data<uint8_t>(), manifest.output.size());
      }
    }
  }

  if(auto result = vfs::fs::file::open({path(id), name}, mode)) return result;

  if(required) {
    MessageDialog()
    .setTitle({"Error"})
    .setText({"Error: missing required file:\n\n", path(id), name})
    .error();
  }

  return {};
}

auto Program::load(uint id, string name, string type, vector<string> options) -> Emulator::Platform::Load {
  string location, option;
  if(gameQueue) {
    auto entry = gameQueue.takeLeft().split("|", 1L);
    location = entry.right();
    if(entry.size() == 1) option = options(0);
    if(entry.size() == 2) option = entry.left();
  } else {
    BrowserDialog dialog;
    location = dialog
    .setTitle({"Load ", name})
    .setPath({settings["Library/Location"].text(), name})
    .setFilters({string{name, "|*.", type}, "All|*.*"})
    .setOptions(options)
    .openFolder();
    option = dialog.option();
  }
  if(!directory::exists(location)) {
    gameQueue.reset();
    return {};
  }

  uint pathID = gamePaths.size();
  gamePaths.append(location);
  return {pathID, option};
}

auto Program::videoRefresh(uint displayID, const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  pitch >>= 2;

  auto display = emulator->displays()[displayID];
  if(display.type == Emulator::Interface::Display::Type::CRT) {
    uint overscanHorizontal = settings["Video/Overscan/Horizontal"].natural();
    uint overscanVertical = settings["Video/Overscan/Vertical"].natural();
    overscanHorizontal *= display.internalWidth / display.width;
    overscanVertical *= display.internalHeight / display.height;
    data += overscanVertical * pitch + overscanHorizontal;
    width -= overscanHorizontal * 2;
    height -= overscanVertical * 2;
  }

  if(video->acquire(output, length, width, height)) {
    length >>= 2;

    for(auto y : range(height)) {
      memory::copy<uint32>(output + y * length, data + y * pitch, width);
    }

    video->release();
    video->output();
  }

  static uint frameCounter = 0;
  static uint64 previous, current;
  frameCounter++;

  current = chrono::timestamp();
  if(current != previous) {
    previous = current;
    statusText = {"FPS: ", frameCounter};
    frameCounter = 0;
  }
}

auto Program::audioSample(const double* samples, uint channels) -> void {
  audio->output(samples);
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  if(focused() || settingsManager->input.allowInput.checked()) {
    inputManager->poll();
    if(auto mapping = inputManager->mapping(port, device, input)) {
      return mapping->poll();
    }
  }
  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
  if(focused() || settingsManager->input.allowInput.checked() || !enable) {
    if(auto mapping = inputManager->mapping(port, device, input)) {
      return mapping->rumble(enable);
    }
  }
}

auto Program::dipSettings(Markup::Node node) -> uint {
  return 0;
}

auto Program::notify(string text) -> void {
}
