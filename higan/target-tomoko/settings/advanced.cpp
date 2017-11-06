AdvancedSettings::AdvancedSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Action::Settings);
  setText("Advanced");

  layout.setMargin(5);

  driverLabel.setText("Driver Selection").setFont(Font().setBold());
  videoLabel.setText("Video:");
  videoDriver.onChange([&] { settings["Video/Driver"].setValue(videoDriver.selected().text()); });
  for(auto& driver : Video::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    videoDriver.append(item);
    if(settings["Video/Driver"].text() == driver) item.setSelected();
  }
  audioLabel.setText("Audio:");
  audioDriver.onChange([&] { settings["Audio/Driver"].setValue(audioDriver.selected().text()); });
  for(auto& driver : Audio::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    audioDriver.append(item);
    if(settings["Audio/Driver"].text() == driver) item.setSelected();
  }
  inputLabel.setText("Input:");
  inputDriver.onChange([&] { settings["Input/Driver"].setValue(inputDriver.selected().text()); });
  for(auto& driver : Input::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    inputDriver.append(item);
    if(settings["Input/Driver"].text() == driver) item.setSelected();
  }

  libraryLabel.setText("Game Library").setFont(Font().setBold());
  libraryPrefix.setText("Location:");
  libraryLocation.setEditable(false).setText(settings["Library/Location"].text());
  libraryChange.setText("Change ...").onActivate([&] {
    if(auto location = BrowserDialog().setTitle("Select Library Location").selectFolder()) {
      settings["Library/Location"].setValue(location);
      libraryLocation.setText(location);
    }
  });

  ignoreManifests.setText("Ignore Manifests").setChecked(settings["Library/IgnoreManifests"].boolean()).onToggle([&] {
    settings["Library/IgnoreManifests"].setValue(ignoreManifests.checked());
  });

  otherLabel.setText("Other").setFont(Font().setBold());
  autoSaveRAM.setText("Auto-Save RAM Periodically").setChecked(settings["Emulation/AutoSaveRAM"].boolean()).onToggle([&] {
    settings["Emulation/AutoSaveRAM"].setValue(autoSaveRAM.checked());
  });
}
