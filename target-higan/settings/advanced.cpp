AdvancedSettings* advancedSettings = nullptr;

AdvancedSettings::AdvancedSettings() {
  driverTitle.setFont(program->boldFont);
  driverTitle.setText("Driver Selection:");
  videoLabel.setText("Video:");
  audioLabel.setText("Audio:");
  inputLabel.setText("Input:");
  libraryTitle.setFont(program->boldFont);
  libraryTitle.setText("Game Library:");
  libraryLabel.setText("Path:");
  libraryPath.setEditable(false);
  libraryPath.setText(utility->libraryPath());
  libraryBrowse.setText("Browse ...");
  libraryShowOnStartup.setChecked(config->library.showOnStartup);
  libraryShowOnStartup.setText("Show game library on program start");
  information.setText("Note: changing advanced settings requires program restart to take effect.");
  infoLabel.setFont(program->boldFont);
  infoLabel.setText({
    Emulator::Name, " v", Emulator::Version, "\n",
    "  ", Emulator::Profile, " Profile\n",
    "  Author: ", Emulator::Author, "\n",
    "  License: ", Emulator::License, "\n",
    "  Website: ", Emulator::Website
  });

  lstring list;

  list.split(";", video.availableDrivers());
  for(unsigned n = 0; n < list.size(); n++) {
    videoDriver.append(list[n]);
    if(list[n] == config->video.driver) videoDriver.setSelection(n);
  }

  list.split(";", audio.availableDrivers());
  for(unsigned n = 0; n < list.size(); n++) {
    audioDriver.append(list[n]);
    if(list[n] == config->audio.driver) audioDriver.setSelection(n);
  }

  list.split(";", input.availableDrivers());
  for(unsigned n = 0; n < list.size(); n++) {
    inputDriver.append(list[n]);
    if(list[n] == config->input.driver) inputDriver.setSelection(n);
  }

  append(driverTitle, {~0, 0});
  append(driverLayout, {~0, 0}, 15);
    driverLayout.append(videoLabel, {0, 0}, 5);
    driverLayout.append(videoDriver, {~0, 0}, 5);
    driverLayout.append(audioLabel, {0, 0}, 5);
    driverLayout.append(audioDriver, {~0, 0}, 5);
    driverLayout.append(inputLabel, {0, 0}, 5);
    driverLayout.append(inputDriver, {~0, 0});
  append(libraryTitle, {~0, 0});
  append(libraryLayout, {~0, 0});
    libraryLayout.append(libraryLabel, {0, 0}, 5);
    libraryLayout.append(libraryPath, {~0, 0}, 5);
    libraryLayout.append(libraryBrowse, {80, 0});
  append(libraryShowOnStartup, {~0, 0}, 15);
  append(information, {~0, 0}, 15);
  if(Intrinsics::platform() != Intrinsics::Platform::MacOSX) {
    append(spacer, {~0, ~0});
    append(infoLabel, {~0, 0});
  }

  videoDriver.onChange = [&] { config->video.driver = videoDriver.text(); };
  audioDriver.onChange = [&] { config->audio.driver = audioDriver.text(); };
  inputDriver.onChange = [&] { config->input.driver = inputDriver.text(); };

  libraryBrowse.onActivate = [&] {
    string path = BrowserWindow().setParent(*settings).setPath(userpath()).directory();
    if(path.empty()) return;
    file::write({configpath(), "higan/library.bml"}, {"Path: ", path, "\n"});
    libraryPath.setText(path);
  };

  libraryShowOnStartup.onToggle = [&] {
    config->library.showOnStartup = libraryShowOnStartup.checked();
  };
}
