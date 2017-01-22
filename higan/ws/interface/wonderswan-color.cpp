WonderSwanColorInterface::WonderSwanColorInterface() {
  information.manufacturer = "Bandai";
  information.name         = "WonderSwan Color";
  information.overscan     = false;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::WonderSwanColor, "WonderSwan Color", "wsc"});

  Port hardwareHorizontalPort{ID::Port::HardwareHorizontal, "Hardware - Horizontal"};
  Port hardwareVerticalPort{ID::Port::HardwareVertical, "Hardware - Vertical"};

  { Device device{ID::Device::Controls, "Controls"};
    device.inputs.append({0, "Y1"});
    device.inputs.append({0, "Y2"});
    device.inputs.append({0, "Y3"});
    device.inputs.append({0, "Y4"});
    device.inputs.append({0, "X1"});
    device.inputs.append({0, "X2"});
    device.inputs.append({0, "X3"});
    device.inputs.append({0, "X4"});
    device.inputs.append({0, "B"});
    device.inputs.append({0, "A"});
    device.inputs.append({0, "Start"});
    device.inputs.append({0, "Rotate"});
    hardwareHorizontalPort.devices.append(device);
    hardwareVerticalPort.devices.append(device);
  }

  ports.append(move(hardwareHorizontalPort));
  ports.append(move(hardwareVerticalPort));
}

auto WonderSwanColorInterface::manifest() -> string {
  return cartridge.information.manifest;
}

auto WonderSwanColorInterface::title() -> string {
  return cartridge.information.title;
}

auto WonderSwanColorInterface::videoSize() -> VideoSize {
  return {224, 224};
}

auto WonderSwanColorInterface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 224;
  uint h = 224;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto WonderSwanColorInterface::videoFrequency() -> double {
  return 3072000.0 / (159.0 * 256.0);  //~75.47hz
}

auto WonderSwanColorInterface::videoColors() -> uint32 {
  return 1 << 12;
}

auto WonderSwanColorInterface::videoColor(uint32 color) -> uint64 {
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  uint64_t R = image::normalize(r, 4, 16);
  uint64_t G = image::normalize(g, 4, 16);
  uint64_t B = image::normalize(b, 4, 16);

  if(settings.colorEmulation) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
  }

  return R << 32 | G << 16 | B << 0;
}

auto WonderSwanColorInterface::audioFrequency() -> double {
  return 3072000.0;
}

auto WonderSwanColorInterface::loaded() -> bool {
  return system.loaded();
}

auto WonderSwanColorInterface::sha256() -> string {
  return cartridge.information.sha256;
}

auto WonderSwanColorInterface::load(uint id) -> bool {
  if(id == ID::WonderSwanColor) return system.load(this, Model::WonderSwanColor);
  return false;
}

auto WonderSwanColorInterface::save() -> void {
  system.save();
}

auto WonderSwanColorInterface::unload() -> void {
  save();
  system.unload();
}

auto WonderSwanColorInterface::power() -> void {
  system.power();
}

auto WonderSwanColorInterface::run() -> void {
  system.run();
}

auto WonderSwanColorInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto WonderSwanColorInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto WonderSwanColorInterface::cheatSet(const string_vector& list) -> void {
  cheat.assign(list);
}

auto WonderSwanColorInterface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto WonderSwanColorInterface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto WonderSwanColorInterface::set(const string& name, const any& value) -> bool {
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
