#include "../higan.hpp"
#include "hotkeys.cpp"
unique_pointer<InputManager> inputManager;

//build mappings list from assignment string
auto InputMapping::bind() -> void {
  mappings.reset();

  for(auto& item : assignment.split(logic() == Logic::AND ? "&" : "|")) {
    auto token = item.split("/");
    if(token.size() < 3) continue;  //skip invalid mappings

    uint64 id = token[0].natural();
    uint group = token[1].natural();
    uint input = token[2].natural();
    string qualifier = token(3, "None");

    Mapping mapping;
    for(auto& device : inputManager->devices) {
      if(id != device->id()) continue;

      mapping.device = device;
      mapping.group = group;
      mapping.input = input;
      mapping.qualifier = Qualifier::None;
      if(qualifier == "Lo") mapping.qualifier = Qualifier::Lo;
      if(qualifier == "Hi") mapping.qualifier = Qualifier::Hi;
      if(qualifier == "Rumble") mapping.qualifier = Qualifier::Rumble;
      break;
    }

    if(!mapping.device) continue;
    mappings.append(mapping);
  }

  settings[path].setValue(assignment);
}

//append new mapping to mappings list
auto InputMapping::bind(string mapping) -> void {
  auto list = assignment.split(logic() == Logic::AND ? "&" : "|");
  if(list.find(mapping)) return;  //already in the mappings list
  if(!assignment || assignment == "None") {
    //create new mapping
    assignment = mapping;
  } else {
    //add additional mapping
    assignment.append(logic() == Logic::AND ? "&" : "|");
    assignment.append(mapping);
  }
  bind();
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
        return bind(encoding), true;
      }
    }

    if((device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Trigger)) {
      if(newValue < -16384 && group != HID::Joypad::GroupID::Trigger) {  //triggers are always hi
        return bind({encoding, "/Lo"}), true;
      }

      if(newValue > +16384) {
        return bind({encoding, "/Hi"}), true;
      }
    }
  }

  if(isAnalog()) {
    if((device->isMouse() && group == HID::Mouse::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)) {
      if(newValue < -16384 || newValue > +16384) {
        return bind(encoding), true;
      }
    }
  }

  if(isRumble()) {
    if(device->isJoypad() && group == HID::Joypad::GroupID::Button) {
      if(newValue) {
        return bind({encoding, "/Rumble"}), true;
      }
    }
  }

  return false;
}

auto InputMapping::unbind() -> void {
  mappings.reset();
  settings[path].setValue(assignment = "None");
}

auto InputMapping::poll() -> int16 {
  if(!mappings) return 0;

  if(isDigital()) {
    bool result = logic() == Logic::AND ? 1 : 0;

    for(auto& mapping : mappings) {
      auto value = mapping.device->group(mapping.group).input(mapping.input).value();
      bool output = logic() == Logic::AND ? 0 : 1;

      if(mapping.device->isKeyboard() && mapping.group == HID::Keyboard::GroupID::Button) output = value != 0;
      if(mapping.device->isMouse() && mapping.group == HID::Mouse::GroupID::Button) output = value != 0;
      if(mapping.device->isJoypad() && mapping.group == HID::Joypad::GroupID::Button) output = value != 0;
      if((mapping.device->isJoypad() && mapping.group == HID::Joypad::GroupID::Axis)
      || (mapping.device->isJoypad() && mapping.group == HID::Joypad::GroupID::Hat)
      || (mapping.device->isJoypad() && mapping.group == HID::Joypad::GroupID::Trigger)) {
        if(mapping.qualifier == Qualifier::Lo) output = value < -16384;
        if(mapping.qualifier == Qualifier::Hi) output = value > +16384;
      }

      if(logic() == Logic::AND) result &= output;
      if(logic() == Logic::OR ) result |= output;
    }

    return result;
  }

  if(isAnalog()) {
    int16 result = 0;

    for(auto& mapping : mappings) {
      auto value = mapping.device->group(mapping.group).input(mapping.input).value();

      //logic does not apply to analog inputs ... always combinatorial
      if(mapping.device->isMouse() && mapping.group == HID::Mouse::GroupID::Axis) result += value;
      if(mapping.device->isJoypad() && mapping.group == HID::Joypad::GroupID::Axis) result += value >> 8;
      if(mapping.device->isJoypad() && mapping.group == HID::Joypad::GroupID::Hat) result += value < 0 ? -1 : value > 0 ? +1 : 0;
    }

    return result;
  }

  return 0;
}

