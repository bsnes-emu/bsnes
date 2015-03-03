AdvancedSettings::AdvancedSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Action::Settings);
  setText("Advanced");

  layout.setMargin(5);
  driverLabel.setText("Driver Selection").setFont(Font::sans(8, "Bold"));
  videoLabel.setText("Video:");
  videoDriver.onChange([&] { config().video.driver = videoDriver.selected()->text(); });
  for(auto& driver : string{video.availableDrivers()}.split(";")) {
    ComboButtonItem item;
    item.setText(driver);
    videoDriver.append(item);
    if(config().video.driver == driver) item.setSelected();
  }
  audioLabel.setText("Audio:");
  audioDriver.onChange([&] { config().audio.driver = audioDriver.selected()->text(); });
  for(auto& driver : string{audio.availableDrivers()}.split(";")) {
    ComboButtonItem item;
    item.setText(driver);
    audioDriver.append(item);
    if(config().audio.driver == driver) item.setSelected();
  }
  inputLabel.setText("Input:");
  inputDriver.onChange([&] { config().input.driver = inputDriver.selected()->text(); });
  for(auto& driver : string{input.availableDrivers()}.split(";")) {
    ComboButtonItem item;
    item.setText(driver);
    inputDriver.append(item);
    if(config().input.driver == driver) item.setSelected();
  }
}
