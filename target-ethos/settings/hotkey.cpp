HotkeySettings* hotkeySettings = nullptr;

HotkeySettings::HotkeySettings() {
  inputList.setHeaderText({"Name", "Mapping"});
  inputList.setHeaderVisible();
  eraseButton.setText("Erase");

  append(inputList, {~0, ~0}, 5);
  append(controlLayout, {~0, 0});
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(eraseButton, {80, 0});

  inputList.onChange = {&HotkeySettings::synchronize, this};
  inputList.onActivate = {&HotkeySettings::assignInput, this};
  eraseButton.onActivate = {&HotkeySettings::eraseInput, this};

  for(auto& hotkey : inputManager->hotkeyMap) inputList.append({"", ""});
  refresh();
}

void HotkeySettings::synchronize() {
  eraseButton.setEnabled(inputList.selected());
}

void HotkeySettings::refresh() {
  unsigned index = 0;
  for(auto& hotkey : inputManager->hotkeyMap) {
    string mapping = hotkey->mapping;
    mapping.replace(",", " and ");
    inputList.setText(index++, {hotkey->name, mapping});
  }
  synchronize();
}

void HotkeySettings::eraseInput() {
  activeInput = inputManager->hotkeyMap[inputList.selection()];
  inputEvent(hidNull, 0, 0, 0, 1);
}

void HotkeySettings::assignInput() {
  activeInput = inputManager->hotkeyMap[inputList.selection()];

  settings->setStatusText({"Set assignment for [", activeInput->name, "] ..."});
  settings->layout.setEnabled(false);
  setEnabled(false);
}

void HotkeySettings::inputEvent(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(activeInput == nullptr) return;
  if(device.isMouse()) return;
  if(device.isJoypad() && group == HID::Joypad::GroupID::Axis) return;
  if(activeInput->bind(device, group, input, oldValue, newValue) == false) return;

  activeInput = nullptr;
  settings->setStatusText("");
  settings->layout.setEnabled(true);
  setEnabled(true);
  refresh();
}
