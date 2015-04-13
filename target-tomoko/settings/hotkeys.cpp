HotkeySettings::HotkeySettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Keyboard);
  setText("Hotkeys");

  layout.setMargin(5);
  mappingList.setHeaderVisible();
  mappingList.onActivate([&] { assignMapping(); });
  mappingList.onChange([&] {
    eraseButton.setEnabled((bool)mappingList.selected());
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
  mappingList.append(ListViewColumn().setText("Name"));
  mappingList.append(ListViewColumn().setText("Mapping").setWidth(~0));
  mappingList.append(ListViewColumn().setText("Device"));
  for(auto& hotkey : inputManager->hotkeys) {
    mappingList.append(ListViewItem().setText(0, hotkey->name));
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::refreshMappings() -> void {
  unsigned position = 0;
  for(auto& hotkey : inputManager->hotkeys) {
    auto path = hotkey->assignment.split("/");
    string assignment = path.takeLast();
    string device = path(0);
    mappingList.item(position++)->setText(1, assignment).setText(2, device);
  }
  mappingList.resizeColumns();
}

auto HotkeySettings::assignMapping() -> void {
  inputManager->poll();  //clear any pending events first

  if(auto item = mappingList.selected()) {
    activeMapping = inputManager->hotkeys[item->offset()];
    settingsManager->statusBar.setText({"Press a key or button to map [", activeMapping->name, "] ..."});
  }
}

auto HotkeySettings::inputEvent(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> void {
  if(!activeMapping) return;
  if(!device.isKeyboard() || oldValue != 0 || newValue != 1) return;

  if(activeMapping->bind(device, group, input, oldValue, newValue)) {
    activeMapping = nullptr;
    settingsManager->statusBar.setText("");
    refreshMappings();
  }
}
