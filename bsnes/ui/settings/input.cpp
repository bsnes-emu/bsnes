InputSettings inputSettings;
static InputMapper::AbstractInput *activeInput = 0;

void InputSettings::create() {
  setTitle("Input Settings");
  application.addWindow(this, "InputSettings", "160,160");
  setStatusFont(application.proportionalFontBold);
  setStatusVisible();

  activeInput = 0;
  activeMouse = 0;

  portLabel.setText("Port:");
  portBox.append(inputMapper.port1.name);
  portBox.append(inputMapper.port2.name);
  deviceLabel.setText("Device:");
  mappingList.setHeaderText("Name", "Mapping");
  mappingList.setHeaderVisible(true);
  clearButton.setText("Clear");
  mouseXaxis.setText("Mouse X-axis");
  mouseYaxis.setText("Mouse Y-axis");
  mouseLeft.setText("Mouse Left");
  mouseMiddle.setText("Mouse Middle");
  mouseRight.setText("Mouse Right");

  layout.setMargin(5);
  selectionLayout.append(portLabel,   50, 0, 5);
  selectionLayout.append(portBox,      0, 0, 5);
  selectionLayout.append(deviceLabel, 50, 0, 5);
  selectionLayout.append(deviceBox,    0, 0);
  layout.append(selectionLayout, 0, Style::ComboBoxHeight, 5);
  layout.append(mappingList, 0, 0, 5);
  mapLayout.append(spacer,       0, 0);
  mapLayout.append(clearButton, 80, 0);
  layout.append(mapLayout, 0, Style::ButtonHeight);

  axisLayout.setMargin(5);
  axisLayout.append(axisSpacer, 0, 0);
  axisControlLayout.append(mouseXaxis, 100, 0, 5);
  axisControlLayout.append(mouseYaxis, 100, 0, 5);
  axisLayout.append(axisControlLayout, 0, Style::ButtonHeight);

  buttonLayout.setMargin(5);
  buttonLayout.append(buttonSpacer, 0, 0);
  buttonControlLayout.append(mouseLeft,   100, 0, 5);
  buttonControlLayout.append(mouseMiddle, 100, 0, 5);
  buttonControlLayout.append(mouseRight,  100, 0, 5);
  buttonLayout.append(buttonControlLayout, 0, Style::ButtonHeight);

  setGeometry({ 0, 0, 480, layout.minimumHeight() + 250 });
  append(layout);
  append(axisLayout);
  append(buttonLayout);

  axisLayout.setVisible(false);
  buttonLayout.setVisible(false);

  portChanged();
  portBox.onChange = { &InputSettings::portChanged, this };
  deviceBox.onChange = { &InputSettings::deviceChanged, this };
  mappingList.onActivate = { &InputSettings::assignInput, this };

  mouseXaxis.onTick = []() { inputSettings.setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Xaxis])); };
  mouseYaxis.onTick = []() { inputSettings.setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Yaxis])); };
  mouseLeft.onTick = []() { inputSettings.setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Button0])); };
  mouseMiddle.onTick = []() { inputSettings.setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Button1])); };
  mouseRight.onTick = []() { inputSettings.setMapping(Scancode::encode(mouse(inputSettings.activeMouse)[Mouse::Button2])); };

  clearButton.onTick = { &InputSettings::clearInput, this };

  onClose = []() { inputSettings.endAssignment(); };
}

void InputSettings::portChanged() {
  deviceBox.reset();
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0
    ? (InputMapper::ControllerPort&)inputMapper.port1
    : (InputMapper::ControllerPort&)inputMapper.port2
  );

  for(unsigned i = 0; i < port.size(); i++) deviceBox.append(port[i]->name);
  deviceChanged();
}

void InputSettings::deviceChanged() {
  mappingList.reset();
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0
    ? (InputMapper::ControllerPort&)inputMapper.port1
    : (InputMapper::ControllerPort&)inputMapper.port2
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  for(unsigned i = 0; i < controller.size(); i++) {
    string mapping = controller[i]->mapping;
    if(mapping == "") mapping = "None";
    mappingList.append(controller[i]->name, mapping);
  }
  mappingList.autoSizeColumns();
}

void InputSettings::mappingChanged() {
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0
    ? (InputMapper::ControllerPort&)inputMapper.port1
    : (InputMapper::ControllerPort&)inputMapper.port2
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  for(unsigned i = 0; i < controller.size(); i++) {
    string mapping = controller[i]->mapping;
    if(mapping == "") mapping = "None";
    mappingList.modify(i, controller[i]->name, mapping);
  }
  mappingList.autoSizeColumns();
}

void InputSettings::assignInput() {
  if(mappingList.selected() == false) return;
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0
    ? (InputMapper::ControllerPort&)inputMapper.port1
    : (InputMapper::ControllerPort&)inputMapper.port2
  );
  InputMapper::Controller &controller = (InputMapper::Controller&)*port[deviceBox.selection()];

  portBox.setEnabled(false);
  deviceBox.setEnabled(false);
  mappingList.setEnabled(false);
  inputMapper.poll();  //flush any pending keypresses
  activeInput = controller[mappingList.selection()];
  setStatusText({ "Set assignment for [", activeInput->name, "] ..." });
  if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
    axisLayout.setVisible(true);
    buttonLayout.setVisible(false);
  } else {
    axisLayout.setVisible(false);
    buttonLayout.setVisible(true);
  }
}

void InputSettings::clearInput() {
  if(mappingList.selected() == false) return;
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0
    ? (InputMapper::ControllerPort&)inputMapper.port1
    : (InputMapper::ControllerPort&)inputMapper.port2
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
  portBox.setEnabled(true);
  deviceBox.setEnabled(true);
  mappingList.setEnabled(true);
  setStatusText("");
  axisLayout.setVisible(false);
  buttonLayout.setVisible(false);
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
  MessageWindow::information(*this,
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

InputSettings::InputSettings() {
  joypadsCalibrated = false;
  joypadsCalibrating = false;
}
