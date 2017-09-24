#include <pce/pce.hpp>

namespace PCEngine {

Model model;
Settings settings;
#include "pc-engine.cpp"
#include "supergrafx.cpp"

Interface::Interface() {
  information.overscan = true;

  Port controllerPort{ID::Port::Controller, "Controller Port"};

  { Device device{ID::Device::None, "None"};
    controllerPort.devices.append(device);
  }

  { Device device{ID::Device::Gamepad, "Gamepad"};
    device.inputs.append({0, "Up"});
    device.inputs.append({0, "Down"});
    device.inputs.append({0, "Left"});
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "II"});
    device.inputs.append({0, "I"});
    device.inputs.append({0, "Select"});
    device.inputs.append({0, "Run"});
    controllerPort.devices.append(device);
  }

  ports.append(move(controllerPort));
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoInformation() -> VideoInformation {
  VideoInformation vi;
  vi.width  = 280;
  vi.height = 240;
  vi.internalWidth  = 1120;
  vi.internalHeight =  240;
  vi.aspectCorrection = 8.0 / 7.0;
  vi.refreshRate = (system.colorburst() * 6.0) / (262.0 * 1365.0);
  return vi;
}

auto Interface::videoColors() -> uint32 {
  return 1 << 9;
}

auto Interface::videoColor(uint32 color) -> uint64 {
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

auto Interface::sha256() -> string {
  return cartridge.sha256();
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
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

auto Interface::cheatSet(const string_vector& list) -> void {
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
