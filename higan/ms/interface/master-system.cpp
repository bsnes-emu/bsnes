auto MasterSystemInterface::information() -> Information {
  Information information;
  information.manufacturer = "Sega";
  information.name         = "Master System";
  information.extension    = "ms";
  return information;
}

auto MasterSystemInterface::displays() -> vector<Display> {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 6;
  display.width  = 256;
  display.height = 240;
  display.internalWidth  = 256;
  display.internalHeight = 240;
  display.aspectCorrection = 8.0 / 7.0;
  if(Region::NTSC()) display.refreshRate = (system.colorburst() * 15.0 / 5.0) / (262.0 * 684.0);
  if(Region::PAL())  display.refreshRate = (system.colorburst() * 15.0 / 5.0) / (312.0 * 684.0);
  return {display};
}

auto MasterSystemInterface::color(uint32 color) -> uint64 {
  uint2 B = color >> 4;
  uint2 G = color >> 2;
  uint2 R = color >> 0;

  uint64 r = image::normalize(R, 2, 16);
  uint64 g = image::normalize(G, 2, 16);
  uint64 b = image::normalize(B, 2, 16);

  return r << 32 | g << 16 | b << 0;
}

auto MasterSystemInterface::ports() -> vector<Port> { return {
  {ID::Port::Controller1, "Controller Port 1"},
  {ID::Port::Controller2, "Controller Port 2"},
  {ID::Port::Hardware,    "Hardware"         }};
}

auto MasterSystemInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller1) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  if(port == ID::Port::Controller2) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  if(port == ID::Port::Hardware) return {
    {ID::Device::MasterSystemControls, "Controls"}
  };

  return {};
}

auto MasterSystemInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::None) return {
  };

  if(device == ID::Device::Gamepad) return {
    {Type::Hat,    "Up"   },
    {Type::Hat,    "Down" },
    {Type::Hat,    "Left" },
    {Type::Hat,    "Right"},
    {Type::Button, "1"    },
    {Type::Button, "2"    }
  };

  if(device == ID::Device::MasterSystemControls) return {
    {Type::Control, "Reset"},
    {Type::Control, "Power"}
  };

  return {};
}

auto MasterSystemInterface::load() -> bool {
  return system.load(this, System::Model::MasterSystem);
}

auto MasterSystemInterface::connected(uint port) -> uint {
  if(port == ID::Port::Controller1) return settings.controllerPort1;
  if(port == ID::Port::Controller2) return settings.controllerPort2;
  if(port == ID::Port::Hardware) return ID::Device::MasterSystemControls;
  return 0;
}

auto MasterSystemInterface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(settings.controllerPort1 = device);
  if(port == ID::Port::Controller2) controllerPort2.connect(settings.controllerPort2 = device);
}
