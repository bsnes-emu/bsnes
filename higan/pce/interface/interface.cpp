#include <pce/pce.hpp>

namespace PCEngine {

Model model;
Settings settings;
#include "pc-engine.cpp"
#include "supergrafx.cpp"

auto Interface::display() -> Display {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 9;
  display.width  = 280;
  display.height = 240;
  display.internalWidth  = 1120;
  display.internalHeight =  240;
  display.aspectCorrection = 8.0 / 7.0;
  display.refreshRate = (system.colorburst() * 6.0) / (262.0 * 1365.0);
  return display;
}

auto Interface::color(uint32 color) -> uint64 {
  uint3 B = color.bits(0,2);
  uint3 R = color.bits(3,5);
  uint3 G = color.bits(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto Interface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto Interface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::ports() -> vector<Port> { return {
  {ID::Port::Controller, "Controller"}};
}

auto Interface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller) return {
    {ID::Device::Gamepad, "Gamepad"}
  };

  return {};
}

auto Interface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Gamepad) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "II"    },
    {Type::Button,  "I"     },
    {Type::Control, "Select"},
    {Type::Control, "Run"   }
  };

  return {};
}

auto Interface::connected(uint port) -> uint {
  if(port == ID::Port::Controller) return settings.controllerPort;
  return 0;
}

auto Interface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller) controllerPort.connect(settings.controllerPort = device);
}

auto Interface::power() -> void {
  system.power();
}

auto Interface::run() -> void {
  system.run();
}

auto Interface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto Interface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto Interface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

auto Interface::cap(const string& name) -> bool {
  return false;
}

auto Interface::get(const string& name) -> any {
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  return false;
}

}
