#include <ws/ws.hpp>

namespace WonderSwan {

Settings settings;
#include "wonderswan.cpp"
#include "wonderswan-color.cpp"
#include "pocket-challenge-v2.cpp"

Interface::Interface() {
  Port hardwarePort{ID::Port::Hardware, "Hardware"};

  { Device device{ID::Device::Controls, "Controls"};
    device.inputs.append({0, "Y1"});
    device.inputs.append({0, "Y2"});
    device.inputs.append({0, "Y3"});
    device.inputs.append({0, "Y4"});
    device.inputs.append({0, "X1"});
    device.inputs.append({0, "X2"});
    device.inputs.append({0, "X3"});
    device.inputs.append({0, "X4"});
    device.inputs.append({0, "B"});
    device.inputs.append({0, "A"});
    device.inputs.append({0, "Start"});
    hardwarePort.devices.append(device);
  }

  ports.append(move(hardwarePort));
}

auto Interface::manifest() -> string {
  return cartridge.information.manifest;
}

auto Interface::title() -> string {
  return cartridge.information.title;
}

auto Interface::videoInformation() -> VideoInformation {
  VideoInformation vi;
  vi.width  = 224;
  vi.height = 144;
  vi.internalWidth  = 224;
  vi.internalHeight = 144;
  vi.aspectCorrection = 1.0;
  vi.refreshRate = 3'072'000.0 / (159.0 * 256.0);
  if(settings.rotateLeft) {
    swap(vi.width, vi.height);
    swap(vi.internalWidth, vi.internalHeight);
  }
  return vi;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::sha256() -> string {
  return cartridge.information.sha256;
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

auto Interface::cheatSet(const string_vector& list) -> void {
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
    system.configureVideoEffects();
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    system.configureVideoPalette();
    return true;
  }

  if(name == "Rotate Display" && value.is<bool>()) {
    settings.rotateLeft = value.get<bool>();
    system.configureVideoEffects();
    return true;
  }

  return false;
}

}
