HotkeySettings *hotkeySettings = nullptr;

HotkeySettings::HotkeySettings() : activeInput(nullptr) {
  title.setFont(application->titleFont);
  title.setText("Hotkey Bindings");

  inputList.setHeaderText("Name", "Mapping");
  inputList.setHeaderVisible();
  clearButton.setText("Clear");

  append(title, {~0, 0}, 5);
  append(inputList, {~0, ~0}, 5);
  append(controlLayout, {~0, 0});
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(clearButton, {80, 0});

  inputList.onChange = {&HotkeySettings::synchronize, this};
  inputList.onActivate = {&HotkeySettings::assignInput, this};
  clearButton.onActivate = {&HotkeySettings::clearInput, this};

  refresh();
}

void HotkeySettings::synchronize() {
  clearButton.setEnabled(inputList.selected());
}

void HotkeySettings::refresh() {
  inputList.reset();
  for(auto &hotkey : inputManager->hotkeyMap) {
    string mapping = hotkey->mapping;
    mapping.replace(",", " and ");
    inputList.append(hotkey->name, mapping);
  }
  synchronize();
}

void HotkeySettings::clearInput() {
  activeInput = inputManager->hotkeyMap[inputList.selection()];
  inputEvent(Scancode::None, 1);
}

void HotkeySettings::assignInput() {
  activeInput = inputManager->hotkeyMap[inputList.selection()];

  settings->setStatusText({"Set assignment for [", activeInput->name, "] ..."});
  settings->panelList.setEnabled(false);
  inputList.setEnabled(false);
  clearButton.setEnabled(false);
}

void HotkeySettings::inputEvent(unsigned scancode, int16_t value) {
  using nall::Mouse;

  if(activeInput == nullptr) return;
  if(Mouse::isAnyButton(scancode) || Mouse::isAnyAxis(scancode)) return;
  if(Joypad::isAnyAxis(scancode)) return;
  if(activeInput->bind(scancode, value) == false) return;

  activeInput = nullptr;
  settings->setStatusText("");
  settings->panelList.setEnabled(true);
  inputList.setEnabled(true);
  clearButton.setEnabled(true);
  refresh();
}
