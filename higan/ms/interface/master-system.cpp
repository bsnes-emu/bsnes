MasterSystemInterface::MasterSystemInterface() {
  information.manufacturer = "Sega";
  information.name         = "Master System";
  information.overscan     = true;

  media.append({ID::MasterSystem, "Master System", "ms"});

  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};
  Port hardware{ID::Port::Hardware, "Hardware"};

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

  { Device device{ID::Device::MasterSystemControls, "Controls"};
    device.inputs.append({0, "Reset"});
    device.inputs.append({0, "Pause"});
    hardware.devices.append(device);
  }

  ports.append(move(controllerPort1));
  ports.append(move(controllerPort2));
  ports.append(move(hardware));
}

auto MasterSystemInterface::videoInformation() -> VideoInformation {
  VideoInformation vi;
  vi.width  = 256;
  vi.height = 240;
  vi.internalWidth  = 256;
  vi.internalHeight = 240;
  vi.aspectCorrection = 8.0 / 7.0;
  if(Region::NTSC()) vi.refreshRate = (system.colorburst() * 15.0 / 5.0) / (262.0 * 684.0);
  if(Region::PAL())  vi.refreshRate = (system.colorburst() * 15.0 / 5.0) / (312.0 * 684.0);
  return vi;
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

auto MasterSystemInterface::load(uint id) -> bool {
  if(id == ID::MasterSystem) return system.load(this, System::Model::MasterSystem);
  return false;
}

auto MasterSystemInterface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(settings.controllerPort1 = device);
  if(port == ID::Port::Controller2) controllerPort2.connect(settings.controllerPort2 = device);
}
