HotkeySettings::HotkeySettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Keyboard);
  setText("Hotkeys");

  layout.setMargin(5);
  mappingList.onActivate([&] { assignMapping(); });
  mappingList.onChange([&] {
    eraseButton.setEnabled((bool)mappingList.selected());
  });
  resetButton.setText("Reset").onActivate([&] {
    if(MessageDialog("Are you sure you want to erase all hotkey mappings?").setParent(*settingsManager).question() == "Yes") {
      for(auto& mapping : inputManager->hotkeys) mapping->unbind();
      refreshMappings();
    }
  });
  eraseButton.setText("Erase").onActivate([&] {
    if(auto item = mappingList.selected()) {
      inputManager->hotkeys[item.offset()]->unbind();
      refreshMappings();
    }
  });

  reloadMappings();
  refreshMappings();
}

auto HotkeySettings::reloadMappings() -> void {
  mappingList.reset();
  mappingList.append(TableViewHeader().setVisible()
    .append(TableViewColumn().setText("Name"))
    .append(TableViewColumn().setText("Mapping").setExpandable())
    .append(TableViewColumn().setText("Device").setAlignment(1.0).setForegroundColor({0, 128, 0}))
  );
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.append(TableViewItem()
      .append(TableViewCell().setText(hotkey->name))
      .append(TableViewCell())
      .append(TableViewCell())
    );
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::refreshMappings() -> void {
  uint position = 0;
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.item(position).cell(1).setText(hotkey->assignmentName());
    mappingList.item(position).cell(2).setText(hotkey->deviceName());
    position++;
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  if(auto item = mappingList.selected()) {
    activeMapping = inputManager->hotkeys[item.offset()];
    settingsManager->layout.setEnabled(false);
    settingsManager->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
  }
}

auto HotkeySettings::inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void {
  if(!activeMapping) return;
  if(device->isMouse()) return;

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
