DriverSettings::DriverSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Place::Settings);
  setText("Drivers");

  layout.setPadding(5);

  videoLabel.setText("Video").setFont(Font().setBold());
  videoLayout.setSize({2, 2});
  videoDriverLabel.setText("Driver:");
  videoDriverOption.onChange([&] {
    videoDriverUpdate.setEnabled(videoDriverOption.selected().text() != video.driverName());
  });
  videoDriverUpdate.setText("Change").onActivate([&] { videoDriverChange(); });
  videoFormatLabel.setText("Format:");
  videoFormatOption.onChange([&] { videoFormatChange(); });
  videoExclusiveToggle.setText("Exclusive fullscreen").onToggle([&] {
    settings["Video/Exclusive"].setValue(videoExclusiveToggle.checked());
    program->updateVideoExclusive();
  });
  videoBlockingToggle.setText("Synchronize").onToggle([&] {
    settings["Video/Blocking"].setValue(videoBlockingToggle.checked());
    program->updateVideoBlocking();
  });
  videoFlushToggle.setText("GPU sync").onToggle([&] {
    settings["Video/Flush"].setValue(videoFlushToggle.checked());
    program->updateVideoFlush();
  });

  audioLabel.setText("Audio").setFont(Font().setBold());
  audioLayout.setSize({2, 2});
  audioDriverLabel.setText("Driver:");
  audioDriverOption.onChange([&] {
    audioDriverUpdate.setEnabled(audioDriverOption.selected().text() != audio.driverName());
  });
  audioDriverUpdate.setText("Change").onActivate([&] { audioDriverChange(); });
  audioDeviceLabel.setText("Device:");
  audioDeviceOption.onChange([&] { audioDeviceChange(); });
  audioFrequencyLabel.setText("Frequency:");
  audioFrequencyOption.onChange([&] { audioFrequencyChange(); });
  audioLatencyLabel.setText("Latency:");
  audioLatencyOption.onChange([&] { audioLatencyChange(); });
  audioExclusiveToggle.setText("Exclusive").onToggle([&] {
    settings["Audio/Exclusive"].setValue(audioExclusiveToggle.checked());
    program->updateAudioExclusive();
  });
  audioBlockingToggle.setText("Synchronize").onToggle([&] {
    settings["Audio/Blocking"].setValue(audioBlockingToggle.checked());
    program->updateAudioBlocking();
  });
  audioDynamicToggle.setText("Dynamic rate").onToggle([&] {
    settings["Audio/Dynamic"].setValue(audioDynamicToggle.checked());
    program->updateAudioDynamic();
  });

  inputLabel.setText("Input").setFont(Font().setBold());
  inputLayout.setSize({2, 1});
  inputDriverLabel.setText("Driver:");
  inputDriverOption.onChange([&] {
    inputDriverUpdate.setEnabled(inputDriverOption.selected().text() != input.driverName());
  });
  inputDriverUpdate.setText("Change").onActivate([&] { inputDriverChange(); });

  //hide video format for simplicity, as it's not very useful just yet ...
  videoLayout.setSize({2, 1});
  videoLayout.remove(videoFormatLabel);
  videoLayout.remove(videoPropertyLayout);
}

//

auto DriverSettings::videoDriverChanged() -> void {
  videoDriverOption.reset();
  for(auto& driver : video.hasDrivers()) {
    ComboButtonItem item{&videoDriverOption};
    item.setText(driver);
    if(driver == video.driverName()) item.setSelected();
  }
  videoDriverActive.setText({"Active driver: ", video.driverName()});
  videoDriverOption.doChange();
  videoFormatChanged();
  videoExclusiveToggle.setChecked(video.exclusive()).setEnabled(video.hasExclusive());
  videoBlockingToggle.setChecked(video.blocking()).setEnabled(video.hasBlocking());
  videoFlushToggle.setChecked(video.flush()).setEnabled(video.hasFlush());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::videoDriverChange() -> void {
  auto item = videoDriverOption.selected();
  settings["Video/Driver"].setValue(item.text());
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the video driver change now anyway?"
  ).setParent(*settingsWindow).question() == "Yes") {
    program->save();
    program->saveUndoState();
    settings["Crashed"].setValue(true);
    settings.save();
    program->updateVideoDriver(*settingsWindow);
    settings["Crashed"].setValue(false);
    settings.save();
    videoDriverChanged();
  }
}

