#include "../higan.hpp"
#include "hotkeys.cpp"
InputManager* inputManager = nullptr;
HID::Null hidNull;

void AbstractInput::bind() {
  inputList.reset();
  lstring list = mapping.split(",");

  for(auto& mapping : list) {
    lstring values = mapping.split("/");
    if(values.size() == 1) continue;  //skip "None" mapping

    uint64_t id = hex(values[0]);
    string group = values(1, "");
    string input = values(2, "");
    string qualifier = values(3, "");

    Input item;
    for(auto device : inputManager->devices) {
      if(id != device->id) continue;
      if(group == "Rumble") {
        item.device = device;
        item.id = id;
        item.group = 0;
        item.input = 0;
        break;
      }
      if(auto groupID = device->find(group)) {
        if(auto inputID = device->group[groupID()].find(input)) {
          item.device = device;
          item.id = id;
          item.group = groupID();
          item.input = inputID();
          item.qualifier = Input::Qualifier::None;
          if(qualifier == "Lo") item.qualifier = Input::Qualifier::Lo;
          if(qualifier == "Hi") item.qualifier = Input::Qualifier::Hi;
          break;
        }
      }
    }
    if(item.device == nullptr) continue;

    inputList.append(item);
  }
}

bool AbstractInput::append(string encode) {
  lstring mappings = mapping.split(",");
  if(mappings.find(encode)) return true;  //mapping already bound
  if(mapping.empty() || mapping == "None") mapping = encode;  //remove "None"
  else mapping.append(",", encode);  //add to existing mapping list
  bind();
  return true;
}

//

bool DigitalInput::bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(device.isNull() || (device.isKeyboard() && device.group[group].input[input].name == "Escape")) {
    inputList.reset();
    mapping = "None";
    return true;
  }

  string encode = {hex(device.id), "/", device.group[group].name, "/", device.group[group].input[input].name};

  if((device.isKeyboard() && group == HID::Keyboard::GroupID::Button)
  || (device.isMouse() && group == HID::Mouse::GroupID::Button)
  || (device.isJoypad() && group == HID::Joypad::GroupID::Button)
  ) {
    if(newValue != 0) return append(encode);
  }

  if((device.isJoypad() && group == HID::Joypad::GroupID::Axis)
  || (device.isJoypad() && group == HID::Joypad::GroupID::Hat)
  ) {
    if(newValue < -16384) return append({encode, "/Lo"});
    if(newValue > +16384) return append({encode, "/Hi"});
  }

  return false;
}

int16_t DigitalInput::poll() {
  if(program->focused() == false) return 0;
  if(inputList.size() == 0) return 0;
  bool result = logic;

  for(auto& item : inputList) {
    HID::Device& device = *(item.device);
    int16_t value = device.group[item.group].input[item.input].value;
    bool output = logic;
    if((device.isKeyboard() && item.group == HID::Keyboard::GroupID::Button)
    || (device.isMouse() && item.group == HID::Mouse::GroupID::Button)
    || (device.isJoypad() && item.group == HID::Joypad::GroupID::Button)
    ) {
      output = value;
    }
    if((device.isJoypad() && item.group == HID::Joypad::GroupID::Axis)
    || (device.isJoypad() && item.group == HID::Joypad::GroupID::Hat)
    ) {
      if(item.qualifier == Input::Qualifier::Lo) output = value < -16384;
      if(item.qualifier == Input::Qualifier::Hi) output = value > +16384;
    }
    if(logic == 0) result |= output;
    if(logic == 1) result &= output;
  }

  return result;
}

//

bool RelativeInput::bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(device.isNull() || (device.isKeyboard() && device.group[group].input[input].name == "Escape")) {
    inputList.reset();
    mapping = "None";
    return true;
  }

  string encode = {hex(device.id), "/", device.group[group].name, "/", device.group[group].input[input].name};

  if((device.isMouse() && group == HID::Mouse::GroupID::Axis)
  || (device.isJoypad() && group == HID::Joypad::GroupID::Axis)
  || (device.isJoypad() && group == HID::Joypad::GroupID::Hat)
  ) {
    if(newValue < -16384) return append(encode);
    if(newValue > +16384) return append(encode);
  }

  return false;
}

