#include "../tomoko.hpp"
InputManager* inputManager = nullptr;

auto InputMapping::bind() -> void {
  auto token = assignment.split("/");
  if(token.size() < 3) return;
  uint64_t id = token[0].hex();
  unsigned group = token[1].decimal();
  unsigned input = token[2].decimal();

  for(auto& device : inputManager->devices) {
    if(id != device->id) continue;

    this->device = device;
    this->group = group;
    this->input = input;
    break;
  }
}

auto InputMapping::bind(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> bool {
  this->assignment = {hex(device.id), "/", group, "/", input, "/", device.group[group].input[input].name};
  this->device = &device;
  this->group = group;
  this->input = input;
  return true;
}

auto InputMapping::poll() -> int16 {
  if(device) return device->group[group].input[input].value;
  return 0;
}

//

InputManager::InputManager() {
  inputManager = this;
  input.onChange = {&InputManager::onChange, this};

  for(auto& emulator : program->emulators) {
    Configuration::Node nodeEmulator;

    emulators.append(InputEmulator());
    auto& inputEmulator = emulators.last();
    inputEmulator.name = emulator->information.name;

    for(auto& port : emulator->port) {
      Configuration::Node nodePort;

      inputEmulator.ports.append(InputPort());
      auto& inputPort = inputEmulator.ports.last();
      inputPort.name = port.name;
      for(auto& device : port.device) {
        Configuration::Node nodeDevice;

        inputPort.devices.append(InputDevice());
        auto& inputDevice = inputPort.devices.last();
        inputDevice.name = device.name;
        for(auto number : device.order) {
          auto& input = device.input[number];
          inputDevice.mappings.append(new InputMapping());
          auto& inputMapping = inputDevice.mappings.last();
          inputMapping->name = input.name;
          inputMapping->link = &input;
          input.guid = (uintptr_t)inputMapping;

          nodeDevice.append(inputMapping->assignment, inputMapping->name);
        }

        nodePort.append(nodeDevice, string{inputDevice.name}.replace(" ", ""));
      }

      nodeEmulator.append(nodePort, string{inputPort.name}.replace(" ", ""));
    }

    config.append(nodeEmulator, string{inputEmulator.name}.replace(" ", ""));
  }

  config.load({configpath(), "tomoko/settings.bml"});
  config.save({configpath(), "tomoko/settings.bml"});
  poll();  //will call bind();
}

auto InputManager::bind() -> void {
  for(auto& emulator : emulators) {
    for(auto& port : emulator.ports) {
      for(auto& device : port.devices) {
        for(auto& mapping : device.mappings) {
          mapping->bind();
        }
      }
    }
  }
}

auto InputManager::poll() -> void {
  auto devices = input.poll();
  bool changed = devices.size() != this->devices.size();
  if(changed == false) {
    for(auto n : range(devices)) {
      changed = devices[n] != this->devices[n];
      if(changed) break;
    }
  }
  if(changed == true) {
    this->devices = devices;
    bind();
  }
}

auto InputManager::onChange(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> void {
  if(settingsManager->focused()) {
    settingsManager->input.inputEvent(device, group, input, oldValue, newValue);
  }
}

auto InputManager::quit() -> void {
  config.save({configpath(), "tomoko/settings.bml"});
}
