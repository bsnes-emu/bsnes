InputSettings::InputSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Joypad);
  setText("Input");

  layout.setMargin(5);
  for(auto& emulator : inputManager->emulators) {
    emulatorList.append(ComboButtonItem().setText(emulator.name));
  }
  emulatorList.onChange([&] { reloadPorts(); });
  portList.onChange([&] { reloadDevices(); });
  deviceList.onChange([&] { reloadMappings(); });
  mappingList.setHeaderVisible();
  mappingList.onActivate([&] { assignMapping(); });
  mappingList.onChange([&] {
    eraseButton.setEnabled((bool)mappingList.selected());
  });
  resetButton.setText("Reset").onActivate([&] {
    if(MessageDialog("Are you sure you want to erase all mappings for this device?").setParent(*settingsManager).question() == 0) {
      for(auto& mapping : activeDevice().mappings) mapping->unbind();
      refreshMappings();
    }
  });
  eraseButton.setText("Erase").onActivate([&] {
    if(auto mapping = mappingList.selected()) {
      activeDevice().mappings[mapping->offset()]->unbind();
      refreshMappings();
    }
  });

  reloadPorts();
}

auto InputSettings::activeEmulator() -> InputEmulator& {
  return inputManager->emulators[emulatorList.selected()->offset()];
}

auto InputSettings::activePort() -> InputPort& {
  return activeEmulator().ports[portList.selected()->offset()];
}

auto InputSettings::activeDevice() -> InputDevice& {
  return activePort().devices[deviceList.selected()->offset()];
}

auto InputSettings::reloadPorts() -> void {
  portList.reset();
  for(auto& port : activeEmulator().ports) {
    portList.append(ComboButtonItem().setText(port.name));
  }
  reloadDevices();
}

auto InputSettings::reloadDevices() -> void {
  deviceList.reset();
  for(auto& device : activePort().devices) {
    deviceList.append(ComboButtonItem().setText(device.name));
  }
  reloadMappings();
}

auto InputSettings::reloadMappings() -> void {
  eraseButton.setEnabled(false);
  mappingList.reset();
  mappingList.append(ListViewColumn().setText("Name"));
  mappingList.append(ListViewColumn().setText("Mapping").setWidth(~0));
  mappingList.append(ListViewColumn().setText("Device").setForegroundColor({0, 128, 0}));
  for(auto& mapping : activeDevice().mappings) {
    mappingList.append(ListViewItem().setText(0, mapping->name));
  }
  refreshMappings();
}

auto InputSettings::refreshMappings() -> void {
  unsigned position = 0;
  for(auto& mapping : activeDevice().mappings) {
    auto path = mapping->assignment.split("/");
    string assignment = path.takeLast();
    string device = path(0);
    mappingList.item(position++)->setText(1, assignment).setText(2, device);
  }
  mappingList.resizeColumns();
}

auto InputSettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  auto item = mappingList.selected();
  activeMapping = activeDevice().mappings[item->offset()];

//settingsManager->layout.setEnabled(false);
  settingsManager->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
}

auto InputSettings::inputEvent(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> void {
  if(!activeMapping) return;
  if(!device.isKeyboard() || oldValue != 0 || newValue != 1) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue)) {
    activeMapping = nullptr;
    settingsManager->statusBar.setText("");
  //settingsManager->layout.setEnabled(true);  //todo: this isn't enabling child widgets properly (bug in hiro)
    refreshMappings();
  }
}
