HotkeySettings::HotkeySettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Keyboard);
  setText("Hotkeys");

  layout.setMargin(5);
  mappingList.onActivate([&] { assignMapping(); });
  mappingList.onChange([&] { eraseButton.setEnabled((bool)mappingList.selected()); });
  resetButton.setText("Reset").onActivate([&] {
    if(MessageDialog("Are you sure you want to erase all hotkey mappings?").setParent(*settingsWindow).question() == "Yes") {
      for(auto& mapping : inputManager->hotkeys) mapping.unbind();
      refreshMappings();
    }
  });
  eraseButton.setText("Erase").onActivate([&] {
    if(auto item = mappingList.selected()) {
      inputManager->hotkeys[item.offset()].unbind();
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
  );
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.append(TableViewItem()
      .append(TableViewCell().setText(hotkey.name))
      .append(TableViewCell())
    );
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::refreshMappings() -> void {
  uint index = 0;
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.item(index++).cell(1).setText(hotkey.displayName());
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  if(auto item = mappingList.selected()) {
    activeMapping = inputManager->hotkeys[item.offset()];
    settingsWindow->layout.setEnabled(false);
    settingsWindow->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
  }
}

auto HotkeySettings::inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void {
  if(!activeMapping) return;
  if(device->isMouse()) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue)) {
    activeMapping.reset();
    settingsWindow->statusBar.setText("Mapping assigned.");
    refreshMappings();
    timer.onActivate([&] {
      timer.setEnabled(false);
      settingsWindow->statusBar.setText();
      settingsWindow->layout.setEnabled();
    }).setInterval(200).setEnabled();
  }
}
