#include <md/md.hpp>

namespace MegaDrive {

Settings settings;

Interface::Interface() {
  information.manufacturer = "Sega";
  information.name         = "Mega Drive";
  information.overscan     = true;

  regions.append({"Autodetect"});
  regions.append({"NTSC-J"});
  regions.append({"NTSC-U"});
  regions.append({"PAL"});

  media.append({ID::MegaDrive, "Mega Drive", "md"});

  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};
  Port extensionPort{ID::Port::Extension, "Extension Port"};

  { Device device{ID::Device::None, "None"};
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
    extensionPort.devices.append(device);
  }

  { Device device{ID::Device::Gamepad, "Gamepad"};
    device.inputs.append({0, "Up"   });
    device.inputs.append({0, "Down" });
    device.inputs.append({0, "Left" });
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "A"    });
    device.inputs.append({0, "B"    });
    device.inputs.append({0, "C"    });
    device.inputs.append({0, "Start"});
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
  }

  ports.append(move(controllerPort1));
  ports.append(move(controllerPort2));
  ports.append(move(extensionPort));
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoResolution() -> VideoSize {
  return {1280, 480};
}

auto Interface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 320;
  uint h = 240;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto Interface::videoColors() -> uint32 {
  return 1 << 9;
}

auto Interface::videoColor(uint32 color) -> uint64 {
  uint R = color.bits(0,2);
  uint G = color.bits(3,5);
  uint B = color.bits(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::load(uint id) -> bool {
  return system.load(this);
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(settings.controllerPort1 = device);
  if(port == ID::Port::Controller2) controllerPort2.connect(settings.controllerPort2 = device);
  if(port == ID::Port::Extension) extensionPort.connect(settings.extensionPort = device);
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
