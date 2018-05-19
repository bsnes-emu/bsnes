#include "../bsnes.hpp"
unique_pointer<InputManager> inputManager;

auto InputMapping::poll() -> int16 {
  return 0;
}

//

InputManager::InputManager() {
  inputManager = this;
  frequency = 5;

  for(auto& port : emulator->ports) {
    InputPort inputPort{port.id, port.name};
    for(auto& device : port.devices) {
      InputDevice inputDevice{device.id, device.name};
      for(auto& input : device.inputs) {
        InputMapping inputMapping;
        inputMapping.name = input.name;
        inputMapping.type = input.type;
        inputMapping.path = string{inputPort.name, "/", inputDevice.name, "/", inputMapping.name}.replace(" ", "");
        inputDevice.mappings.append(inputMapping);
      }
      inputPort.devices.append(inputDevice);
    }
    ports.append(inputPort);
  }

  input->onChange({&InputManager::onChange, this});
}

auto InputManager::poll() -> void {
  //polling actual hardware devices is time-consuming; skip if poll was called too recently
  auto thisPoll = chrono::millisecond();
  if(thisPoll - lastPoll < frequency) return;
  lastPoll = thisPoll;

  devices = input->poll();
}

auto InputManager::onChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
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
