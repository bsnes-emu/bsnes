auto GameGearInterface::information() -> Information {
  Information information;
  information.manufacturer = "Sega";
  information.name         = "Game Gear";
  information.extension    = "gg";
  return information;
}

auto GameGearInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::LCD;
  display.colors = 1 << 12;
  display.width  = 160;
  display.height = 144;
  display.internalWidth  = 160;
  display.internalHeight = 144;
  display.aspectCorrection = 1.0;
  return display;
}

auto GameGearInterface::color(uint32 color) -> uint64 {
  uint4 B = color >> 8;
  uint4 G = color >> 4;
  uint4 R = color >> 0;

  uint64 r = image::normalize(R, 4, 16);
  uint64 g = image::normalize(G, 4, 16);
  uint64 b = image::normalize(B, 4, 16);

  return r << 32 | g << 16 | b << 0;
}

auto GameGearInterface::ports() -> vector<Port> { return {
  {ID::Port::Hardware, "Hardware"}};
}

auto GameGearInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Hardware) return {
    {ID::Device::GameGearControls, "Controls"}
  };

  return {};
}

auto GameGearInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::GameGearControls) return {
    {Type::Hat,     "Up"   },
    {Type::Hat,     "Down" },
    {Type::Hat,     "Left" },
    {Type::Hat,     "Right"},
    {Type::Button,  "1"    },
    {Type::Button,  "2"    },
    {Type::Control, "Start"}
  };

  return {};
}

auto GameGearInterface::load() -> bool {
  return system.load(this, System::Model::GameGear);
}
