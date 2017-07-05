auto Program::path(uint id) -> string {
  return mediumPaths(id);
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

auto Program::load(uint id, string name, string type, string_vector options) -> Emulator::Platform::Load {
  string location, option;
  if(mediumQueue) {
    auto entry = mediumQueue.takeLeft().split("|", 1L);
    location = entry.right();
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
    mediumQueue.reset();
    return {};
  }

  uint pathID = mediumPaths.size();
  mediumPaths.append(location);
  return {pathID, option};
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  pitch >>= 2;

  if(emulator->information.overscan && settings["Video/Overscan/Mask"].boolean()) {
    uint horizontal = settings["Video/Overscan/Horizontal"].natural();
    uint vertical = settings["Video/Overscan/Vertical"].natural();
    emulator->videoCrop(data, width, height, horizontal, vertical);
  }

  if(video->lock(output, length, width, height)) {
    length >>= 2;

    for(auto y : range(height)) {
      memory::copy(output + y * length, data + y * pitch, width * sizeof(uint32));
    }

    video->unlock();
    video->refresh();
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
  int16 left  = sclamp<16>(samples[0] * 32768.0);
  int16 right = sclamp<16>(samples[1] * 32768.0);
  audio->sample(left, right);
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  if(presentation->focused() || settings["Input/FocusLoss/AllowInput"].boolean()) {
    inputManager->poll();
    auto& mapping = inputManager->emulator->ports[port].devices[device].mappings[input];
    return mapping.poll();
  }
  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
  if(presentation->focused() || settings["Input/FocusLoss/AllowInput"].boolean() || !enable) {
    auto& mapping = inputManager->emulator->ports[port].devices[device].mappings[input];
    return mapping.rumble(enable);
  }
}

auto Program::dipSettings(Markup::Node node) -> uint {
  return 0;
}

auto Program::notify(string text) -> void {
}
