auto Program::loadRequest(uint id, string name, string type, bool required) -> void {
  string location = BrowserDialog()
  .setTitle({"Load ", name})
  .setPath({userpath(), "Emulation/", name})
  .setFilters({string{name, "|*.", type}})
  .openFolder();
  if(!directory::exists(location)) return;

  mediaPaths(id) = location;
  folderPaths.append(location);
  emulator->load(id);
}

auto Program::loadRequest(uint id, string filename, bool required) -> void {
  string pathname = mediaPaths(emulator->group(id));
  string location = {pathname, filename};

  if(filename == "manifest.bml" && pathname && !pathname.endsWith("sys/")) {
    if(!file::exists(location)) {
      if(auto manifest = execute("icarus", "--manifest", pathname)) {
        memorystream stream{(const uint8_t*)manifest.data(), manifest.size()};
        return emulator->load(id, stream);
      }
    }
  }

  if(file::exists(location)) {
    mmapstream stream{location};
    return emulator->load(id, stream);
  }

  if(required) MessageDialog().setTitle("loki").setText({
    "Missing required file: ", nall::filename(location), "\n\n",
    "From location:\n", nall::pathname(location)
  }).error();
}

auto Program::saveRequest(uint id, string filename) -> void {
  string pathname = mediaPaths(emulator->group(id));
  string location = {pathname, filename};

//filestream stream{location, file::mode::write};
//return emulator->save(id, stream);
}

auto Program::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
  uint32_t* output;
  uint length;

  if(video->lock(output, length, width, height)) {
    pitch >>= 2, length >>= 2;

    for(auto y : range(height)) {
      memory::copy(output + y * length, data + y * pitch, width * sizeof(uint32));
    }

    video->unlock();
    video->refresh();
  }
}

auto Program::audioSample(int16 left, int16 right) -> void {
  int samples[] = {left, right};
  dsp.sample(samples);
  while(dsp.pending()) {
    dsp.read(samples);
    audio->sample(samples[0], samples[1]);
  }
}

auto Program::inputPoll(uint port, uint device, uint input) -> int16 {
  if(!presentation->focused()) return 0;

  shared_pointer<HID::Keyboard> keyboard;
  for(auto& device : devices) {
    if(device->isKeyboard()) {
      keyboard = device;
      break;
    }
  }

  if(port == (uint)SFC::Device::Port::Controller1) {
    if(device == (uint)SFC::Device::ID::Gamepad) {
      #define map(id, name) \
        case id: \
          if(auto code = keyboard->buttons().find(name)) { \
            return keyboard->buttons().input(*code).value(); \
          } \
          break; \

      switch(input) {
        map(SFC::Gamepad::Up, "Up");
        map(SFC::Gamepad::Down, "Down");
        map(SFC::Gamepad::Left, "Left");
        map(SFC::Gamepad::Right, "Right");
        map(SFC::Gamepad::B, "Z");
        map(SFC::Gamepad::A, "X");
        map(SFC::Gamepad::Y, "A");
        map(SFC::Gamepad::X, "S");
        map(SFC::Gamepad::L, "D");
        map(SFC::Gamepad::R, "C");
        map(SFC::Gamepad::Select, "Apostrophe");
        map(SFC::Gamepad::Start, "Return");
      }

      #undef map
    }
  }

  return 0;
}

auto Program::inputRumble(uint port, uint device, uint input, bool enable) -> void {
}

auto Program::dipSettings(const Markup::Node& node) -> uint {
  return 0;
}

auto Program::path(uint group) -> string {
  return mediaPaths(group);
}

auto Program::notify(string text) -> void {
}
