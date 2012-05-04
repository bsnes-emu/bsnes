InputSettings *inputSettings = nullptr;

InputSettings::InputSettings() : activeInput(nullptr) {
  title.setFont(application->titleFont);
  title.setText("Input Settings");
  focusLabel.setText("When Focus is Lost:");
  focusPause.setText("Pause Emulation");
  focusAllow.setText("Allow Input");
  inputList.setHeaderText("Name", "Mapping");
  inputList.setHeaderVisible();
  clearButton.setText("Clear");

  append(title, {~0, 0}, 5);
  append(focusLayout, {~0, 0}, 5);
    focusLayout.append(focusLabel, {0, 0}, 5);
    focusLayout.append(focusPause, {0, 0}, 5);
    focusLayout.append(focusAllow, {0, 0});
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

  focusPause.setChecked(config->input.focusPause);
  focusAllow.setChecked(config->input.focusAllow);
  focusAllow.setEnabled(!config->input.focusPause);

  focusPause.onToggle = [&] { config->input.focusPause = focusPause.checked(); focusAllow.setEnabled(!focusPause.checked()); };
  focusAllow.onToggle = [&] { config->input.focusAllow = focusAllow.checked(); };
  systemList.onChange = {&InputSettings::systemChanged, this};
  portList.onChange = {&InputSettings::portChanged, this};
  deviceList.onChange = {&InputSettings::deviceChanged, this};
  inputList.onChange = {&InputSettings::synchronize, this};
  inputList.onActivate = {&InputSettings::assignInput, this};
  assign[0].onActivate = [&] { assignMouseInput(0); };
  assign[1].onActivate = [&] { assignMouseInput(1); };
  assign[2].onActivate = [&] { assignMouseInput(2); };
  clearButton.onActivate = {&InputSettings::clearInput, this};

  systemChanged();
}

void InputSettings::synchronize() {
  if(inputList.selected() == false) {
    assign[0].setVisible(false);
    assign[1].setVisible(false);
    assign[2].setVisible(false);
  } else {
    unsigned number = activeDevice().order[inputList.selection()];
    auto &input = activeDevice().input[number];
    auto selectedInput = inputManager->inputMap[input.guid];

    if(dynamic_cast<DigitalInput*>(selectedInput)) {
      assign[0].setText("Mouse Left");
      assign[1].setText("Mouse Middle");
      assign[2].setText("Mouse Right");
      assign[0].setVisible(true);
      assign[1].setVisible(true);
      assign[2].setVisible(true);
    }

    if(dynamic_cast<AnalogInput*>(selectedInput)) {
      assign[0].setText("Mouse X-axis");
      assign[1].setText("Mouse Y-axis");
      assign[0].setVisible(true);
      assign[1].setVisible(true);
      assign[2].setVisible(false);
    }
  }

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
  for(unsigned number : activeDevice().order) {
    auto &input = activeDevice().input[number];
    auto abstract = inputManager->inputMap(input.guid);
    string mapping = abstract->mapping;
    mapping.replace("KB0::", "");
    mapping.replace("MS0::", "Mouse::");
    mapping.replace(",", " or ");
    inputList.append(input.name, mapping);
  }
  synchronize();
}

void InputSettings::clearInput() {
  unsigned number = activeDevice().order[inputList.selection()];
  auto &input = activeDevice().input[number];
  activeInput = inputManager->inputMap[input.guid];
  inputEvent(Scancode::None, 1);
}

void InputSettings::assignInput() {
  unsigned number = activeDevice().order[inputList.selection()];
  auto &input = activeDevice().input[number];
  activeInput = inputManager->inputMap[input.guid];

  settings->setStatusText({"Set assignment for [", activeDevice().name, "::", input.name, "] ..."});
  settings->layout.setEnabled(false);
  setEnabled(false);
}

void InputSettings::assignMouseInput(unsigned n) {
  unsigned number = activeDevice().order[inputList.selection()];
  auto &input = activeDevice().input[number];
  activeInput = inputManager->inputMap[input.guid];

  if(dynamic_cast<DigitalInput*>(activeInput)) {
    return inputEvent(mouse(0).button(n), 1, true);
  }

  if(dynamic_cast<AnalogInput*>(activeInput)) {
    return inputEvent(mouse(0).axis(n), 1, true);
  }
}

void InputSettings::inputEvent(unsigned scancode, int16_t value, bool allowMouseInput) {
  using nall::Mouse;
  if(activeInput == nullptr) return;
  if(allowMouseInput == false && (Mouse::isAnyButton(scancode) || Mouse::isAnyAxis(scancode))) return;
  if(activeInput->bind(scancode, value) == false) return;

  activeInput = nullptr;
  deviceChanged();
  settings->setStatusText("");
  settings->layout.setEnabled(true);
  setEnabled(true);
  synchronize();
}
