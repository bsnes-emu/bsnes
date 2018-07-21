#include <md/md.hpp>

namespace MegaDrive {

Settings settings;

Interface::Interface() {
  information.manufacturer = "Sega";
  information.name         = "Mega Drive";
  information.overscan     = true;
  information.resettable   = true;

  media.append({ID::MegaDrive, "Mega Drive", "md"});

  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};
  Port extensionPort{ID::Port::Extension, "Extension Port"};

  { Device device{ID::Device::None, "None"};
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
    extensionPort.devices.append(device);
  }

  { Device device{ID::Device::ControlPad, "Control Pad"};
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

  { Device device{ID::Device::FightingPad, "Fighting Pad"};
    device.inputs.append({0, "Up"   });
    device.inputs.append({0, "Down" });
    device.inputs.append({0, "Left" });
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "A"    });
    device.inputs.append({0, "B"    });
    device.inputs.append({0, "C"    });
    device.inputs.append({0, "X"    });
    device.inputs.append({0, "Y"    });
    device.inputs.append({0, "Z"    });
    device.inputs.append({0, "Mode" });
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

auto Interface::videoInformation() -> VideoInformation {
  VideoInformation vi;
  vi.width  = 320;
  vi.height = 240;
  vi.internalWidth  = 1280;
  vi.internalHeight =  480;
  vi.aspectCorrection = 1.0;
  vi.refreshRate = (system.frequency() / 2.0) / (vdp.frameHeight() * 1710.0);
  return vi;
}

auto Interface::videoColors() -> uint32 {
  return 3 * (1 << 9);
}

auto Interface::videoColor(uint32 color) -> uint64 {
  uint R = color.bits(0, 2);
  uint G = color.bits(3, 5);
  uint B = color.bits(6, 8);
  uint M = color.bits(9,10);

  uint lookup[3][8] = {
    {  0,  29,  52,  70,  87, 101, 116, 130},  //shadow
    {  0,  52,  87, 116, 144, 172, 206, 255},  //normal
    {130, 144, 158, 172, 187, 206, 228, 255},  //highlight
  };

  uint64 r = image::normalize(lookup[M][R], 8, 16);
  uint64 g = image::normalize(lookup[M][G], 8, 16);
  uint64 b = image::normalize(lookup[M][B], 8, 16);

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

auto Interface::connected(uint port) -> uint {
  if(port == ID::Port::Controller1) return settings.controllerPort1;
  if(port == ID::Port::Controller2) return settings.controllerPort2;
  if(port == ID::Port::Extension) return settings.extensionPort;
  return 0;
}

auto Interface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(settings.controllerPort1 = device);
  if(port == ID::Port::Controller2) controllerPort2.connect(settings.controllerPort2 = device);
  if(port == ID::Port::Extension) extensionPort.connect(settings.extensionPort = device);
}

auto Interface::power() -> void {
  system.power(/* reset = */ false);
}

auto Interface::reset() -> void {
  system.power(/* reset = */ true);
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
