WonderSwanInterface::WonderSwanInterface() {
  information.manufacturer = "Bandai";
  information.name         = "WonderSwan";
  information.overscan     = false;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::WonderSwan, "WonderSwan", "ws"});

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

auto WonderSwanInterface::manifest() -> string {
  return cartridge.information.manifest;
}

auto WonderSwanInterface::title() -> string {
  return cartridge.information.title;
}

auto WonderSwanInterface::videoSize() -> VideoSize {
  return {224, 224};
}

auto WonderSwanInterface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 224;
  uint h = 224;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto WonderSwanInterface::videoFrequency() -> double {
  return 3072000.0 / (159.0 * 256.0);  //~75.47hz
}

auto WonderSwanInterface::videoColors() -> uint32 {
  return 1 << 12;
}

auto WonderSwanInterface::videoColor(uint32 color) -> uint64 {
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

auto WonderSwanInterface::audioFrequency() -> double {
  return 3072000.0;
}

auto WonderSwanInterface::loaded() -> bool {
  return system.loaded();
}

auto WonderSwanInterface::sha256() -> string {
  return cartridge.information.sha256;
}

auto WonderSwanInterface::load(uint id) -> bool {
  if(id == ID::WonderSwan) return system.load(this, Model::WonderSwan);
  return false;
}

auto WonderSwanInterface::save() -> void {
  system.save();
}

auto WonderSwanInterface::unload() -> void {
  save();
  system.unload();
}

auto WonderSwanInterface::power() -> void {
  system.power();
}

auto WonderSwanInterface::run() -> void {
  system.run();
}

auto WonderSwanInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto WonderSwanInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto WonderSwanInterface::cheatSet(const string_vector& list) -> void {
  cheat.assign(list);
}

auto WonderSwanInterface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto WonderSwanInterface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto WonderSwanInterface::set(const string& name, const any& value) -> bool {
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
