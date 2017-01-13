MasterSystemInterface::MasterSystemInterface() {
  information.manufacturer = "Sega";
  information.name         = "Master System";
  information.overscan     = true;
  information.resettable   = false;

  information.capability.states = false;
  information.capability.cheats = false;

  media.append({ID::MasterSystem, "Master System", "ms"});

  Port hardware{ID::Port::Hardware, "Hardware"};
  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};

  { Device device{ID::Device::MasterSystemControls, "Controls"};
    device.inputs.append({0, "Reset"});
    hardware.devices.append(device);
  }

  { Device device{ID::Device::None, "None"};
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::Gamepad, "Gamepad"};
    device.inputs.append({0, "Up"});
    device.inputs.append({0, "Down"});
    device.inputs.append({0, "Left"});
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "1"});
    device.inputs.append({0, "2"});
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
  }

  ports.append(move(hardware));
  ports.append(move(controllerPort1));
  ports.append(move(controllerPort2));
}

auto MasterSystemInterface::manifest() -> string {
  return cartridge.manifest();
}

auto MasterSystemInterface::title() -> string {
  return cartridge.title();
}

auto MasterSystemInterface::videoSize() -> VideoSize {
  return {256, 240};
}

auto MasterSystemInterface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  auto a = arc ? 8.0 / 7.0 : 1.0;
  uint w = 256;
  uint h = 240;
  uint m = min(width / (w * a), height / h);
  return {uint(w * a * m), uint(h * m)};
}

auto MasterSystemInterface::videoFrequency() -> double {
  return 60.0;
}

auto MasterSystemInterface::videoColors() -> uint32 {
  return 1 << 6;
}

auto MasterSystemInterface::videoColor(uint32 color) -> uint64 {
  uint2 B = color >> 4;
  uint2 G = color >> 2;
  uint2 R = color >> 0;

  uint64 r = image::normalize(R, 2, 16);
  uint64 g = image::normalize(G, 2, 16);
  uint64 b = image::normalize(B, 2, 16);

  return r << 32 | g << 16 | b << 0;
}

auto MasterSystemInterface::audioFrequency() -> double {
  return 44'100.0;
}

auto MasterSystemInterface::loaded() -> bool {
  return system.loaded();
}

auto MasterSystemInterface::load(uint id) -> bool {
  if(id == ID::MasterSystem) return system.load(this, Model::MasterSystem);
  return false;
}

auto MasterSystemInterface::save() -> void {
  system.save();
}

auto MasterSystemInterface::unload() -> void {
  system.unload();
}

auto MasterSystemInterface::connect(uint port, uint device) -> void {
  peripherals.connect(port, device);
}

auto MasterSystemInterface::power() -> void {
  system.power();
}

auto MasterSystemInterface::run() -> void {
  system.run();
}

auto MasterSystemInterface::serialize() -> serializer {
  return {};
}

auto MasterSystemInterface::unserialize(serializer& s) -> bool {
  return false;
}

auto MasterSystemInterface::cap(const string& name) -> bool {
  return false;
}

auto MasterSystemInterface::get(const string& name) -> any {
  return {};
}

auto MasterSystemInterface::set(const string& name, const any& value) -> bool {
  return false;
}
