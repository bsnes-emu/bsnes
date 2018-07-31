auto InputSettings::create() -> void {
  setIcon(Icon::Device::Joypad);
  setText("Input");

  layout.setPadding(5);
  portLabel.setText("Port:");
  portList.onChange([&] { reloadDevices(); });
  deviceLabel.setText("Device:");
  deviceList.onChange([&] { reloadMappings(); });
  turboLabel.setText("Turbo rate:");
  for(uint frequency : range(1, 9)) {
    ComboButtonItem item{&turboList};
    item.setText(frequency);
    if(frequency == settings["Input/Turbo/Frequency"].natural()) item.setSelected();
  }
  turboList.onChange([&] {
    uint frequency = turboList.selected().text().natural();
    settings["Input/Turbo/Frequency"].setValue(frequency);
    inputManager.turboCounter = 0;
    inputManager.turboFrequency = frequency;
  });
  mappingList.setBatchable();
  mappingList.onActivate([&] { if(assignButton.enabled()) assignButton.doActivate(); });
  mappingList.onChange([&] { updateControls(); });
  assignMouse1.onActivate([&] { assignMouseInput(0); });
  assignMouse2.onActivate([&] { assignMouseInput(1); });
  assignMouse3.onActivate([&] { assignMouseInput(2); });
  assignButton.setText("Assign").onActivate([&] {
    assignMapping();
  });
  clearButton.setText("Clear").onActivate([&] {
    for(auto mapping : mappingList.batched()) {
      activeDevice().mappings[mapping.offset()].unbind();
    }
    refreshMappings();
  });
}

auto InputSettings::updateControls() -> void {
  auto batched = mappingList.batched();
  assignButton.setEnabled(batched.size() == 1);
  clearButton.setEnabled(batched.size() >= 1);
  assignMouse1.setVisible(false);
  assignMouse2.setVisible(false);
  assignMouse3.setVisible(false);

  if(batched.size() == 1) {
    auto& input = activeDevice().mappings[batched.left().offset()];
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

auto InputSettings::activePort() -> InputPort& {
  return inputManager.ports[portList.selected().offset()];
}

auto InputSettings::activeDevice() -> InputDevice& {
  auto index = deviceList.selected().property("index").natural();
  return activePort().devices[index];
}

auto InputSettings::reloadPorts() -> void {
  portList.reset();
  for(auto& port : inputManager.ports) {
    if(port.name == "Expansion Port") continue;
    portList.append(ComboButtonItem().setText(port.name));
  }
  reloadDevices();
}

auto InputSettings::reloadDevices() -> void {
  deviceList.reset();
  uint index = 0;
  for(auto& device : activePort().devices) {
    if(device.mappings) {  //only display devices that have configurable inputs
      deviceList.append(ComboButtonItem().setText(device.name).setProperty("index", index));
    }
    index++;
  }
  reloadMappings();
}

auto InputSettings::reloadMappings() -> void {
  mappingList.reset();
  mappingList.append(TableViewHeader().setVisible()
    .append(TableViewColumn().setText("Name"))
    .append(TableViewColumn().setText("Mapping").setExpandable())
  );
  for(auto& mapping : activeDevice().mappings) {
    mappingList.append(TableViewItem()
      .append(TableViewCell().setText(mapping.name).setFont(Font().setBold()).setBackgroundColor({240, 240, 255}))
      .append(TableViewCell())
    );
  }
  refreshMappings();
  updateControls();
}

auto InputSettings::refreshMappings() -> void {
  uint index = 0;
  for(auto& mapping : activeDevice().mappings) {
    mappingList.item(index++).cell(1).setText(mapping.displayName());
  }
  Application::processEvents();
  mappingList.resizeColumns();
}

auto InputSettings::assignMapping() -> void {
  inputManager.poll();  //clear any pending events first

  for(auto mapping : mappingList.batched()) {
    activeMapping = activeDevice().mappings[mapping.offset()];
    settingsWindow.layout.setEnabled(false);
    settingsWindow.statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
    settingsWindow.setDismissable(false);
  }
}

auto InputSettings::cancelMapping() -> void {
  activeMapping.reset();
  settingsWindow.statusBar.setText();
  settingsWindow.layout.setEnabled();
  settingsWindow.doSize();
  settingsWindow.setDismissable(true);
}

auto InputSettings::assignMouseInput(uint id) -> void {
  if(auto mapping = mappingList.selected()) {
    activeMapping = activeDevice().mappings[mapping.offset()];
    if(auto mouse = inputManager.findMouse()) {
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
    activeMapping.reset();
    settingsWindow.statusBar.setText("Mapping assigned.");
    refreshMappings();
    timer.onActivate([&] {
      timer.setEnabled(false);
      cancelMapping();
    }).setInterval(200).setEnabled();
  }
}
