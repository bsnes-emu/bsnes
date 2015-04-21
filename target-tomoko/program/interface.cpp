//request from emulation core to load non-volatile media folder
auto Program::loadRequest(unsigned id, string name, string type) -> void {
}

//request from emulation core to load non-volatile media file
auto Program::loadRequest(unsigned id, string path) -> void {
  string location = {mediaPaths(emulator->group(id)), path};
  if(!file::exists(location)) return;
  mmapstream stream{location};
  return emulator->load(id, stream);
}

//request from emulation core to save non-volatile media file
auto Program::saveRequest(unsigned id, string path) -> void {
  string location = {mediaPaths(emulator->group(id)), path};
  filestream stream{location, file::mode::write};
  return emulator->save(id, stream);
}

auto Program::videoColor(unsigned source, uint16 alpha, uint16 red, uint16 green, uint16 blue) -> uint32 {
  alpha >>= 8;
  red   >>= 8;
  green >>= 8;
  blue  >>= 8;
  return alpha << 24 | red << 16 | green << 8 | blue << 0;
}

auto Program::videoRefresh(const uint32* palette, const uint32* data, unsigned pitch, unsigned width, unsigned height) -> void {
  uint32* output;
  unsigned length;

  if(video.lock(output, length, width, height)) {
    pitch >>= 2, length >>= 2;

    for(auto y : range(height)) {
      const uint32* sp = data + y * pitch;
      uint32* dp = output + y * length;
      for(auto x : range(width)) {
        *dp++ = palette[*sp++];
      }
    }

    video.unlock();
    video.refresh();
  }

  static unsigned frameCounter = 0;
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
  signed samples[] = {lsample, rsample};
  dsp.sample(samples);
  while(dsp.pending()) {
    dsp.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

auto Program::inputPoll(unsigned port, unsigned device, unsigned input) -> int16 {
  if(presentation->focused()) {
    auto guid = emulator->port[port].device[device].input[input].guid;
    auto mapping = (InputMapping*)guid;
    if(mapping) return mapping->poll();
  }
  return 0;
}

auto Program::inputRumble(unsigned port, unsigned device, unsigned input, bool enable) -> void {
}

auto Program::dipSettings(const Markup::Node& node) -> unsigned {
}

auto Program::path(unsigned group) -> string {
  return mediaPaths(group);
}

auto Program::notify(string text) -> void {
}
