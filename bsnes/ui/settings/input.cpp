InputSettings *inputSettings = 0;

InputSettings::InputSettings() : activeInput(0) {
  title.setFont(application->titleFont);
  title.setText("Input Settings");
  inputList.setHeaderText("Name", "Mapping");
  inputList.setHeaderVisible();
  clearButton.setText("Clear");

  for(unsigned n = 0; n < inputManager->inputList.size(); n++) {
    primary.append(inputManager->inputList[n].name);
  }
  primaryChange();

  append(title, ~0, 0, 5);
  append(selectionLayout, ~0, 0, 5);
    selectionLayout.append(primary, ~0, 0, 5);
    selectionLayout.append(secondary, ~0, 0, 5);
    selectionLayout.append(tertiary, ~0, 0);
  append(inputList, ~0, ~0, 5);
  append(controlLayout, ~0, 0);
    controlLayout.append(spacer, ~0, 0);
    controlLayout.append(clearButton, 80, 0);

  primary.onChange = { &InputSettings::primaryChange, this };
  secondary.onChange = { &InputSettings::secondaryChange, this };
  tertiary.onChange = { &InputSettings::tertiaryChange, this };
  inputList.onChange = { &InputSettings::synchronize, this };
  inputList.onActivate = { &InputSettings::assignInputBegin, this };

  synchronize();
}

void InputSettings::synchronize() {
  clearButton.setEnabled(inputList.selected());
}

void InputSettings::primaryChange() {
  secondary.reset();
  tertiary.reset();
  PrimaryInput &input = inputManager->inputList[primary.selection()];
  for(unsigned n = 0; n < input.size(); n++) {
    secondary.append(input[n].name);
  }
  secondaryChange();
}

void InputSettings::secondaryChange() {
  tertiary.reset();
  PrimaryInput &pinput = inputManager->inputList[primary.selection()];
  SecondaryInput &input = pinput[secondary.selection()];
  for(unsigned n = 0; n < input.size(); n++) {
    tertiary.append(input[n].name);
  }
  tertiaryChange();
}

void InputSettings::tertiaryChange() {
  inputList.reset();
  PrimaryInput &pinput = inputManager->inputList[primary.selection()];
  SecondaryInput &sinput = pinput[secondary.selection()];
  TertiaryInput &input = sinput[tertiary.selection()];
  for(unsigned n = 0; n < input.size(); n++) {
    inputList.append(input[n].name, input[n].mapping);
  }
  synchronize();
  inputList.autoSizeColumns();
}

void InputSettings::assignInputBegin() {
  PrimaryInput &pinput = inputManager->inputList[primary.selection()];
  SecondaryInput &sinput = pinput[secondary.selection()];
  TertiaryInput &tinput = sinput[tertiary.selection()];
  activeInput = &tinput[inputList.selection()];

  settingsWindow->layout.setEnabled(false);
  settingsWindow->setStatusText({ "Set asssignment for [", tinput.name, "::", activeInput->name, "] ..." });
}

void InputSettings::inputEvent(int16_t scancode, int16_t value) {
  if(activeInput == 0) return;
  if(activeInput->bind(scancode, value) == false) return;

  activeInput = 0;
  tertiaryChange();
  settingsWindow->setStatusText("");
  settingsWindow->layout.setEnabled(true);
}
