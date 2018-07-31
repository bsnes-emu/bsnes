auto HotkeySettings::create() -> void {
  setIcon(Icon::Device::Keyboard);
  setText("Hotkeys");

  layout.setPadding(5);
  mappingList.setBatchable();
  mappingList.onActivate([&] {
    if(assignButton.enabled()) assignButton.doActivate();
  });
  mappingList.onChange([&] {
    auto batched = mappingList.batched();
    assignButton.setEnabled(batched.size() == 1);
    clearButton.setEnabled(batched.size() >= 1);
  });
  assignButton.setText("Assign").onActivate([&] {
    assignMapping();
  });
  clearButton.setText("Clear").onActivate([&] {
    for(auto item : mappingList.batched()) {
      inputManager.hotkeys[item.offset()].unbind();
    }
    refreshMappings();
  });
}

auto HotkeySettings::reloadMappings() -> void {
  mappingList.reset();
  mappingList.append(TableViewHeader().setVisible()
    .append(TableViewColumn().setText("Name"))
    .append(TableViewColumn().setText("Mapping").setExpandable())
  );
  for(auto& hotkey : inputManager.hotkeys) {
    mappingList.append(TableViewItem()
      .append(TableViewCell().setText(hotkey.name).setFont(Font().setBold()).setBackgroundColor({240, 240, 255}))
      .append(TableViewCell())
    );
  }
  refreshMappings();
  mappingList.doChange();
}

auto HotkeySettings::refreshMappings() -> void {
  uint index = 0;
  for(auto& hotkey : inputManager.hotkeys) {
    mappingList.item(index++).cell(1).setText(hotkey.displayName());
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::assignMapping() -> void {
  inputManager.poll();  //clear any pending events first

  if(auto item = mappingList.selected()) {
    activeMapping = inputManager.hotkeys[item.offset()];
    settingsWindow.layout.setEnabled(false);
    settingsWindow.statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
    settingsWindow.setDismissable(false);
  }
}

auto HotkeySettings::cancelMapping() -> void {
  activeMapping.reset();
  settingsWindow.statusBar.setText();
  settingsWindow.layout.setEnabled();
  settingsWindow.doSize();
  settingsWindow.setDismissable(true);
}

auto HotkeySettings::inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void {
  if(!activeMapping) return;
  if(device->isMouse()) return;

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
