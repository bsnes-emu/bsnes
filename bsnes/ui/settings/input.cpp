InputSettings inputSettings;
static InputMapper::AbstractInput *activeInput = 0;

void InputSettings::create() {
  title.setText("Input Settings");
  title.setFont(application.titleFont);

  locked = false;
  activeInput = 0;
  activeMouse = 0;
  joypadsCalibrated = false;
  joypadsCalibrating = false;

  portLabel.setText("Port:");
  portBox.append(inputMapper.port1.name);
  portBox.append(inputMapper.port2.name);
  portBox.append(inputMapper.hotkeys.name);
  deviceLabel.setText("Device:");
  clearButton.setText("Clear");

  mappingList.setHeaderText("Name", "Mapping");
  mappingList.setHeaderVisible(true);

  panelLayout.setMargin(5);
  panelLayout.append(panel, SettingsWindow::PanelWidth, ~0, 5);
  panelLayout.append(layout);

  layout.append(title, ~0, 0, 5);

  selectionLayout.append(portLabel, 0, 0, 5);
  selectionLayout.append(portBox, ~0, 0, 5);
  selectionLayout.append(deviceLabel, 0, 0, 5);
  selectionLayout.append(deviceBox, ~0, 0);
  layout.append(selectionLayout, 5);

  layout.append(mappingList, ~0, ~0, 5);

  controlLayout.append(customButton1, 100, 0, 5);
  controlLayout.append(customButton2, 100, 0, 5);
  controlLayout.append(customButton3, 100, 0, 5);
  controlLayout.append(spacer, ~0, 0);
  controlLayout.append(clearButton, 100, 0);
  layout.append(controlLayout);

  settingsWindow.append(panelLayout);

  portBox.onChange = { &InputSettings::portChanged, this };
  deviceBox.onChange = { &InputSettings::deviceChanged, this };

  mappingList.onActivate = { &InputSettings::assignInput, this };

  customButton1.onTick = [this]() { manualInput(1); };
  customButton2.onTick = [this]() { manualInput(2); };
  customButton3.onTick = [this]() { manualInput(3); };
  clearButton.onTick = [this]() { manualInput(0); };

  portChanged();
}

void InputSettings::focus() {
  mappingList.autoSizeColumns();
  customButton1.setVisible(false);
  customButton2.setVisible(false);
  customButton3.setVisible(false);
}

void InputSettings::portChanged() {
  locked = true;
  deviceBox.reset();
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0 ? (InputMapper::ControllerPort&)inputMapper.port1 :
    portBox.selection() == 1 ? (InputMapper::ControllerPort&)inputMapper.port2 :
  /*portBox.selection() == 2*/ (InputMapper::ControllerPort&)inputMapper.hotkeys
  );

  for(unsigned i = 0; i < port.size(); i++) deviceBox.append(port[i]->name);
  locked = false;
  deviceChanged();
}

void InputSettings::deviceChanged() {
  if(locked) return;
  locked = true;
  mappingList.reset();
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0 ? (InputMapper::ControllerPort&)inputMapper.port1 :
    portBox.selection() == 1 ? (InputMapper::ControllerPort&)inputMapper.port2 :
  /*portBox.selection() == 2*/ (InputMapper::ControllerPort&)inputMapper.hotkeys
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  for(unsigned i = 0; i < controller.size(); i++) {
    string mapping = controller[i]->mapping;
    if(mapping == "") mapping = "None";
    mappingList.append(controller[i]->name, mapping);
  }
  mappingList.autoSizeColumns();
  locked = false;
}

void InputSettings::mappingChanged() {
  if(locked) return;
  locked = true;
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0 ? (InputMapper::ControllerPort&)inputMapper.port1 :
    portBox.selection() == 1 ? (InputMapper::ControllerPort&)inputMapper.port2 :
  /*portBox.selection() == 2*/ (InputMapper::ControllerPort&)inputMapper.hotkeys
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  for(unsigned i = 0; i < controller.size(); i++) {
    string mapping = controller[i]->mapping;
    if(mapping == "") mapping = "None";
    mappingList.modify(i, controller[i]->name, mapping);
  }
  mappingList.autoSizeColumns();
  locked = false;
}

void InputSettings::assignInput() {
  if(mappingList.selected() == false) return;
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0 ? (InputMapper::ControllerPort&)inputMapper.port1 :
    portBox.selection() == 1 ? (InputMapper::ControllerPort&)inputMapper.port2 :
  /*portBox.selection() == 2*/ (InputMapper::ControllerPort&)inputMapper.hotkeys
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  settingsWindow.panel.setEnabled(false);
  portBox.setEnabled(false);
  deviceBox.setEnabled(false);
  mappingList.setEnabled(false);
  inputMapper.poll();  //flush any pending keypresses
  activeInput = controller[mappingList.selection()];
  settingsWindow.setStatusText({ "Set assignment for [", activeInput->name, "] ..." });

  if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
    customButton1.setText("Mouse X-axis");
    customButton2.setText("Mouse Y-axis");
    customButton3.setText("Mouse Z-axis");
  } else {
    customButton1.setText("Mouse Left");
    customButton2.setText("Mouse Middle");
    customButton3.setText("Mouse Right");
  }
  customButton1.setVisible(true);
  customButton2.setVisible(true);
  customButton3.setVisible(true);
}

