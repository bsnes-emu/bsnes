InputSettings *inputSettings = 0;

InputSettings::InputSettings() : activeInput(0) {
  title.setFont(application->titleFont);
  title.setText("Input Settings");
  inputList.setHeaderText("Name", "Mapping");
  inputList.setHeaderVisible();
  assignPrimary.setVisible(false);
  assignSecondary.setVisible(false);
  assignTertiary.setVisible(false);
  clearButton.setText("Clear");

  for(unsigned n = 0; n < inputManager->inputList.size(); n++) {
    primary.append(inputManager->inputList[n].name);
  }
  primaryChange();

  append(title,                           {  ~0,  0 }, 5);
  append(selectionLayout,                 {  ~0,  0 }, 5);
    selectionLayout.append(primary,       {  ~0,  0 }, 5);
    selectionLayout.append(secondary,     {  ~0,  0 }, 5);
    selectionLayout.append(tertiary,      {  ~0,  0 }, 0);
  append(inputList,                       {  ~0, ~0 }, 5);
  append(controlLayout,                   {  ~0,  0 }, 0);
    controlLayout.append(assignPrimary,   { 100,  0 }, 5);
    controlLayout.append(assignSecondary, { 100,  0 }, 5);
    controlLayout.append(assignTertiary,  { 100,  0 }, 5);
    controlLayout.append(spacer,          {  ~0,  0 }, 0);
    controlLayout.append(clearButton,     {  80,  0 }, 0);

  primary.onChange = { &InputSettings::primaryChange, this };
  secondary.onChange = { &InputSettings::secondaryChange, this };
  tertiary.onChange = { &InputSettings::tertiaryChange, this };
  inputList.onChange = { &InputSettings::synchronize, this };
  inputList.onActivate = { &InputSettings::assignInput, this };
  assignPrimary.onActivate = [&] { assignMouseInput(0); };
  assignSecondary.onActivate = [&] { assignMouseInput(1); };
  assignTertiary.onActivate = [&] { assignMouseInput(2); };
  clearButton.onActivate = { &InputSettings::clearInput, this };

  synchronize();
}

void InputSettings::synchronize() {
  PrimaryInput &pinput = inputManager->inputList[primary.selection()];
  SecondaryInput &sinput = pinput[secondary.selection()];
  TertiaryInput &tinput = sinput[tertiary.selection()];
  secondary.setEnabled(pinput.size() > 1);
  tertiary.setEnabled(sinput.size() > 1);
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

void InputSettings::assignInput() {
  PrimaryInput &pinput = inputManager->inputList[primary.selection()];
  SecondaryInput &sinput = pinput[secondary.selection()];
  TertiaryInput &tinput = sinput[tertiary.selection()];
  activeInput = &tinput[inputList.selection()];

  settingsWindow->setStatusText({ "Set asssignment for [", tinput.name, "::", activeInput->name, "] ..." });
  settingsWindow->panelList.setEnabled(false);
  primary.setEnabled(false);
  secondary.setEnabled(false);
  tertiary.setEnabled(false);
  inputList.setEnabled(false);

  if(dynamic_cast<AnalogInput*>(activeInput)) {
    assignPrimary.setText("Mouse X-axis");
    assignSecondary.setText("Mouse Y-axis");
    assignPrimary.setVisible(true);
    assignSecondary.setVisible(true);
  }

  if(dynamic_cast<DigitalInput*>(activeInput)) {
    assignPrimary.setText("Mouse Left");
    assignSecondary.setText("Mouse Middle");
    assignTertiary.setText("Mouse Right");
    assignPrimary.setVisible(true);
    assignSecondary.setVisible(true);
    assignTertiary.setVisible(true);
  }
}

void InputSettings::assignMouseInput(unsigned n) {
  if(activeInput == 0) return;

  if(dynamic_cast<AnalogInput*>(activeInput)) {
    return inputEvent(mouse(0).axis(n), 1u, true);
  }

  if(dynamic_cast<DigitalInput*>(activeInput)) {
    return inputEvent(mouse(0).button(n), 1u, true);
  }
}

void InputSettings::clearInput() {
  PrimaryInput &pinput = inputManager->inputList[primary.selection()];
  SecondaryInput &sinput = pinput[secondary.selection()];
  TertiaryInput &tinput = sinput[tertiary.selection()];
  activeInput = &tinput[inputList.selection()];
  inputEvent(Scancode::None, 1u);
}

void InputSettings::inputEvent(int16_t scancode, int16_t value, bool allowMouseInput) {
  using nall::Mouse;
  if(activeInput == 0) return;
  if(allowMouseInput == false && (Mouse::isAnyButton(scancode) || Mouse::isAnyAxis(scancode))) return;
  if(activeInput->bind(scancode, value) == false) return;

  activeInput = 0;
  tertiaryChange();
  settingsWindow->setStatusText("");
  settingsWindow->panelList.setEnabled(true);
  primary.setEnabled(true);
  inputList.setEnabled(true);
  assignPrimary.setVisible(false);
  assignSecondary.setVisible(false);
  assignTertiary.setVisible(false);
  synchronize();
}
