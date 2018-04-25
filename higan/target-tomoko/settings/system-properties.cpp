SystemProperties::SystemProperties() {
  systemProperties = this;

  layout.setMargin(5);
  systemLabel.setAlignment(1.0).setText("System:");
  for(auto& emulator : program->emulators) {
    systemOption.append(ComboButtonItem().setText(emulator->information.name));
  }
  loadLabel.setAlignment(1.0).setText("Load:");
  loadEdit.setEditable(false);
  loadBrowse.setText("Browse ...").onActivate([&] {
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
      loadEdit.setText(location);
    }
  });
  aliasLabel.setAlignment(1.0).setText("Alias:");
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
  systemOption.item(0).setSelected();
  loadEdit.setText("");
  aliasEdit.setText("");
  acceptButton.setText("Append");
  setFocused();
  setVisible();
  systemOption.setFocused();
}

auto SystemProperties::modify(Markup::Node system) -> void {
  setCentered(*settingsManager);
  systemOption.item(0).setSelected();
  for(auto item : systemOption.items()) {
    if(item.text() == system.text()) item.setSelected();
  }
  loadEdit.setText(system["Load"].text());
  aliasEdit.setText(system["Alias"].text());
  acceptButton.setText("Modify");
  setFocused();
  setVisible();
  systemOption.setFocused();
}
