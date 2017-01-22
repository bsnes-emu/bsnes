GameBoyColorInterface::GameBoyColorInterface() {
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy Color";
  information.overscan     = false;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::GameBoyColor, "Game Boy Color", "gb"});

  Port hardwarePort{ID::Port::Hardware, "Hardware"};

  { Device device{ID::Device::Controls, "Controls"};
    device.inputs.append({0, "Up"    });
    device.inputs.append({0, "Down"  });
    device.inputs.append({0, "Left"  });
    device.inputs.append({0, "Right" });
    device.inputs.append({0, "B"     });
    device.inputs.append({0, "A"     });
    device.inputs.append({0, "Select"});
    device.inputs.append({0, "Start" });
    hardwarePort.devices.append(device);
  }

  ports.append(move(hardwarePort));
}

auto GameBoyColorInterface::manifest() -> string {
  return cartridge.manifest();
}

auto GameBoyColorInterface::title() -> string {
  return cartridge.title();
}

auto GameBoyColorInterface::videoSize() -> VideoSize {
  return {160, 144};
}

auto GameBoyColorInterface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 160;
  uint h = 144;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto GameBoyColorInterface::videoFrequency() -> double {
  return 4194304.0 / (154.0 * 456.0);
}

auto GameBoyColorInterface::videoColors() -> uint32 {
  return 1 << 15;
}

auto GameBoyColorInterface::videoColor(uint32 color) -> uint64 {
  uint r = color.bits( 0, 4);
  uint g = color.bits( 5, 9);
  uint b = color.bits(10,14);

  uint64_t R = image::normalize(r, 5, 16);
  uint64_t G = image::normalize(g, 5, 16);
  uint64_t B = image::normalize(b, 5, 16);

  if(settings.colorEmulation) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(960, R), 10, 16);
    G = image::normalize(min(960, G), 10, 16);
    B = image::normalize(min(960, B), 10, 16);
  }

  return R << 32 | G << 16 | B << 0;
}

auto GameBoyColorInterface::audioFrequency() -> double {
  return 4194304.0 / 2.0;
}

auto GameBoyColorInterface::loaded() -> bool {
  return system.loaded();
}

auto GameBoyColorInterface::sha256() -> string {
  return cartridge.sha256();
}

auto GameBoyColorInterface::load(uint id) -> bool {
  if(id == ID::GameBoyColor) return system.load(this, System::Revision::GameBoyColor);
  return false;
}

auto GameBoyColorInterface::save() -> void {
  system.save();
}

auto GameBoyColorInterface::unload() -> void {
  save();
  system.unload();
}

auto GameBoyColorInterface::power() -> void {
  system.power();
}

auto GameBoyColorInterface::run() -> void {
  system.run();
}

auto GameBoyColorInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto GameBoyColorInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto GameBoyColorInterface::cheatSet(const string_vector& list) -> void {
  cheat.assign(list);
}

auto GameBoyColorInterface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto GameBoyColorInterface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto GameBoyColorInterface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    system.configureVideoEffects();
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    system.configureVideoPalette();
    return true;
  }

  return false;
}
