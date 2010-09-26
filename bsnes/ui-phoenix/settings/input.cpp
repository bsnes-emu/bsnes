InputSettings inputSettings;
static InputMapper::AbstractInput *activeInput = 0;

void InputSettings::create() {
  application.windows.append(this);
  Window::create(0, 0, 256, 256, "Input Settings");
  setDefaultFont(application.proportionalFont);
  setFont(application.proportionalFontBold);
  setStatusVisible();

  unsigned x = 5, y = 5;

  portLabel.create(*this, x, y, 50, Style::ComboBoxHeight, "Port:");
  portBox.create(*this, x + 50, y, 200, Style::ComboBoxHeight);
  portBox.addItem(inputMapper.port1.name);
  portBox.addItem(inputMapper.port2.name);
  deviceLabel.create(*this, x + 255, y, 50, Style::ComboBoxHeight, "Device:");
  deviceBox.create(*this, x + 305, y, 200, Style::ComboBoxHeight); y += Style::ComboBoxHeight + 5;

  mappingList.create(*this, x, y, 505, 300, "Name\tMapping"); y += 300 + 5;
  mappingList.setHeaderVisible();
  mappingList.setFocused();

  setGeometry(160, 160, 515, y);

  portChanged();
  portBox.onChange = { &InputSettings::portChanged, this };
  deviceBox.onChange = { &InputSettings::deviceChanged, this };
  mappingList.onActivate = { &InputSettings::assignInput, this };
}

void InputSettings::portChanged() {
  deviceBox.reset();
  InputMapper::ControllerPort &port = (
    portBox.selection() == 0
    ? (InputMapper::ControllerPort&)inputMapper.port1
    : (InputMapper::ControllerPort&)inputMapper.port2
  );

  for(unsigned i = 0; i < port.size(); i++) {
    deviceBox.addItem(port[i]->name);
  }
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
    mappingList.addItem(string(controller[i]->name, "\t", mapping));
  }
  mappingList.resizeColumnsToContent();
}

void InputSettings::assignInput() {
  if(auto position = mappingList.selection()) {
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
    activeInput = controller[position()];
    setStatusText(string("Set assignment for [", activeInput->name, "] ..."));
  }
}

void InputSettings::setMapping(const char *mapping) {
  activeInput->mapping = mapping;
  activeInput = 0;
  inputMapper.bind();
  setStatusText("");
  portBox.setEnabled(true);
  deviceBox.setEnabled(true);
  mappingList.setEnabled(true);
  deviceChanged();
}

void InputSettings::inputEvent(uint16_t scancode, int16_t value) {
  if(activeInput == 0) return;  //not remapping any keys right now?

  string mapping = Scancode::encode(scancode);
  if(auto position = strpos(mapping, "::Escape")) return setMapping("");

  if(dynamic_cast<InputMapper::AnalogInput*>(activeInput)) {
  } else if(dynamic_cast<InputMapper::DigitalInput*>(activeInput)) {
    if(Keyboard::isAnyKey(scancode) && value) {
      setMapping(mapping);
    } else if(Joypad::isAnyHat(scancode) && value) {
      if(value == Joypad::HatUp) setMapping(string(mapping, ".Up"));
      else if(value == Joypad::HatDown) setMapping(string(mapping, ".Down"));
      else if(value == Joypad::HatLeft) setMapping(string(mapping, ".Left"));
      else if(value == Joypad::HatRight) setMapping(string(mapping, ".Right"));
    } else if(Joypad::isAnyButton(scancode) && value) {
      setMapping(mapping);
    }
  }
}
