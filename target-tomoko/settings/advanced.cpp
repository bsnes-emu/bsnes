AdvancedSettings::AdvancedSettings(TabFrame* parent) : TabFrameItem(parent) {
  setImage(Icon::Action::Settings);
  setText("Advanced");

  layout.setMargin(5);

  driverLabel.setText("Driver Selection").setFont(Font().setBold());
  videoLabel.setText("Video:");
  videoDriver.onChange([&] { config->video.driver = videoDriver.selected()->text(); });
  for(auto& driver : Video::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    videoDriver.append(item);
    if(config->video.driver == driver) item.setSelected();
  }
  audioLabel.setText("Audio:");
  audioDriver.onChange([&] { config->audio.driver = audioDriver.selected()->text(); });
  for(auto& driver : Audio::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    audioDriver.append(item);
    if(config->audio.driver == driver) item.setSelected();
  }
  inputLabel.setText("Input:");
  inputDriver.onChange([&] { config->input.driver = inputDriver.selected()->text(); });
  for(auto& driver : Input::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    inputDriver.append(item);
    if(config->input.driver == driver) item.setSelected();
  }

  libraryLabel.setText("Game Library").setFont(Font().setBold());
  libraryPrefix.setText("Location:");
  libraryLocation.setEditable(false).setText(config->library.location);
  libraryChange.setText("Change ...").onActivate([&] {
    if(auto location = BrowserDialog().setTitle("Select Library Location").selectFolder()) {
      libraryLocation.setText(config->library.location = location);
    }
  });
}
