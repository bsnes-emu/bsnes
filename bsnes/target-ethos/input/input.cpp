#include "../ethos.hpp"
InputManager *inputManager = nullptr;

void AbstractInput::bind() {
  if(mapping.empty()) type = Type::Button, mapping = "None";

       if(mapping.endswith(".Up")) type = Type::HatUp;
  else if(mapping.endswith(".Down")) type = Type::HatDown;
  else if(mapping.endswith(".Left")) type = Type::HatLeft;
  else if(mapping.endswith(".Right")) type = Type::HatRight;
  else if(mapping.endswith(".Lo")) type = Type::AxisLo;
  else if(mapping.endswith(".Hi")) type = Type::AxisHi;
  else if(mapping.beginswith("JP") && mapping.position("Axis")) type = Type::Axis;
  else if(mapping.beginswith("MS") && mapping.endswith("axis")) type = Type::MouseAxis;
  else if(mapping.beginswith("MS")) type = Type::MouseButton;
  else type = Type::Button;

  string decode = mapping;
  if(auto position = decode.position(".")) decode[position()] = 0;
  scancode = Scancode::decode(decode);
}

void InputManager::bind() {
  for(auto &input : inputMap) input.data.bind();
}

void InputManager::poll() {
  input.poll(table);
}

int16_t InputManager::poll(unsigned guid) {
  return table[inputMap[guid].scancode];
}

InputManager::InputManager() {
  bootstrap();
}

void InputManager::bootstrap() {
  unsigned guid = 0;
  for(auto &emulator : application->emulator) {
    for(auto &port : emulator->port) {
      for(auto &device : port.device) {
        for(auto &number : device.displayinput) {
          auto &input = device.input[number];

          AbstractInput abstract;
          abstract.type = AbstractInput::Type::Button;
          abstract.name = {emulator->information.name, "::", port.name, "::", device.name, "::", input.name};
          abstract.mapping = "None";
          abstract.scancode = 0;
          abstract.name.replace(" ", "");

          input.guid = guid++;
          inputMap(input.guid) = abstract;
        }
      }
    }
  }

  for(auto &input : inputMap) {
    config.append(input.data.mapping, input.data.name);
  }

  config.load(application->path("input.cfg"));
  config.save(application->path("input.cfg"));

  bind();
}
