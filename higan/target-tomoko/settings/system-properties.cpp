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
    string filters = "Games|";
    for(auto& emulator : program->emulators) {
      for(auto& media : emulator->media) {
        filters.append("*.", media.type, ":");
      }
    }
    filters.trimRight(":", 1L);
    if(auto location = BrowserDialog()
    .setTitle("Select Boot Game")
    .setPath(settings["Library/Location"].text())
    .setFilters(filters)
    .openFolder()) {
      bootEdit.setText(location);
    }
  });
  acceptButton.onActivate([&] {
    setVisible(false);
    settingsManager->systems.accept();
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
  acceptButton.setText("Append");
  setFocused();
  setVisible();
  nameEdit.setFocused();
}

auto SystemProperties::modify(Markup::Node system) -> void {
  setCentered(*settingsManager);
  nameEdit.setText(system["Name"].text());
  bootEdit.setText(system["Boot"].text());
  acceptButton.setText("Modify");
  setFocused();
  setVisible();
  nameEdit.setFocused();
}
