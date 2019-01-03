#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

#include "neo-geo-pocket.cpp"
#include "neo-geo-pocket-color.cpp"

//todo: add correct values
auto Interface::display() -> Display {
  Display display;
  display.type   = Display::Type::LCD;
  display.colors = 1;
  display.width  = 320;
  display.height = 240;
  display.internalWidth  = 320;
  display.internalHeight = 240;
  display.aspectCorrection = 1.0;
  display.refreshRate = 60.0;
  return display;
}

auto Interface::color(uint32 color) -> uint64 {
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
    {Type::Hat, "Up"   },
    {Type::Hat, "Down" },
    {Type::Hat, "Left" },
    {Type::Hat, "Right"}
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
