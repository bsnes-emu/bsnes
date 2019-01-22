#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

#include "neo-geo-pocket.cpp"
#include "neo-geo-pocket-color.cpp"

//todo: add correct values
auto Interface::display() -> Display {
  Display display;
  display.type   = Display::Type::LCD;
  display.colors = 1 << 12;
  display.width  = 160;
  display.height = 152;
  display.internalWidth  = 160;
  display.internalHeight = 152;
  display.aspectCorrection = 1.0;
  return display;
}

auto Interface::color(uint32 color) -> uint64 {
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  natural R = image::normalize(r, 4, 16);
  natural G = image::normalize(g, 4, 16);
  natural B = image::normalize(b, 4, 16);

  return R << 32 | G << 16 | B << 0;
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
}

auto Interface::unload() -> void {
  save();
}

auto Interface::ports() -> vector<Port> { return {
  {ID::Port::Hardware, "Hardware"}};
}

auto Interface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Hardware) return {
    {ID::Device::Controls, "Controls"}
  };

  return {};
}

auto Interface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Controls) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "A"     },
    {Type::Button,  "B"     },
    {Type::Control, "Option"}
  };

  return {};
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

}