int16_t RelativeInput::poll() {
  if(program->focused() == false) return 0;
  if(inputList.size() == 0) return 0;
  int16_t result = 0;

  for(auto& item : inputList) {
    HID::Device& device = *(item.device);
    int16_t value = device.group[item.group].input[item.input].value;
    if(device.isJoypad() && item.group == HID::Joypad::GroupID::Axis) value >>= 8;
    if(device.isJoypad() && item.group == HID::Joypad::GroupID::Hat) value = (value < 0 ? -1 : value > 0 ? + 1 : 0);
    if(device.isMouse() && input.acquired() == false) value = 0;
    result += value;
  }

  return result;
}

//

bool RumbleInput::bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(device.isNull() || (device.isKeyboard() && device.group[group].input[input].name == "Escape")) {
    inputList.reset();
    mapping = "None";
    return true;
  }

  string encode = {hex(device.id), "/Rumble"};

  if(device.isJoypad() && group == HID::Joypad::GroupID::Button) {
    if(newValue != 0) return append(encode);
  }

  return false;
}

void RumbleInput::rumble(bool enable) {
  if(program->focused() == false) return;
  if(inputList.size() == 0) return;

  for(auto& item : inputList) {
    input.rumble(item.id, enable);
  }
}

//

HotkeyInput::HotkeyInput() {
  logic = 1;  //AND
  inputManager->hotkeyMap.append(this);
}

//

//convert an input mapping string to a more human-readable form for the UI
string InputManager::sanitize(string mapping, string concatenate) const {
  lstring values = mapping.split(",");
  for(auto& value : values) {
    lstring part = value.split("/");
    if(part.size() < 2) continue;  //skip "None" mapping
    if(part[0] == "1") part[0] = "Keyboard";
    else if(part[0] == "2") part[0] = "Mouse";
    else part[0] = {"Joypad(", part[0].slice(0, 3), ")"};
    value = part.merge(".");
  }
  return values.merge(concatenate);
}

void InputManager::onChange(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(settings->focused()) {
    inputSettings->inputEvent(device, group, input, oldValue, newValue);
    hotkeySettings->inputEvent(device, group, input, oldValue, newValue);
  }
}

HID::Device* InputManager::findMouse() {
  for(auto device : devices) {
    if(device->isMouse()) return device;
  }
  return nullptr;
}

void InputManager::bind() {
  for(auto& input : inputMap) input->bind();
  for(auto& input : hotkeyMap) input->bind();
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

  if(presentation->focused()) pollHotkeys();
}

void InputManager::saveConfiguration() {
  config.save(program->path("input.bml"));
}

InputManager::InputManager() {
  inputManager = this;
  bootstrap();

  input.onChange = {&InputManager::onChange, this};
}

void InputManager::bootstrap() {
  unsigned guid = 0;
  for(auto& emulator : program->emulator) {
    Configuration::Node emulatorNode;

    for(auto& port : emulator->port) {
      Configuration::Node portNode;

      for(auto& device : port.device) {
        Configuration::Node deviceNode;

        for(auto& number : device.order) {
          auto& input = device.input[number];

          AbstractInput* abstract = nullptr;
          if(input.type == 0) abstract = new DigitalInput;
          if(input.type == 1) abstract = new RelativeInput;
          if(input.type == 2) abstract = new RumbleInput;
          if(abstract == nullptr) continue;

          abstract->name = string{input.name}.replace(" ", "");
          abstract->mapping = "None";
          abstract->logic = 0;  //OR

          input.guid = guid++;
          inputMap.append(abstract);

          deviceNode.append(abstract->mapping, abstract->name);
        }

        portNode.append(deviceNode, string{device.name}.replace(" ", ""));
      }

      emulatorNode.append(portNode, string{port.name}.replace(" ", ""));
    }

    config.append(emulatorNode, string{emulator->information.name}.replace(" ", ""));
  }

  appendHotkeys();

  config.load(program->path("input.bml"));
  config.save(program->path("input.bml"));

  bind();
}
