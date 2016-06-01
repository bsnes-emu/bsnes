//request from emulation core to load non-volatile media folder
auto Program::loadRequest(uint id, string name, string type, bool required) -> void {
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

//request from emulation core to load non-volatile media file
auto Program::loadRequest(uint id, string filename, bool required) -> void {
  string pathname = mediumPaths(emulator->group(id));
  string location = {pathname, filename};

  if(filename == "manifest.bml" && pathname && !pathname.endsWith(".sys/")) {
    if(!file::exists(location) || settings["Library/IgnoreManifests"].boolean()) {
      if(auto manifest = execute("icarus", "--manifest", pathname)) {
        memorystream stream{manifest.output.data<uint8_t>(), manifest.output.size()};
        return emulator->load(id, stream);
      }
    }
  }

  if(file::exists(location)) {
    mmapstream stream{location};
    return emulator->load(id, stream);
  }

  if(required) MessageDialog().setTitle("higan").setText({
    "Missing required file: ", nall::filename(location), "\n\n",
    "From location:\n", nall::pathname(location)
  }).error();
}

//request from emulation core to save non-volatile media file
auto Program::saveRequest(uint id, string filename) -> void {
  string pathname = mediumPaths(emulator->group(id));
  string location = {pathname, filename};
  if(!pathname) return;  //should never occur

  filestream stream{location, file::mode::write};
  return emulator->save(id, stream);
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

auto Program::path(uint group) -> string {
  return mediumPaths(group);
}

auto Program::notify(string text) -> void {
}
