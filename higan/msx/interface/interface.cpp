#include <msx/msx.hpp>

namespace MSX {

auto Interface::information() -> Information {
  Information information;
  information.manufacturer = "";
  information.name         = "MSX";
  information.extension    = ".msx";
  return information;
}

auto Interface::displays() -> vector<Display> {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 4;
  display.width  = 256;
  display.height = 192;
  display.internalWidth  = 256;
  display.internalHeight = 192;
  display.aspectCorrection = 1.0;
  display.refreshRate = 60.0;  //todo: PAL
  return {display};
}

auto Interface::color(uint32 color) -> uint64 {
  switch(color.bits(0,3)) {
  }
  return 0;
}

auto Interface::loaded() -> bool {
  return false;
}

auto Interface::hashes() -> vector<string> {
  return {};
}

auto Interface::manifests() -> vector<string> {
  return {};
}

auto Interface::titles() -> vector<string> {
  return {};
}

auto Interface::load() -> bool {
  return false;
}

auto Interface::save() -> void {
}

auto Interface::unload() -> void {
  save();
}

auto Interface::ports() -> vector<Port> { return {
  {ID::Port::Controller1, "Controller Port 1"},
  {ID::Port::Controller2, "Controller Port 2"}};
}

auto Interface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller1) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  if(port == ID::Port::Controller2) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  return {};
}

auto Interface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Gamepad) return {
    {Type::Hat,    "Up"   },
    {Type::Hat,    "Down" },
    {Type::Hat,    "Left" },
    {Type::Hat,    "Right"},
    {Type::Button, "1"    },
    {Type::Button, "2"    }
  };

  return {};
}

auto Interface::power() -> void {
}

auto Interface::run() -> void {
}

auto Interface::serialize() -> serializer {
  return {};
}

auto Interface::unserialize(serializer& s) -> bool {
  return false;
}

}
