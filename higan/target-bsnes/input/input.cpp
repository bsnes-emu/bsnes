#include "../bsnes.hpp"
#include "hotkeys.cpp"
unique_pointer<InputManager> inputManager;

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

auto InputMapping::bind(string mapping) -> void {
  if(assignment.split(logic() == Logic::AND ? "&" : "|").find(mapping)) return;  //ignore if already in mappings list
  if(!assignment || assignment == "None") {  //create new mapping
    assignment = mapping;
  } else {  //add additional mapping
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
  int16 result;

  for(auto& mapping : mappings) {
    auto& device = mapping.device;
    auto& group = mapping.group;
    auto& input = mapping.input;
    auto& qualifier = mapping.qualifier;
    auto value = device->group(group).input(input).value();

    if(isDigital()) {
      boolean output;

      if((device->isKeyboard() && group == HID::Keyboard::GroupID::Button)
      || (device->isMouse() && group == HID::Mouse::GroupID::Button)
      || (device->isJoypad() && group == HID::Joypad::GroupID::Button)) {
        output = value != 0;
      }
      if((device->isJoypad() && group == HID::Joypad::GroupID::Axis)
      || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)
      || (device->isJoypad() && group == HID::Joypad::GroupID::Trigger)) {
        if(qualifier == Qualifier::Lo) output = value < -16384;
        if(qualifier == Qualifier::Hi) output = value > +16384;
      }

      if(logic() == Logic::AND && output == 0) return 0;
      if(logic() == Logic::OR  && output == 1) return 1;
    }

    if(isAnalog()) {
      //logic does not apply to analog inputs ... always combinatorial
      if(device->isMouse() && group == HID::Mouse::GroupID::Axis) result += value;
      if(device->isJoypad() && group == HID::Joypad::GroupID::Axis) result += value >> 8;
      if(device->isJoypad() && group == HID::Joypad::GroupID::Hat) result += value < 0 ? -1 : value > 0 ? +1 : 0;
    }
  }

  if(isDigital() && logic() == Logic::AND) return 1;
  return result;
}

auto InputMapping::rumble(bool enable) -> void {
  for(auto& mapping : mappings) {
    ::input->rumble(mapping.device->id(), enable);
  }
}

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
}

auto InputManager::initialize() -> void {
  devices.reset();
  ports.reset();
  hotkeys.reset();

  if(!input) return;
  input->onChange({&InputManager::onChange, this});
  frequency = max(1u, settings["Input/Frequency"].natural());

  for(auto& port : emulator->ports()) {
    InputPort inputPort{port.id, port.name};
    for(auto& device : emulator->devices(port.id)) {
      InputDevice inputDevice{device.id, device.name};
      for(auto& input : emulator->inputs(device.id)) {
        InputMapping inputMapping;
        inputMapping.name = input.name;
        inputMapping.type = input.type;
        inputMapping.path = string{"Emulator/", inputPort.name, "/", inputDevice.name, "/", inputMapping.name}.replace(" ", "");
        inputMapping.assignment = settings(inputMapping.path).text();
        inputDevice.mappings.append(inputMapping);
      }
      inputPort.devices.append(inputDevice);
    }
    ports.append(inputPort);
  }

  bindHotkeys();
  poll();
}

auto InputManager::bind() -> void {
  for(auto& port : ports) {
    for(auto& device : port.devices) {
      for(auto& mapping : device.mappings) {
        mapping.bind();
      }
    }
  }

  for(auto& hotkey : hotkeys) {
    hotkey.bind();
  }
}

auto InputManager::poll() -> void {
  //polling actual hardware devices is time-consuming; skip if poll was called too recently
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
  if(settingsWindow->focused()) {
    settingsWindow->input.inputEvent(device, group, input, oldValue, newValue);
    settingsWindow->hotkeys.inputEvent(device, group, input, oldValue, newValue);
  }
}

auto InputManager::mapping(uint port, uint device, uint input) -> maybe<InputMapping&> {
  if(!emulator) return nothing;
  for(auto& inputPort : ports) {
    if(inputPort.id != port) continue;
    for(auto& inputDevice : inputPort.devices) {
      if(inputDevice.id != device) continue;
      if(input >= inputDevice.mappings.size()) continue;
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
