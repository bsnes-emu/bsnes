auto Program::path(uint id) -> string {
  return mediumPaths(id);
}

auto Program::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  if(auto result = vfs::fs::file::open({path(id), name}, mode)) return result;
  if(name == "manifest.bml") {
    if(auto manifest = execute("icarus", "--manifest", path(id))) {
      return vfs::memory::file::open(manifest.output.data<uint8_t>(), manifest.output.size());
    }
  }
  return {};
}

auto Program::load(uint id, string name, string type, bool required) -> void {
  string location = BrowserDialog()
  .setTitle({"Load ", name})
  .setPath({settings["Library/Location"].text(), name})
  .setFilters({string{name, "|*.", type}, "All|*.*"})
  .openFolder();
  if(!directory::exists(location)) return;

  mediumPaths(id) = location;
  folderPaths.append(location);
  emulator->load(id);
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  if(video->lock(output, length, width, height)) {
    pitch >>= 2, length >>= 2;

    for(auto y : range(height)) {
      memory::copy(output + y * length, data + y * pitch, width * sizeof(uint32));
    }

    if(emulator->information.overscan && settings["Video/Overscan/Mask"].boolean()) {
      auto h = settings["Video/Overscan/Horizontal"].natural();
      auto v = settings["Video/Overscan/Vertical"].natural();

      if(h) for(auto y : range(height)) {
        memory::fill(output + y * length, 4 * h);
        memory::fill(output + y * length + (width - h), 4 * h);
      }

      if(v) for(auto y : range(v)) {
        memory::fill(output + y * length, 4 * width);
        memory::fill(output + (height - 1 - y) * length, 4 * width);
      }
    }

    video->unlock();
    video->refresh();
  }

  static uint frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
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
    auto guid = emulator->ports[port].devices[device].inputs[input].guid;
    auto mapping = (InputMapping*)guid;
    if(mapping) return mapping->poll();
  }
  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
  if(presentation->focused() || settings["Input/FocusLoss/AllowInput"].boolean() || !enable) {
    auto guid = emulator->ports[port].devices[device].inputs[input].guid;
    auto mapping = (InputMapping*)guid;
    if(mapping) return mapping->rumble(enable);
  }
}

auto Program::dipSettings(const Markup::Node& node) -> uint {
  return 0;
}

auto Program::notify(string text) -> void {
}
