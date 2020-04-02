auto InputSettings::create() -> void {
  setCollapsible();
  setVisible(false);

  inputFocusLabel.setText("When focus is lost:");
  pauseEmulation.setText("Pause emulation").onActivate([&] {
    settings.input.defocus = "Pause";
  });
  blockInput.setText("Block input").onActivate([&] {
    settings.input.defocus = "Block";
  });
  allowInput.setText("Allow input").onActivate([&] {
    settings.input.defocus = "Allow";
  });
  if(settings.input.defocus == "Pause") pauseEmulation.setChecked();
  if(settings.input.defocus == "Block") blockInput.setChecked();
  if(settings.input.defocus == "Allow") allowInput.setChecked();
  separator.setColor({192, 192, 192});

  portLabel.setText("Port:");
  portList.onChange([&] { reloadDevices(); });
  deviceLabel.setText("Device:");
  deviceList.onChange([&] { reloadMappings(); });
  turboLabel.setText("Turbo rate:").setToolTip(
    "The number of frames to wait between toggling turbo buttons."
  );
  for(uint frequency : range(1, 9)) {
    ComboButtonItem item{&turboList};
    item.setText(frequency);
    if(frequency == settings.input.turbo.frequency) item.setSelected();
  }
  turboList.setToolTip(turboLabel.toolTip()).onChange([&] {
    uint frequency = turboList.selected().text().natural();
    settings.input.turbo.frequency = frequency;
    inputManager.turboCounter = 0;
    inputManager.turboFrequency = frequency;
  });
  mappingList.setBatchable();
  mappingList.setHeadered();
  mappingList.onActivate([&](auto cell) { assignMapping(cell); });
  mappingList.onChange([&] { updateControls(); });
  mappingList.onSize([&] { mappingList.resizeColumns(); });
  inputSink.setFocusable();
  assignMouse1.onActivate([&] { assignMouseInput(0); });
  assignMouse2.onActivate([&] { assignMouseInput(1); });
  assignMouse3.onActivate([&] { assignMouseInput(2); });
  assignButton.setText("Assign").onActivate([&] {
    clearButton.doActivate();
    assignMapping(mappingList.selected().cell(0));
  });
  clearButton.setText("Clear").onActivate([&] {
    cancelMapping();
    for(auto mapping : mappingList.batched()) {
      for(uint index : range(BindingLimit)) {
        activeDevice().mappings[mapping.offset()].unbind(index);
      }
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

  if(activeMapping) {
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

  controlLayout.resize();
}

auto InputSettings::activePort() -> InputPort& {
  return inputManager.ports[portList.selected().offset()];
}

auto InputSettings::activeDevice() -> InputDevice& {
  auto index = deviceList.selected().attribute("index").natural();
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
      deviceList.append(ComboButtonItem().setText(device.name).setAttribute("index", index));
    }
    index++;
  }
  reloadMappings();
}

auto InputSettings::reloadMappings() -> void {
  mappingList.reset();
  mappingList.append(TableViewColumn().setText("Name"));
  for(uint n : range(BindingLimit)) {
    mappingList.append(TableViewColumn().setText({"Mapping #", 1 + n}).setExpandable());
  }
  for(auto& mapping : activeDevice().mappings) {
    TableViewItem item{&mappingList};
    item.append(TableViewCell().setText(mapping.name).setFont(Font().setBold()));
    for(uint n : range(BindingLimit)) item.append(TableViewCell());
  }
  refreshMappings();
  cancelMapping();
}

auto InputSettings::refreshMappings() -> void {
  uint item = 0;
  for(auto& mapping : activeDevice().mappings) {
    for(uint index : range(BindingLimit)) {
      auto cell = mappingList.item(item).cell(1 + index);
      auto& binding = mapping.bindings[index];
      cell.setIcon(binding.icon());
      cell.setText(binding.name());
    }
    item++;
  }
  Application::processEvents();
  mappingList.resizeColumns();
}

auto InputSettings::assignMapping(TableViewCell cell) -> void {
  inputManager.poll();  //clear any pending events first

  for(auto mapping : mappingList.batched()) {
    activeMapping = activeDevice().mappings[mapping.offset()];
    activeBinding = max(0, (int)cell.offset() - 1);
    mappingList.item(mapping.offset()).cell(1 + activeBinding).setIcon(Icon::Go::Right).setText("(assign ...)");
    settingsWindow.statusBar.setText({"Press a key or button for mapping #", 1 + activeBinding, " [", activeMapping->name, "] ..."});
    settingsWindow.setDismissable(false);
    updateControls();
    Application::processEvents();
    inputSink.setFocused();
    return;  //map only one input at a time
  }
}

auto InputSettings::cancelMapping() -> void {
  activeMapping.reset();
  settingsWindow.statusBar.setText();
  settingsWindow.doSize();
  settingsWindow.setDismissable(true);
  mappingList.setFocused();
  updateControls();
}

auto InputSettings::assignMouseInput(uint id) -> void {
  if(!activeMapping) return;
  if(auto mouse = inputManager.findMouse()) {
    if(activeMapping->isDigital()) {
      return inputEvent(mouse, HID::Mouse::GroupID::Button, id, 0, 1, true);
    } else if(activeMapping->isAnalog()) {
      return inputEvent(mouse, HID::Mouse::GroupID::Axis, id, 0, +32767, true);
    }
  }
}

auto InputSettings::inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, bool allowMouseInput) -> void {
  if(!activeMapping) return;
  if(device->isMouse() && !allowMouseInput) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue, activeBinding)) {
    activeMapping.reset();
    settingsWindow.statusBar.setText("Mapping assigned.");
    refreshMappings();
    timer.onActivate([&] {
      timer.setEnabled(false);
      cancelMapping();
    }).setInterval(200).setEnabled();
  }
}