void InputSettings::manualInput(unsigned button) {
  if(activeInput == 0 && button == 0) return clearInput();
  if(activeInput == 0) return;

  switch(button) {
  case 0:
    setMapping("");
    break;
  case 1:
    if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
      setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Xaxis]));
    } else {
      setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Button0]));
    }
    break;
  case 2:
    if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
      setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Yaxis]));
    } else {
      setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Button1]));
    }
    break;
  case 3:
    if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
      setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Zaxis]));
    } else {
      setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Button2]));
    }
    break;
  }
}

void InputSettings::clearInput() {
  if(mappingList.selected() == false) return;
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0 ? (InputMapper::ControllerPort&)inputMapper.port1 :
    portBox.selection() == 1 ? (InputMapper::ControllerPort&)inputMapper.port2 :
  /*portBox.selection() == 2*/ (InputMapper::ControllerPort&)inputMapper.hotkeys
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  controller[mappingList.selection()]->mapping = "";
  inputMapper.bind();
  endAssignment();
}

void InputSettings::setMapping(const string &mapping) {
  activeInput->mapping = mapping;
  inputMapper.bind();
  endAssignment();
}

void InputSettings::endAssignment() {
  activeInput = 0;
  settingsWindow.panel.setEnabled(true);
  portBox.setEnabled(true);
  deviceBox.setEnabled(true);
  mappingList.setEnabled(true);
  settingsWindow.setStatusText("");
  customButton1.setVisible(false);
  customButton2.setVisible(false);
  customButton3.setVisible(false);
  mappingChanged();
  mappingList.setFocused();
}

void InputSettings::inputEvent(uint16_t scancode, int16_t value) {
  if(activeInput == 0) return;  //not remapping any keys right now?

  string mapping = Scancode::encode(scancode);
  if(auto position = strpos(mapping, "::Escape")) return setMapping("");

  if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
  } else if(dynamic_cast<InputMapper::DigitalInput*>(activeInput)) {
    if(Keyboard::isAnyKey(scancode) && value) {
      setMapping(mapping);
    } else if(Mouse::isAnyButton(scancode) && value) {
      activeMouse = Mouse::numberDecode(scancode);
    } else if(Joypad::isAnyHat(scancode) && value) {
      if(value == Joypad::HatUp) setMapping({ mapping, ".Up" });
      else if(value == Joypad::HatDown) setMapping({ mapping, ".Down" });
      else if(value == Joypad::HatLeft) setMapping({ mapping, ".Left" });
      else if(value == Joypad::HatRight) setMapping({ mapping, ".Right" });
    } else if(Joypad::isAnyAxis(scancode)) {
      if(joypadsCalibrated == false) return calibrateJoypads();
      unsigned joypadNumber = Joypad::numberDecode(scancode);
      unsigned axisNumber = Joypad::axisDecode(scancode);
      int16_t calibration = joypadCalibration[joypadNumber][axisNumber];
      if(calibration > -12288 && calibration < +12288 && value < -24576) setMapping({ mapping, ".Lo" });
      else if(calibration > -12288 && calibration < +12288 && value > +24576) setMapping({ mapping, ".Hi" });
      else if(calibration <= -12288 && value >= +12288) setMapping({ mapping, ".Hi" });
      else if(calibration >= +12288 && value <= -12288) setMapping({ mapping, ".Lo" });
    } else if(Joypad::isAnyButton(scancode) && value) {
      setMapping(mapping);
    }
  }
}

void InputSettings::calibrateJoypads() {
  if(joypadsCalibrating == true) return;
  joypadsCalibrating = true;
  MessageWindow::information(settingsWindow,
    "Analog joypads must be calibrated prior to use.\n\n"
    "Please move all analog axes, and press all analog buttons.\n"
    "Please do this for every controller you wish to use.\n"
    "Once finished, please let go of all axes and buttons, and press OK."
  );
  inputMapper.poll();
  for(unsigned j = 0; j < Joypad::Count &&j<2; j++) {
    for(unsigned a = 0; a < Joypad::Axes; a++) {
      joypadCalibration[j][a] = inputMapper.value(joypad(j).axis(a));
    }
  }
  joypadsCalibrating = false;
  joypadsCalibrated = true;
}
