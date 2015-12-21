//request from emulation core to load non-volatile media folder
auto Program::loadRequest(uint id, string name, string type, bool required) -> void {
  string location = BrowserDialog()
  .setTitle({"Load ", name})
  .setPath({settings["Library/Location"].text(), name})
  .setFilters({string{name, "|*.", type}})
  .openFolder();
  if(!directory::exists(location)) return;

  mediaPaths(id) = location;
  folderPaths.append(location);
  emulator->load(id);
}

//request from emulation core to load non-volatile media file
auto Program::loadRequest(uint id, string filename, bool required) -> void {
  string pathname = mediaPaths(emulator->group(id));
  string location = {pathname, filename};

  if(filename == "manifest.bml" && !pathname.find(".sys/")) {
    if(!file::exists(location) || settings["Library/IgnoreManifests"].boolean()) {
      if(auto manifest = execute("icarus", "-m", pathname)) {
        memorystream stream{(const uint8*)manifest.data(), manifest.size()};
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
  string pathname = mediaPaths(emulator->group(id));
  string location = {pathname, filename};
  filestream stream{location, file::mode::write};
  return emulator->save(id, stream);
}

auto Program::videoColor(uint source, uint16 a, uint16 r, uint16 g, uint16 b) -> uint32 {
  if(settings["Video/Saturation"].natural() != 100) {
    uint16 grayscale = uclamp<16>((r + g + b) / 3);
    double saturation = settings["Video/Saturation"].natural() * 0.01;
    double inverse = max(0.0, 1.0 - saturation);
    r = uclamp<16>(r * saturation + grayscale * inverse);
    g = uclamp<16>(g * saturation + grayscale * inverse);
    b = uclamp<16>(b * saturation + grayscale * inverse);
  }

  if(settings["Video/Gamma"].natural() != 100) {
    double exponent = settings["Video/Gamma"].natural() * 0.01;
    double reciprocal = 1.0 / 32767.0;
    r = r > 32767 ? r : 32767 * pow(r * reciprocal, exponent);
    g = g > 32767 ? g : 32767 * pow(g * reciprocal, exponent);
    b = b > 32767 ? b : 32767 * pow(b * reciprocal, exponent);
  }

  if(settings["Video/Luminance"].natural() != 100) {
    double luminance = settings["Video/Luminance"].natural() * 0.01;
    r = r * luminance;
    g = g * luminance;
    b = b * luminance;
  }

  a >>= 8;
  r >>= 8;
  g >>= 8;
  b >>= 8;
  return a << 24 | r << 16 | g << 8 | b << 0;
}

auto Program::videoRefresh(const uint32* palette, const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32* output;
  uint length;

  if(video->lock(output, length, width, height)) {
    pitch >>= 2, length >>= 2;

    for(auto y : range(height)) {
      const uint32* sp = data + y * pitch;
      uint32* dp = output + y * length;
      for(auto x : range(width)) {
        *dp++ = palette[*sp++];
      }
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

auto Program::audioSample(int16 lsample, int16 rsample) -> void {
  int samples[] = {lsample, rsample};
  dsp.sample(samples);
  while(dsp.pending()) {
    dsp.read(samples);
    audio->sample(samples[0], samples[1]);
  }
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  if(presentation->focused()) {
    auto guid = emulator->port[port].device[device].input[input].guid;
    auto mapping = (InputMapping*)guid;
    if(mapping) return mapping->poll();
  }
  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
  if(presentation->focused() || !enable) {
    auto guid = emulator->port[port].device[device].input[input].guid;
    auto mapping = (InputMapping*)guid;
    if(mapping) return mapping->rumble(enable);
  }
}

auto Program::dipSettings(const Markup::Node& node) -> uint {
}

auto Program::path(uint group) -> string {
  return mediaPaths(group);
}

auto Program::notify(string text) -> void {
}
