SettingsDialog::SettingsDialog() {
  settingsDialog = this;

  layout.setMargin(5);
  locationLabel.setText("Library Location:");
  locationEdit.setEditable(false).setText(settings["Library/Location"].text());
  changeLocationButton.setText("Change ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*this).setTitle("Select Library Location").selectFolder()) {
      settings["Library/Location"].setValue(location);
      locationEdit.setText(location);
    }
  });
  createManifestsOption.setText("Create Manifests (not recommended; breaks backward-compatibility)")
  .setChecked(settings["icarus/CreateManifests"].boolean()).onToggle([&] {
    settings["icarus/CreateManifests"].setValue(createManifestsOption.checked());
  });
  useDatabaseOption.setText("Use Database (highly recommended; provides bit-perfect memory mapping)")
  .setChecked(settings["icarus/UseDatabase"].boolean()).onToggle([&] {
    settings["icarus/UseDatabase"].setValue(useDatabaseOption.checked());
  });

  setTitle("icarus Settings");
  setSize({480, layout.minimumSize().height()});
}
