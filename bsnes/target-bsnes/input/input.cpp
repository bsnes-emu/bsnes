#include "../bsnes.hpp"
#include "hotkeys.cpp"
InputManager inputManager;

auto InputMapping::bind() -> void {
  for(auto& binding : bindings) binding = {};

  for(uint index : range(BindingLimit)) {
    auto& assignment = assignments[index];
    auto& binding = bindings[index];

    auto token = assignment.split("/");
    if(token.size() < 3) continue;  //skip invalid mappings

    uint64 id = token[0].natural();
    uint group = token[1].natural();
    uint input = token[2].natural();
    string qualifier = token(3, "None");

    for(auto& device : inputManager.devices) {
      if(id != device->id()) continue;

      binding.device = device;
      binding.group = group;
      binding.input = input;
      binding.qualifier = Qualifier::None;
      if(qualifier == "Lo") binding.qualifier = Qualifier::Lo;
      if(qualifier == "Hi") binding.qualifier = Qualifier::Hi;
      if(qualifier == "Rumble") binding.qualifier = Qualifier::Rumble;
      break;
    }
  }

  string text;
  for(auto& assignment : assignments) text.append(assignment, ";");
  text.trimRight(";");
  settings[path].setValue(text);
}

auto InputMapping::bind(string mapping, uint binding) -> void {
  if(binding >= BindingLimit) return;
  assignments[binding] = mapping;
  bind();
}

auto InputMapping::bind(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, uint binding) -> bool {
  if(device->isNull() || (device->isKeyboard() && device->group(group).input(input).name() == "Escape")) {
    return unbind(binding), true;
  }

  string encoding = {"0x", hex(device->id()), "/", group, "/", input};

  if(isDigital()) {
    if((device->isKeyboard() && group == HID::Keyboard::GroupID::Button)
    || (device->isMouse() && group == HID::Mouse::GroupID::Button)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Button)) {
      if(newValue) {
        return bind(encoding, binding), true;
      }
    }

    if((device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Trigger)) {
      if(newValue < -16384 && group != HID::Joypad::GroupID::Trigger) {  //triggers are always hi
        return bind({encoding, "/Lo"}, binding), true;
      }

      if(newValue > +16384) {
        return bind({encoding, "/Hi"}, binding), true;
      }
    }
  }

  if(isAnalog()) {
    if((device->isMouse() && group == HID::Mouse::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Axis)
    || (device->isJoypad() && group == HID::Joypad::GroupID::Hat)) {
      if(newValue < -16384 || newValue > +16384) {
        return bind(encoding, binding), true;
      }
    }
  }

  if(isRumble()) {
    if(device->isJoypad() && group == HID::Joypad::GroupID::Button) {
      if(newValue) {
        return bind({encoding, "/Rumble"}, binding), true;
      }
    }
  }

  return false;
}

auto InputMapping::unbind(uint binding) -> void {
  bindings[binding] = {};
  assignments[binding] = {};
  string text;
  for(auto& assignment : assignments) text.append(assignment, ";");
  text.trimRight(";");
  settings[path].setValue(text);
}

