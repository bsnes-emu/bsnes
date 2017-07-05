MasterSystemInterface::MasterSystemInterface() {
  information.manufacturer = "Sega";
  information.name         = "Master System";
  information.overscan     = true;

  media.append({ID::MasterSystem, "Master System", "ms"});

  Port hardware{ID::Port::Hardware, "Hardware"};
  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};

  { Device device{ID::Device::MasterSystemControls, "Controls"};
    device.inputs.append({0, "Reset"});
    device.inputs.append({0, "Pause"});
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

auto MasterSystemInterface::videoResolution() -> VideoSize {
  return {256, 240};
}

auto MasterSystemInterface::videoSize(uint width, uint height, bool aspectCorrection, uint cropHorizontal, uint cropVertical) -> VideoSize {
  double widthDivider = (256 - cropHorizontal * 2) * (aspectCorrection ? 8.0 / 7.0 : 1.0);
  double heightDivider = (240 - cropVertical * 2);
  uint multiplier = min(width / widthDivider, height / heightDivider);
  return {uint(widthDivider * multiplier), uint(heightDivider * multiplier)};
}

auto MasterSystemInterface::videoCrop(const uint32*& data, uint& width, uint& height, uint cropHorizontal, uint cropVertical) -> void {
  data += cropVertical * 256 + cropHorizontal;
  width -= cropHorizontal * 2;
  height -= cropVertical * 2;
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
