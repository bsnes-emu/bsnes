#include <gba/gba.hpp>

namespace GameBoyAdvance {

#define returns(T) T { return ([&] { struct With : T { With() {
#define $ }}; return With(); })(); }

Settings settings;

auto Interface::information() -> returns(Information) {
  manufacturer = "Nintendo";
  name         = "Game Boy Advance";
}$

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::display() -> returns(Display) {
  type   = Display::Type::LCD;
  colors = 1 << 15;
  width  = 240;
  height = 160;
  internalWidth  = 240;
  internalHeight = 160;
  aspectCorrection = 1.0;
  refreshRate = system.frequency() / (228.0 * 1232.0);
  if(settings.rotateLeft) {
    swap(width, height);
    swap(internalWidth, internalHeight);
  }
}$

auto Interface::color(uint32 color) -> uint64 {
  uint R = color.bits( 0, 4);
  uint G = color.bits( 5, 9);
  uint B = color.bits(10,14);

  uint64 r = image::normalize(R, 5, 16);
  uint64 g = image::normalize(G, 5, 16);
  uint64 b = image::normalize(B, 5, 16);

  if(settings.colorEmulation) {
    double lcdGamma = 4.0, outGamma = 2.2;
    double lb = pow(B / 31.0, lcdGamma);
    double lg = pow(G / 31.0, lcdGamma);
    double lr = pow(R / 31.0, lcdGamma);
    r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
  }

  return r << 32 | g << 16 | b << 0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::load() -> bool {
  return system.load(this);
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
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
    {Type::Button,  "B"     },
    {Type::Button,  "A"     },
    {Type::Trigger, "L"     },
    {Type::Trigger, "R"     },
    {Type::Control, "Select"},
    {Type::Control, "Start" },
    {Type::Rumble,  "Rumble"}
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

#undef returns
#undef $

}
