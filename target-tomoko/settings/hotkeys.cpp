HotkeySettings::HotkeySettings(TabFrame* parent) : TabFrameItem(parent) {
  setImage(Icon::Device::Keyboard);
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
      inputManager->hotkeys[item->offset()]->unbind();
      refreshMappings();
    }
  });

  reloadMappings();
  refreshMappings();
}

auto HotkeySettings::reloadMappings() -> void {
  mappingList.reset();
  mappingList.append(ListViewHeader().setVisible()
    .append(ListViewColumn().setText("Name"))
    .append(ListViewColumn().setText("Mapping").setExpandable())
    .append(ListViewColumn().setText("Device"))
  );
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.append(ListViewItem()
      .append(ListViewCell().setText(hotkey->name))
      .append(ListViewCell())
      .append(ListViewCell())
    );
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::refreshMappings() -> void {
  unsigned position = 0;
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.item(position)->cell(1)->setText(hotkey->assignmentName());
    mappingList.item(position)->cell(2)->setText(hotkey->deviceName());
    position++;
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  if(auto item = mappingList.selected()) {
    activeMapping = inputManager->hotkeys[item->offset()];
    settingsManager->layout.setEnabled(false);
    settingsManager->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
  }
}

auto HotkeySettings::inputEvent(shared_pointer<HID::Device> device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> void {
  if(!activeMapping) return;
  if(device->isMouse()) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue)) {
    activeMapping = nullptr;
    settingsManager->statusBar.setText("");
    settingsManager->layout.setEnabled(true);
    refreshMappings();
  }
}
