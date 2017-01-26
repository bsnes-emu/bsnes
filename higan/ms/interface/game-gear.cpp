GameGearInterface::GameGearInterface() {
  information.manufacturer = "Sega";
  information.name         = "Game Gear";
  information.overscan     = false;

  media.append({ID::GameGear, "Game Gear", "gg"});

  Port hardware{ID::Port::Hardware, "Hardware"};

  { Device device{ID::Device::GameGearControls, "Controls"};
    device.inputs.append({0, "Up"});
    device.inputs.append({0, "Down"});
    device.inputs.append({0, "Left"});
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "1"});
    device.inputs.append({0, "2"});
    device.inputs.append({0, "Start"});
    hardware.devices.append(device);
  }

  ports.append(move(hardware));
}

auto GameGearInterface::videoSize() -> VideoSize {
  return {160, 144};
}

auto GameGearInterface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 160;
  uint h = 144;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto GameGearInterface::videoFrequency() -> double {
  return 60.0;
}

auto GameGearInterface::videoColors() -> uint32 {
  return 1 << 12;
}

auto GameGearInterface::videoColor(uint32 color) -> uint64 {
  uint4 B = color >> 8;
  uint4 G = color >> 4;
  uint4 R = color >> 0;

  uint64 r = image::normalize(R, 4, 16);
  uint64 g = image::normalize(G, 4, 16);
  uint64 b = image::normalize(B, 4, 16);

  return r << 32 | g << 16 | b << 0;
}

auto GameGearInterface::load(uint id) -> bool {
  if(id == ID::GameGear) return system.load(this, Model::GameGear);
  return false;
}
