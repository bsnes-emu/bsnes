#include "../tomoko.hpp"
#include "hotkeys.cpp"
unique_pointer<InputManager> inputManager;

auto InputMapping::bind() -> void {
  auto token = assignment.split("/");
  if(token.size() < 3) return unbind();
  uint64 id = token[0].natural();
  uint group = token[1].natural();
  uint input = token[2].natural();
  string qualifier = token(3, "None");

  for(auto& device : inputManager->devices) {
    if(id != device->id()) continue;

    this->device = device;
    this->group = group;
    this->input = input;
    this->qualifier = Qualifier::None;
    if(qualifier == "Lo") this->qualifier = Qualifier::Lo;
    if(qualifier == "Hi") this->qualifier = Qualifier::Hi;
    if(qualifier == "Rumble") this->qualifier = Qualifier::Rumble;
    break;
  }

  settings[path].setValue(assignment);
}

auto InputMapping::bind(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> bool {
  if(device->isNull() || (device->isKeyboard() && device->group(group).input(input).name() == "Escape")) {
    return unbind(), true;
  }

  string encoding = {"0x", hex(device->id()), "/", group, "/", input};

  if(isDigital()) {
    if((device->isKeyboard() && group == HID::Keyboard::GroupID::Button)
    || (device->isMouse() && group == HID::Mouse::GroupID::Button)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Button)) {
      if(newValue) {
        this->assignment = encoding;
        return bind(), true;
      }
    }

    if((device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Trigger)) {
      if(newValue < -16384 && group != HID::Joypad::GroupID::Trigger) {  //triggers are always hi
        this->assignment = {encoding, "/Lo"};
        return bind(), true;
      }

      if(newValue > +16384) {
        this->assignment = {encoding, "/Hi"};
        return bind(), true;
      }
    }
  }

  if(isAnalog()) {
    if((device->isMouse() && group == HID::Mouse::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)) {
      if(newValue < -16384 || newValue > +16384) {
        this->assignment = encoding;
        return bind(), true;
      }
    }
  }

  if(isRumble()) {
    if(device->isJoypad() && group == HID::Joypad::GroupID::Button) {
      if(newValue) {
        this->assignment = {encoding, "/Rumble"};
        return bind(), true;
      }
    }
  }

  return false;
}

auto InputMapping::poll() -> int16 {
  if(!device) return 0;
  auto value = device->group(group).input(input).value();

  if(isDigital()) {
    if(device->isKeyboard() && group == HID::Keyboard::GroupID::Button) return value != 0;
    if(device->isMouse() && group == HID::Mouse::GroupID::Button) return value != 0;
    if(device->isJoypad() && group == HID::Joypad::GroupID::Button) return value != 0;
    if((device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Trigger)) {
      if(qualifier == Qualifier::Lo) return value < -16384;
      if(qualifier == Qualifier::Hi) return value > +16384;
    }
  }

  if(isAnalog()) {
    if(device->isMouse() && group == HID::Mouse::GroupID::Axis) return value;
    if(device->isJoypad() && group == HID::Joypad::GroupID::Axis) return value >> 8;
    if(device->isJoypad() && group == HID::Joypad::GroupID::Hat) return value < 0 ? -1 : value > 0 ? +1 : 0;
  }

  return 0;
}

auto InputMapping::rumble(bool enable) -> void {
  if(!device) return;
  ::input->rumble(device->id(), enable);
}

auto InputMapping::unbind() -> void {
  assignment = "None";
  device = nullptr;
  group = 0;
  input = 0;
  qualifier = Qualifier::None;
  settings[path].setValue(assignment);
}

auto InputMapping::assignmentName() -> string {
  if(!device) return "None";
  string path;
  path.append(device->name());
  if(device->name() != "Keyboard") {
    //keyboards only have one group; no need to append group name
    path.append(".", device->group(group).name());
  }
  path.append(".", device->group(group).input(input).name());
  if(qualifier == Qualifier::Lo) path.append(".Lo");
  if(qualifier == Qualifier::Hi) path.append(".Hi");
  if(qualifier == Qualifier::Rumble) path.append(".Rumble");
  return path;
}

auto InputMapping::deviceName() -> string {
  if(!device) return "";
  return hex(device->id());
}

//

InputManager::InputManager() {
  inputManager = this;
  latency = max(1u, settings["Input/Latency"].natural());

  for(auto& emulator : program->emulators) {
    auto& inputEmulator = emulators(emulators.size());
    inputEmulator.interface = emulator;
    inputEmulator.name = emulator->information.name;

    for(auto& port : emulator->ports) {
      auto& inputPort = inputEmulator.ports(port.id);
      inputPort.name = port.name;
      for(auto& device : port.devices) {
        auto& inputDevice = inputPort.devices(device.id);
        inputDevice.name = device.name;
        for(auto& input : device.inputs) {
          auto& inputMapping = inputDevice.mappings(inputDevice.mappings.size());
          inputMapping.name = input.name;
          inputMapping.type = input.type;

          inputMapping.path = string{inputEmulator.name, "/", inputPort.name, "/", inputDevice.name, "/", inputMapping.name}.replace(" ", "");
          inputMapping.assignment = settings(inputMapping.path).text();
          inputMapping.bind();
        }
      }
    }
  }

  appendHotkeys();
}

//Emulator::Interface::inputPoll() needs to call into InputManager::InputEmulator
//this function is calling during Program::loadMedium() to link the two together
auto InputManager::bind(Emulator::Interface* interface) -> void {
  this->emulator = nullptr;
  for(auto& emulator : emulators) {
    if(emulator.interface == interface) {
      this->emulator = &emulator;
    }
  }
  assert(this->emulator != nullptr);
}

auto InputManager::bind() -> void {
  for(auto& emulator : emulators) {
    for(auto& port : emulator.ports) {
      for(auto& device : port.devices) {
        for(auto& mapping : device.mappings) {
          mapping.bind();
        }
      }
    }
  }

  for(auto& hotkey : hotkeys) {
    hotkey->bind();
  }
}

auto InputManager::poll() -> void {
  //polling actual hardware is very time-consuming: skip call if poll was called too recently
  auto thisPoll = chrono::millisecond();
  if(thisPoll - lastPoll < latency) return;
  lastPoll = thisPoll;

  auto devices = input->poll();
  bool changed = devices.size() != this->devices.size();
  if(!changed) {
    for(auto n : range(devices)) {
      changed = devices[n] != this->devices[n];
      if(changed) break;
    }
  }
  if(changed) {
    this->devices = devices;
    bind();
  }
}

auto InputManager::onChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(settingsManager->focused()) {
    settingsManager->input.inputEvent(device, group, input, oldValue, newValue);
    settingsManager->hotkeys.inputEvent(device, group, input, oldValue, newValue);
  }
}

auto InputManager::quit() -> void {
  emulators.reset();
  hotkeys.reset();
}

auto InputManager::findMouse() -> shared_pointer<HID::Device> {
  for(auto& device : devices) {
    if(device->isMouse()) return device;
  }
  return {};
}
