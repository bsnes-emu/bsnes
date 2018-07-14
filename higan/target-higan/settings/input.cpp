InputSettings::InputSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Joypad);
  setText("Input");

  layout.setPadding(5);
  defocusLabel.setText("When Focus is Lost:");
  pauseEmulation.setText("Pause Emulation").onActivate([&] {
    settings["Input/Defocus"].setValue("Pause");
    allowInput.setEnabled(!pauseEmulation.checked());
  });
  blockInput.setText("Block Input").onActivate([&] {
    settings["Input/Defocus"].setValue("Block");
  });
  allowInput.setText("Allow Input").onActivate([&] {
    settings["Input/Defocus"].setValue("Allow");
  });
  if(settings["Input/Defocus"].text() == "Pause") pauseEmulation.setChecked();
  if(settings["Input/Defocus"].text() == "Block") blockInput.setChecked();
  if(settings["Input/Defocus"].text() == "Allow") allowInput.setChecked();
  for(auto& emulator : inputManager->emulators) {
    emulatorList.append(ComboButtonItem().setText(emulator.name));
  }
  emulatorList.onChange([&] { reloadPorts(); });
  portList.onChange([&] { reloadDevices(); });
  deviceList.onChange([&] { reloadMappings(); });
  mappingList.onActivate([&] { assignMapping(); });
  mappingList.onChange([&] { updateControls(); });
  assignMouse1.setVisible(false).onActivate([&] { assignMouseInput(0); });
  assignMouse2.setVisible(false).onActivate([&] { assignMouseInput(1); });
  assignMouse3.setVisible(false).onActivate([&] { assignMouseInput(2); });
  resetButton.setText("Reset").onActivate([&] {
    if(MessageDialog("Are you sure you want to erase all mappings for this device?").setParent(*settingsManager).question() == "Yes") {
      for(auto& mapping : activeDevice().mappings) mapping.unbind();
      refreshMappings();
    }
  });
  eraseButton.setText("Erase").onActivate([&] {
    if(auto mapping = mappingList.selected()) {
      activeDevice().mappings[mapping.offset()].unbind();
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
    auto& input = activeDevice().mappings[mapping.offset()];

    if(input.isDigital()) {
      assignMouse1.setVisible().setText("Mouse Left");
      assignMouse2.setVisible().setText("Mouse Middle");
      assignMouse3.setVisible().setText("Mouse Right");
    } else if(input.isAnalog()) {
      assignMouse1.setVisible().setText("Mouse X-axis");
      assignMouse2.setVisible().setText("Mouse Y-axis");
    }
  }
}

auto InputSettings::activeEmulator() -> InputEmulator& {
  return inputManager->emulators[emulatorList.selected().offset()];
}

auto InputSettings::activePort() -> InputPort& {
  return activeEmulator().ports[portList.selected().offset()];
}

auto InputSettings::activeDevice() -> InputDevice& {
  auto index = deviceList.selected().property("index").natural();
  return activePort().devices[index];
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
  for(auto n : range(activePort().devices.size())) {
    auto& device = activePort().devices[n];
    if(!device.mappings) continue;  //do not display devices that have no configurable inputs
    deviceList.append(ComboButtonItem().setText(device.name).setProperty("index", n));
  }
  reloadMappings();
}

auto InputSettings::reloadMappings() -> void {
  eraseButton.setEnabled(false);
  mappingList.reset();
  mappingList.append(TableViewHeader().setVisible()
    .append(TableViewColumn().setText("Name"))
    .append(TableViewColumn().setText("Mapping").setExpandable())
  );
  for(auto& mapping : activeDevice().mappings) {
    mappingList.append(TableViewItem()
      .append(TableViewCell().setText(mapping.name))
      .append(TableViewCell())
    );
  }
  refreshMappings();
}

auto InputSettings::refreshMappings() -> void {
  uint index = 0;
  for(auto& mapping : activeDevice().mappings) {
    mappingList.item(index++).cell(1).setText(mapping.displayName());
  }
  mappingList.resizeColumns();
}

auto InputSettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  if(auto mapping = mappingList.selected()) {
    activeMapping = &activeDevice().mappings[mapping.offset()];
    settingsManager->layout.setEnabled(false);
    settingsManager->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
  }
}

auto InputSettings::assignMouseInput(uint id) -> void {
  if(auto mouse = inputManager->findMouse()) {
    if(auto mapping = mappingList.selected()) {
      activeMapping = &activeDevice().mappings[mapping.offset()];

      if(activeMapping->isDigital()) {
        return inputEvent(mouse, HID::Mouse::GroupID::Button, id, 0, 1, true);
      } else if(activeMapping->isAnalog()) {
        return inputEvent(mouse, HID::Mouse::GroupID::Axis, id, 0, +32767, true);
      }
    }
  }
}

auto InputSettings::inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, bool allowMouseInput) -> void {
  if(!activeMapping) return;
  if(device->isMouse() && !allowMouseInput) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue)) {
    activeMapping = nullptr;
    settingsManager->statusBar.setText("Mapping assigned.");
    refreshMappings();
    timer.onActivate([&] {
      timer.setEnabled(false);
      settingsManager->statusBar.setText();
      settingsManager->layout.setEnabled();
    }).setInterval(200).setEnabled();
  }
}