auto InputMapping::rumble(bool enable) -> void {
  for(auto& mapping : mappings) {
    ::input->rumble(mapping.device->id(), enable);
  }
}

//create a human-readable string from mappings list for display in the user interface
auto InputMapping::displayName() -> string {
  if(!mappings) return "None";

  string path;
  for(auto& mapping : mappings) {
    path.append(mapping.device->name());
    if(mapping.device->name() != "Keyboard" && mapping.device->name() != "Mouse") {
      //show device IDs to distinguish between multiple joypads
      path.append("(", hex(mapping.device->id()), ")");
    }
    if(mapping.device->name() != "Keyboard") {
      //keyboards only have one group; no need to append group name
      path.append(".", mapping.device->group(mapping.group).name());
    }
    path.append(".", mapping.device->group(mapping.group).input(mapping.input).name());
    if(mapping.qualifier == Qualifier::Lo) path.append(".Lo");
    if(mapping.qualifier == Qualifier::Hi) path.append(".Hi");
    if(mapping.qualifier == Qualifier::Rumble) path.append(".Rumble");
    path.append(logic() == Logic::AND ? " and " : " or ");
  }

  return path.trimRight(logic() == Logic::AND ? " and " : " or ", 1L);
}

//

InputManager::InputManager() {
  inputManager = this;
  input->onChange({&InputManager::onChange, this});
  frequency = max(1u, settings["Input/Frequency"].natural());

  for(auto& emulator : program->emulators) {
    InputEmulator inputEmulator;
    inputEmulator.interface = emulator;
    inputEmulator.name = emulator->information.name;
    for(auto& port : emulator->ports) {
      InputPort inputPort{port.id, port.name};
      for(auto& device : port.devices) {
        InputDevice inputDevice{device.id, device.name};
        for(auto& input : device.inputs) {
          InputMapping inputMapping;
          inputMapping.name = input.name;
          inputMapping.type = input.type;

          inputMapping.path = string{inputEmulator.name, "/", inputPort.name, "/", inputDevice.name, "/", inputMapping.name}.replace(" ", "");
          inputMapping.assignment = settings(inputMapping.path).text();
          inputMapping.bind();

          inputDevice.mappings.append(inputMapping);
        }
        inputPort.devices.append(inputDevice);
      }
      inputEmulator.ports.append(move(inputPort));
    }
    emulators.append(move(inputEmulator));
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
  if(thisPoll - lastPoll < frequency) return;
  lastPoll = thisPoll;

  auto devices = input->poll();
  bool changed = devices.size() != this->devices.size();
  if(!changed) {
    for(auto n : range(devices.size())) {
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

auto InputManager::mapping(uint port, uint device, uint input) -> maybe<InputMapping&> {
  if(!emulator) return nothing;
  for(auto& inputPort : emulator->ports) {
    if(inputPort.id != port) continue;
    for(auto& inputDevice : inputPort.devices) {
      if(inputDevice.id != device) continue;
      if(input >= inputDevice.mappings.size()) return nothing;
      return inputDevice.mappings[input];
    }
  }
  return nothing;
}

auto InputManager::findMouse() -> shared_pointer<HID::Device> {
  for(auto& device : devices) {
    if(device->isMouse()) return device;
  }
  return {};
}
