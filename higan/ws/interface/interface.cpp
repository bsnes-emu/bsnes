#include <ws/ws.hpp>

namespace WonderSwan {

Settings settings;
#include "wonderswan.cpp"
#include "wonderswan-color.cpp"
#include "pocket-challenge-v2.cpp"

auto Interface::display() -> Display {
  Display display;
  display.type = Display::Type::LCD;
  display.colors    = 1 << 12;
  display.width  = 224;
  display.height = 144;
  display.internalWidth  = 224;
  display.internalHeight = 144;
  display.aspectCorrection = 1.0;
  display.refreshRate = 3'072'000.0 / (159.0 * 256.0);
  if(settings.rotateLeft) {
    swap(display.width, display.height);
    swap(display.internalWidth, display.internalHeight);
  }
  return display;
}

//todo: the WonderSwan and PocketChallengeV2 interfaces should be generating grayscale shades
//instead, the PPU is currently selecting grayscale shades from the WonderSwanColor palette

auto Interface::color(uint32 color) -> uint64 {
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  uint64_t R = image::normalize(r, 4, 16);
  uint64_t G = image::normalize(g, 4, 16);
  uint64_t B = image::normalize(b, 4, 16);

  if(settings.colorEmulation) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
  }

  return R << 32 | G << 16 | B << 0;
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
    {Type::Hat,     "Y1"   },
    {Type::Hat,     "Y2"   },
    {Type::Hat,     "Y3"   },
    {Type::Hat,     "Y4"   },
    {Type::Hat,     "X1"   },
    {Type::Hat,     "X2"   },
    {Type::Hat,     "X3"   },
    {Type::Hat,     "X4"   },
    {Type::Button,  "B"    },
    {Type::Button,  "A"    },
    {Type::Control, "Start"}
  };

  return {};
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
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  if(name == "Rotate Display") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  if(name == "Rotate Display") return settings.rotateLeft;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    Emulator::video.setPalette();
    return true;
  }

  if(name == "Rotate Display" && value.is<bool>()) {
    settings.rotateLeft = value.get<bool>();
    Emulator::video.setEffect(Emulator::Video::Effect::RotateLeft, settings.rotateLeft);
    return true;
  }

  return false;
}

}
