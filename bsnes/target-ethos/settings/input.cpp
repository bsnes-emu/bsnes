InputSettings *inputSettings = nullptr;

InputSettings::InputSettings() {
  title.setFont(application->titleFont);
  title.setText("Input Settings");
  inputList.setHeaderText("Name", "Mapping");
  inputList.setHeaderVisible();
  clearButton.setText("Clear");

  append(title, {~0, 0}, 5);
  append(selectionLayout, {~0, 0}, 5);
    selectionLayout.append(systemList, {~0, 0}, 5);
    selectionLayout.append(portList, {~0, 0}, 5);
    selectionLayout.append(deviceList, {~0, 0});
  append(inputList, {~0, ~0}, 5);
  append(controlLayout, {~0, 0});
    controlLayout.append(assign[0], {100, 0}, 5);
    controlLayout.append(assign[1], {100, 0}, 5);
    controlLayout.append(assign[2], {100, 0}, 5);
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(clearButton, {80, 0});

  for(auto &emulator : application->emulator) {
    systemList.append(emulator->information.name);
  }

  systemList.onChange = {&InputSettings::systemChanged, this};
  portList.onChange = {&InputSettings::portChanged, this};
  deviceList.onChange = {&InputSettings::deviceChanged, this};
  inputList.onChange = {&InputSettings::synchronize, this};

  systemChanged();
}

void InputSettings::synchronize() {
  clearButton.setEnabled(inputList.selected());
}

Emulator::Interface& InputSettings::activeSystem() {
  return *application->emulator[systemList.selection()];
}

Emulator::Interface::Port& InputSettings::activePort() {
  return activeSystem().port[portList.selection()];
}

Emulator::Interface::Port::Device& InputSettings::activeDevice() {
  return activePort().device[deviceList.selection()];
}

void InputSettings::systemChanged() {
  portList.reset();
  for(auto &port : activeSystem().port) {
    portList.append(port.name);
  }
  portChanged();
}

void InputSettings::portChanged() {
  deviceList.reset();
  for(auto &device : activePort().device) {
    deviceList.append(device.name);
  }
  deviceChanged();
}

void InputSettings::deviceChanged() {
  inputList.reset();
  for(unsigned number : activeDevice().displayinput) {
    auto &input = activeDevice().input[number];
    inputList.append(input.name, inputManager->inputMap(input.guid).mapping);
  }
  synchronize();
}
