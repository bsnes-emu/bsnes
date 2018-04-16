SystemProperties::SystemProperties() {
  systemProperties = this;

  layout.setMargin(5);
  nameLabel.setText("Name:");
  bootLabel.setText("Boot:");
  bootEdit.setEditable(false);
  for(auto& emulator : program->emulators) {
    bootEdit.append(ComboEditItem().setText(emulator->information.name));
  }
  bootBrowse.setText("Browse ...").onActivate([&] {
    if(auto location = BrowserDialog().setTitle("Select Boot Game").setPath(settings["Library/Location"].text()).selectFolder()) {
      bootEdit.setText(location);
    }
  });
  hiddenOption.setText("Hidden");
  acceptButton.onActivate([&] {
    setVisible(false);
    settingsManager->systems.acceptProperties();
  });
  cancelButton.setText("Cancel").onActivate([&] {
    setVisible(false);
  });

  setTitle("System Properties");
  setSize({480, layout.minimumSize().height()});
  setDismissable();
}

auto SystemProperties::append() -> void {
  setCentered(*settingsManager);
  nameEdit.setText("");
  bootEdit.setText("");
  hiddenOption.setChecked(false);
  acceptButton.setText("Append");
  setFocused();
  setVisible();
  nameEdit.setFocused();
}

auto SystemProperties::modify(Markup::Node system) -> void {
  setCentered(*settingsManager);
  nameEdit.setText(system["Name"].text());
  bootEdit.setText(system["Boot"].text());
  hiddenOption.setChecked(system["Hidden"].boolean());
  acceptButton.setText("Modify");
  setFocused();
  setVisible();
  nameEdit.setFocused();
}
