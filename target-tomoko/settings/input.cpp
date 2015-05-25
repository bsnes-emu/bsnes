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
  mappingList.onChange([&] { updateControls(); });
  assignMouse1.setVisible(false).onActivate([&] { assignMouseInput(0); });
  assignMouse2.setVisible(false).onActivate([&] { assignMouseInput(1); });
  assignMouse3.setVisible(false).onActivate([&] { assignMouseInput(2); });
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

auto InputSettings::updateControls() -> void {
  eraseButton.setEnabled((bool)mappingList.selected());
  assignMouse1.setVisible(false);
  assignMouse2.setVisible(false);
  assignMouse3.setVisible(false);

  if(auto mapping = mappingList.selected()) {
    auto input = activeDevice().mappings[mapping->offset()];

    if(input->isDigital()) {
      assignMouse1.setVisible().setText("Mouse Left");
      assignMouse2.setVisible().setText("Mouse Middle");
      assignMouse3.setVisible().setText("Mouse Right");
    } else if(input->isAnalog()) {
      assignMouse1.setVisible().setText("Mouse X-axis");
      assignMouse2.setVisible().setText("Mouse Y-axis");
    }
  }
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
    mappingList.item(position++)->setText(1, mapping->assignmentName()).setText(2, mapping->deviceName());
  }
  mappingList.resizeColumns();
}

auto InputSettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  if(auto mapping = mappingList.selected()) {
    activeMapping = activeDevice().mappings[mapping->offset()];
    settingsManager->layout.setEnabled(false);
    settingsManager->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
  }
}

auto InputSettings::assignMouseInput(unsigned id) -> void {
  if(auto mouse = inputManager->findMouse()) {
    if(auto mapping = mappingList.selected()) {
      activeMapping = activeDevice().mappings[mapping->offset()];

      if(activeMapping->isDigital()) {
        return inputEvent(mouse, HID::Mouse::GroupID::Button, id, 0, 1, true);
      } else if(activeMapping->isAnalog()) {
        return inputEvent(mouse, HID::Mouse::GroupID::Axis, id, 0, +32767, true);
      }
    }
  }
}

auto InputSettings::inputEvent(shared_pointer<HID::Device> device, unsigned group, unsigned input, int16 oldValue, int16 newValue, bool allowMouseInput) -> void {
  if(!activeMapping) return;
  if(device->isMouse() && !allowMouseInput) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue)) {
    activeMapping = nullptr;
    settingsManager->statusBar.setText("");
    settingsManager->layout.setEnabled(true);
    refreshMappings();
  }
}