auto InputMapping::poll() -> int16 {
  if(turboID) {
    auto& mapping = inputManager.ports[portID].devices[deviceID].mappings[turboID()];
    auto result = mapping.poll();
    if(result) return inputManager.turboCounter >= inputManager.turboFrequency;
  }

  uint validBindings = 0;
  int16 result = 0;

  for(auto& binding : bindings) {
    if(!binding.device) continue;  //unbound
    validBindings++;

    auto& device = binding.device;
    auto& group = binding.group;
    auto& input = binding.input;
    auto& qualifier = binding.qualifier;
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

  if(isDigital() && logic() == Logic::AND && validBindings > 0) return 1;
  return result;
}

auto InputMapping::rumble(bool enable) -> void {
  for(auto& binding : bindings) {
    if(!binding.device) continue;
    input.rumble(binding.device->id(), enable);
  }
}

//

auto InputMapping::Binding::icon() -> image {
  if(device && device->isKeyboard()) return Icon::Device::Keyboard;
  if(device && device->isMouse()) return Icon::Device::Mouse;
  if(device && device->isJoypad()) return Icon::Device::Joypad;
  return {};
}

auto InputMapping::Binding::name() -> string {
  if(device && device->isKeyboard()) {
    return device->group(group).input(input).name();
  }
  if(device && device->isMouse()) {
    return device->group(group).input(input).name();
  }
  if(device && device->isJoypad()) {
    string name{Hash::CRC16(string{device->id()}).digest().upcase()};
    name.append(" ", device->group(group).name());
    name.append(" ", device->group(group).input(input).name());
    if(qualifier == Qualifier::Lo) name.append(" Lo");
    if(qualifier == Qualifier::Hi) name.append(" Hi");
    if(qualifier == Qualifier::Rumble) name.append(" Rumble");
    return name;
  }
  return {};
}

//

auto InputManager::initialize() -> void {
  devices.reset();
  ports.reset();
  hotkeys.reset();

  input.onChange({&InputManager::onChange, this});

  lastPoll = 0;  //force a poll event immediately
  frequency = max(1u, settings.input.frequency);

  turboCounter = 0;
  turboFrequency = max(1, settings.input.turbo.frequency);

  auto information = emulator->information();
  auto ports = emulator->ports();
  for(uint portID : range(ports.size())) {
    auto& port = ports[portID];
    InputPort inputPort{port.id, port.name};
    auto devices = emulator->devices(port.id);
    for(uint deviceID : range(devices.size())) {
      auto& device = devices[deviceID];
      InputDevice inputDevice{device.id, device.name};
      auto inputs = emulator->inputs(device.id);
      for(uint inputID : range(inputs.size())) {
        auto& input = inputs[inputID];
        InputMapping inputMapping;
        inputMapping.portID = portID;
        inputMapping.deviceID = deviceID;
        inputMapping.inputID = inputID;
        inputMapping.name = input.name;
        inputMapping.type = input.type;
        inputMapping.path = string{information.name, "/", inputPort.name, "/", inputDevice.name, "/", inputMapping.name}.replace(" ", "");
        auto assignments = settings(inputMapping.path).text().split(";");
        for(uint index : range(BindingLimit)) inputMapping.assignments[index] = assignments(index);
        inputDevice.mappings.append(inputMapping);
      }
      for(uint inputID : range(inputs.size())) {
        auto& input = inputs[inputID];
        if(input.type != InputMapping::Type::Button && input.type != InputMapping::Type::Trigger) continue;
        uint turboID = inputDevice.mappings.size();
        InputMapping inputMapping;
        inputMapping.portID = portID;
        inputMapping.deviceID = deviceID;
        inputMapping.inputID = turboID;
        inputMapping.name = string{"Turbo ", input.name};
        inputMapping.type = input.type;
        inputMapping.path = string{information.name, "/", inputPort.name, "/", inputDevice.name, "/", inputMapping.name}.replace(" ", "");
        auto assignments = settings(inputMapping.path).text().split(";");
        for(uint index : range(BindingLimit)) inputMapping.assignments[index] = assignments(index);
        inputDevice.mappings.append(inputMapping);
        inputDevice.mappings[inputID].turboID = turboID;
      }
      inputPort.devices.append(inputDevice);
    }
    this->ports.append(inputPort);
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
  if(Application::modal()) return;

  //polling actual hardware devices is time-consuming; skip if poll was called too recently
  auto thisPoll = chrono::millisecond();
  if(thisPoll - lastPoll < frequency) return;
  lastPoll = thisPoll;

  auto devices = input.poll();
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

auto InputManager::frame() -> void {
  if(++turboCounter >= turboFrequency * 2) turboCounter = 0;
}

auto InputManager::onChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(settingsWindow.focused()) {
    inputSettings.inputEvent(device, group, input, oldValue, newValue);
    hotkeySettings.inputEvent(device, group, input, oldValue, newValue);
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