auto DriverSettings::videoFormatChanged() -> void {
  videoFormatOption.reset();
  for(auto& format : video.hasFormats()) {
    ComboButtonItem item{&videoFormatOption};
    item.setText(format);
    if(format == video.format()) item.setSelected();
  }
//videoFormatOption.setEnabled(video.hasFormat());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::videoFormatChange() -> void {
  auto item = videoFormatOption.selected();
  settings["Video/Format"].setValue(item.text());
  video.setFormat(item.text());
}

//

auto DriverSettings::audioDriverChanged() -> void {
  audioDriverOption.reset();
  for(auto& driver : audio.hasDrivers()) {
    ComboButtonItem item{&audioDriverOption};
    item.setText(driver);
    if(driver == audio.driverName()) item.setSelected();
  }
  audioDriverActive.setText({"Active driver: ", audio.driverName()});
  audioDriverOption.doChange();
  audioDeviceChanged();
  audioFrequencyChanged();
  audioLatencyChanged();
  audioExclusiveToggle.setChecked(audio.exclusive()).setEnabled(audio.hasExclusive());
  audioBlockingToggle.setChecked(audio.blocking()).setEnabled(audio.hasBlocking());
  audioDynamicToggle.setChecked(audio.dynamic()).setEnabled(audio.hasDynamic());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::audioDriverChange() -> void {
  auto item = audioDriverOption.selected();
  settings["Audio/Driver"].setValue(item.text());
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the audio driver change now anyway?"
  ).setParent(*settingsWindow).question() == "Yes") {
    program->save();
    program->saveUndoState();
    settings["Crashed"].setValue(true);
    settings.save();
    program->updateAudioDriver(*settingsWindow);
    settings["Crashed"].setValue(false);
    settings.save();
    audioDriverChanged();
  }
}

auto DriverSettings::audioDeviceChanged() -> void {
  audioDeviceOption.reset();
  for(auto& device : audio.hasDevices()) {
    ComboButtonItem item{&audioDeviceOption};
    item.setText(device);
    if(device == audio.device()) item.setSelected();
  }
//audioDeviceOption.setEnabled(audio->hasDevice());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::audioDeviceChange() -> void {
  auto item = audioDeviceOption.selected();
  settings["Audio/Device"].setValue(item.text());
  program->updateAudioDevice();
  audioFrequencyChanged();
  audioLatencyChanged();
}

auto DriverSettings::audioFrequencyChanged() -> void {
  audioFrequencyOption.reset();
  for(auto& frequency : audio.hasFrequencies()) {
    ComboButtonItem item{&audioFrequencyOption};
    item.setText({(uint)frequency, "hz"});
    if(frequency == audio.frequency()) item.setSelected();
  }
//audioFrequencyOption.setEnabled(audio->hasFrequency());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::audioFrequencyChange() -> void {
  auto item = audioFrequencyOption.selected();
  settings["Audio/Frequency"].setValue(item.text());
  program->updateAudioFrequency();
}

auto DriverSettings::audioLatencyChanged() -> void {
  audioLatencyOption.reset();
  for(auto& latency : audio.hasLatencies()) {
    ComboButtonItem item{&audioLatencyOption};
    item.setText(latency);
    if(latency == audio.latency()) item.setSelected();
  }
//audioLatencyOption.setEnabled(audio->hasLatency());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::audioLatencyChange() -> void {
  auto item = audioLatencyOption.selected();
  settings["Audio/Latency"].setValue(item.text());
  program->updateAudioLatency();
}

//

auto DriverSettings::inputDriverChanged() -> void {
  inputDriverOption.reset();
  for(auto& driver : input.hasDrivers()) {
    ComboButtonItem item{&inputDriverOption};
    item.setText(driver);
    if(driver == input.driverName()) item.setSelected();
  }
  inputDriverActive.setText({"Active driver: ", input.driverName()});
  inputDriverOption.doChange();
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::inputDriverChange() -> void {
  auto item = inputDriverOption.selected();
  settings["Input/Driver"].setValue(item.text());
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the input driver change now anyway?"
  ).setParent(*settingsWindow).question() == "Yes") {
    program->save();
    program->saveUndoState();
    settings["Crashed"].setValue(true);
    settings.save();
    program->updateInputDriver(*settingsWindow);
    settings["Crashed"].setValue(false);
    settings.save();
    inputDriverChanged();
  }
}
