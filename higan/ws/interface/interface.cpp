#include <ws/ws.hpp>

namespace WonderSwan {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;

  information.manufacturer = "Bandai";
  information.name         = "WonderSwan";
  information.width        = 224;  //note: technically 224x144; but screen can be rotated
  information.height       = 224;  //by using a square size; this can be done in the core
  information.overscan     = false;
  information.aspectRatio  = 1.0;
  information.resettable   = false;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::WonderSwan, "WonderSwan", "ws", true});
  media.append({ID::WonderSwanColor, "WonderSwan Color", "wsc", true});

  { Device device{0, ID::DeviceHorizontal, "Controller"};
    device.inputs.append({ 0, 0, "Y1"});
    device.inputs.append({ 1, 0, "Y2"});
    device.inputs.append({ 2, 0, "Y3"});
    device.inputs.append({ 3, 0, "Y4"});
    device.inputs.append({ 4, 0, "X1"});
    device.inputs.append({ 5, 0, "X2"});
    device.inputs.append({ 6, 0, "X3"});
    device.inputs.append({ 7, 0, "X4"});
    device.inputs.append({ 8, 0, "B"});
    device.inputs.append({ 9, 0, "A"});
    device.inputs.append({10, 0, "Start"});
    device.inputs.append({11, 0, "Rotate"});
    devices.append(device);
  }

  { Device device{1, ID::DeviceVertical, "Controller"};
    device.inputs.append({ 0, 0, "Y1"});
    device.inputs.append({ 1, 0, "Y2"});
    device.inputs.append({ 2, 0, "Y3"});
    device.inputs.append({ 3, 0, "Y4"});
    device.inputs.append({ 4, 0, "X1"});
    device.inputs.append({ 5, 0, "X2"});
    device.inputs.append({ 6, 0, "X3"});
    device.inputs.append({ 7, 0, "X4"});
    device.inputs.append({ 8, 0, "B"});
    device.inputs.append({ 9, 0, "A"});
    device.inputs.append({10, 0, "Start"});
    device.inputs.append({11, 0, "Rotate"});
    devices.append(device);
  }

  ports.append({0, "Horizontal Orientation", {devices[0]}});
  ports.append({1, "Vertical Orientation", {devices[1]}});
}

auto Interface::manifest() -> string {
  return cartridge.information.manifest;
}

auto Interface::title() -> string {
  return cartridge.information.title;
}

auto Interface::videoFrequency() -> double {
  return 3072000.0 / (159.0 * 256.0);  //~75.47hz
}

auto Interface::videoColors() -> uint32 {
  return 1 << 12;
}

auto Interface::videoColor(uint32 color) -> uint64 {
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

auto Interface::audioFrequency() -> double {
  return 3072000.0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::sha256() -> string {
  return cartridge.information.sha256;
}

auto Interface::load(uint id) -> void {
  if(id == ID::WonderSwan) system.load(Model::WonderSwan);
  if(id == ID::WonderSwanColor) system.load(Model::WonderSwanColor);
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
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

auto Interface::cheatSet(const lstring& list) -> void {
  cheat.reset();
  for(auto& codeset : list) {
    lstring codes = codeset.split("+");
    for(auto& code : codes) {
      lstring part = code.split("/");
      if(part.size() == 2) cheat.append(hex(part[0]), hex(part[1]));
      if(part.size() == 3) cheat.append(hex(part[0]), hex(part[1]), hex(part[2]));
    }
  }
}

auto Interface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    system.configureVideoEffects();
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    system.configureVideoPalette();
    return true;
  }

  return false;
}

}
