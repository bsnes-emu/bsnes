#include "../loki.hpp"
InputManager* inputManager = nullptr;

void AbstractInput::bind() {
  for(auto device : inputManager->devices) {
    if(device->isKeyboard() == false) continue;
    if(auto group = device->find("Button")) {
      if(auto input = device->group[group()].find(mapping)) {
        this->device = device;
        this->group = group();
        this->input = input();
        break;
      }
    }
  }
}

int16_t AbstractInput::poll() {
  if(device == nullptr) return 0;
  return device->group[group].input[input].value;
}

InputManager::InputManager() {
  inputManager = this;
}

void InputManager::load() {
  unsigned guid = 0;
  Configuration::Node emulatorNode;

  for(auto& port : emulator->port) {
    Configuration::Node portNode;

    for(auto& device : port.device) {
      Configuration::Node deviceNode;

      for(auto& number : device.order) {
        auto& input = device.input[number];
        input.guid = guid++;

        auto abstract = new AbstractInput;
        abstract->name = string{input.name}.replace(" ", "");
        abstract->mapping = "None";
        inputMap.append(abstract);

        deviceNode.append(abstract->mapping, abstract->name);
      }

      portNode.append(deviceNode, string{device.name}.replace(" ", ""));
    }

    emulatorNode.append(portNode, string{port.name}.replace(" ", ""));
  }

  append(emulatorNode, "SuperFamicom");

  Configuration::Document::load(program->path("input.bml"));
  Configuration::Document::save(program->path("input.bml"));
}

void InputManager::unload() {
  Configuration::Document::save(program->path("input.bml"));
}

void InputManager::bind() {
  for(auto input : inputMap) input->bind();
}

void InputManager::poll() {
  auto devices = input.poll();
  bool changed = devices.size() != this->devices.size();
  if(changed == false) {
    for(unsigned n = 0; n < devices.size(); n++) {
      changed = devices[n] != this->devices[n];
      if(changed) break;
    }
  }
  if(changed == true) {
    this->devices = devices;
    bind();
  }
}
