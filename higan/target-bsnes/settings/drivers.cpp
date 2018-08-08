auto DriverSettings::create() -> void {
  setIcon(Icon::Place::Settings);
  setText("Drivers");

  layout.setPadding(5);

  videoLabel.setText("Video").setFont(Font().setBold());
  videoLayout.setSize({2, 2});
  videoDriverLabel.setText("Driver:");
  videoDriverOption.onChange([&] {
    videoDriverUpdate.setEnabled(videoDriverOption.selected().text() != video.driver());
  });
  videoDriverUpdate.setText("Change").onActivate([&] { videoDriverChange(); });
  videoFormatLabel.setText("Format:");
  videoFormatOption.onChange([&] { videoFormatChange(); });
  videoExclusiveToggle.setText("Exclusive").setToolTip(
    "(Direct3D only)\n\n"
    "Acquires exclusive access to the display in fullscreen mode.\n"
    "Eliminates compositing issues such as video stuttering."
  ).onToggle([&] {
    settings.video.exclusive = videoExclusiveToggle.checked();
    program.updateVideoExclusive();
  });
  videoBlockingToggle.setText("Synchronize").setToolTip(
    "Waits for the video card to be ready before rendering frames.\n"
    "Eliminates dropped or duplicated frames; but can distort audio.\n\n"
    "With this option, it's recommended to disable audio sync,\n"
    "and enable dynamic rate control. Or alternatively, adjust the\n"
    "audio skew option to reduce buffer under/overflows."
  ).onToggle([&] {
    settings.video.blocking = videoBlockingToggle.checked();
    program.updateVideoBlocking();
    presentation.speedMenu.setEnabled(!videoBlockingToggle.checked() && audioBlockingToggle.checked());
  });
  videoFlushToggle.setText("GPU sync").setToolTip({
    "(OpenGL only)\n\n"
    "Causes the GPU to wait until frames are fully rendered.\n"
    "In the best case, this can remove up to one frame of input lag.\n"
    "However, it incurs a roughly 20% performance penalty.\n\n"
    "You should disable this option unless you find it necessary."
  }).onToggle([&] {
    settings.video.flush = videoFlushToggle.checked();
    program.updateVideoFlush();
  });
  videoSpacer.setColor({192, 192, 192});

  audioLabel.setText("Audio").setFont(Font().setBold());
  audioLayout.setSize({2, 2});
  audioDriverLabel.setText("Driver:");
  audioDriverOption.onChange([&] {
    audioDriverUpdate.setEnabled(audioDriverOption.selected().text() != audio.driver());
  });
  audioDriverUpdate.setText("Change").onActivate([&] { audioDriverChange(); });
  audioDeviceLabel.setText("Device:");
  audioDeviceOption.onChange([&] { audioDeviceChange(); });
  audioFrequencyLabel.setText("Frequency:");
  audioFrequencyOption.onChange([&] { audioFrequencyChange(); });
  audioLatencyLabel.setText("Latency:");
  audioLatencyOption.onChange([&] { audioLatencyChange(); });
  audioExclusiveToggle.setText("Exclusive").setToolTip(
    "(ASIO, WASAPI only)\n\n"
    "Acquires exclusive control of the sound card device.\n"
    "This can significantly reduce audio latency.\n"
    "However, it will block sounds from all other applications."
  ).onToggle([&] {
    settings.audio.exclusive = audioExclusiveToggle.checked();
    program.updateAudioExclusive();
  });
  audioBlockingToggle.setText("Synchronize").setToolTip(
    "Waits for the audio card to be ready before outputting samples.\n"
    "Eliminates audio distortio; but can distort video.\n\n"
    "With this option, it's recommended to disable video sync.\n"
    "For best results, use this with an adaptive sync monitor."
  ).onToggle([&] {
    settings.audio.blocking = audioBlockingToggle.checked();
    program.updateAudioBlocking();
    presentation.speedMenu.setEnabled(!videoBlockingToggle.checked() && audioBlockingToggle.checked());
  });
  audioDynamicToggle.setText("Dynamic rate").setToolTip(
    "(OSS only)\n\n"
    "Dynamically adjusts the audio frequency by tiny amounts.\n"
    "Use this with video sync enabled, and audio sync disabled.\n\n"
    "This can produce perfectly smooth video and clean audio,\n"
    "but only if your monitor refresh rate is set correctly:\n"
    "60hz for NTSC games, and 50hz for PAL games."
  ).onToggle([&] {
    settings.audio.dynamic = audioDynamicToggle.checked();
    program.updateAudioDynamic();
  });
  audioSpacer.setColor({192, 192, 192});

  inputLabel.setText("Input").setFont(Font().setBold());
  inputLayout.setSize({2, 1});
  inputDriverLabel.setText("Driver:");
  inputDriverOption.onChange([&] {
    inputDriverUpdate.setEnabled(inputDriverOption.selected().text() != input.driver());
  });
  inputDriverUpdate.setText("Change").onActivate([&] { inputDriverChange(); });

  //this will hide the video format setting for simplicity, as it's not very useful just yet ...
  //videoLayout.setSize({2, 1});
  //videoLayout.remove(videoFormatLabel);
  //videoLayout.remove(videoPropertyLayout);
}

//

auto DriverSettings::videoDriverChanged() -> void {
  videoDriverOption.reset();
  for(auto& driver : video.hasDrivers()) {
    ComboButtonItem item{&videoDriverOption};
    item.setText(driver);
    if(driver == video.driver()) item.setSelected();
  }
  videoDriverActive.setText({"Active driver: ", video.driver()});
  videoDriverOption.doChange();
  videoFormatChanged();
  videoExclusiveToggle.setChecked(video.exclusive()).setEnabled(video.hasExclusive());
  videoBlockingToggle.setChecked(video.blocking()).setEnabled(video.hasBlocking());
  videoFlushToggle.setChecked(video.flush()).setEnabled(video.hasFlush());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::videoDriverChange() -> void {
  auto item = videoDriverOption.selected();
  settings.video.driver = item.text();
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the video driver change now anyway?"
  ).setParent(*settingsWindow).question() == "Yes") {
    program.save();
    program.saveUndoState();
    settings.general.crashed = true;
    settings.save();
    program.updateVideoDriver(settingsWindow);
    settings.general.crashed = false;
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
  settings.video.format = item.text();
  video.setFormat(item.text());
}

//

auto DriverSettings::audioDriverChanged() -> void {
  audioDriverOption.reset();
  for(auto& driver : audio.hasDrivers()) {
    ComboButtonItem item{&audioDriverOption};
    item.setText(driver);
    if(driver == audio.driver()) item.setSelected();
  }
  audioDriverActive.setText({"Active driver: ", audio.driver()});
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
  settings.audio.driver = item.text();
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the audio driver change now anyway?"
  ).setParent(*settingsWindow).question() == "Yes") {
    program.save();
    program.saveUndoState();
    settings.general.crashed = true;
    settings.save();
    program.updateAudioDriver(settingsWindow);
    settings.general.crashed = false;
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
  settings.audio.device = item.text();
  program.updateAudioDevice();
  audioFrequencyChanged();
  audioLatencyChanged();
}

auto DriverSettings::audioFrequencyChanged() -> void {
  audioFrequencyOption.reset();
  for(auto& frequency : audio.hasFrequencies()) {
    ComboButtonItem item{&audioFrequencyOption};
    item.setText({frequency, "hz"});
    if(frequency == audio.frequency()) item.setSelected();
  }
//audioFrequencyOption.setEnabled(audio->hasFrequency());
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::audioFrequencyChange() -> void {
  auto item = audioFrequencyOption.selected();
  settings.audio.frequency = item.text().natural();
  program.updateAudioFrequency();
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
  settings.audio.latency = item.text().natural();
  program.updateAudioLatency();
}

//

auto DriverSettings::inputDriverChanged() -> void {
  inputDriverOption.reset();
  for(auto& driver : input.hasDrivers()) {
    ComboButtonItem item{&inputDriverOption};
    item.setText(driver);
    if(driver == input.driver()) item.setSelected();
  }
  inputDriverActive.setText({"Active driver: ", input.driver()});
  inputDriverOption.doChange();
  layout.setGeometry(layout.geometry());
}

auto DriverSettings::inputDriverChange() -> void {
  auto item = inputDriverOption.selected();
  settings.input.driver = item.text();
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the input driver change now anyway?"
  ).setParent(*settingsWindow).question() == "Yes") {
    program.save();
    program.saveUndoState();
    settings.general.crashed = true;
    settings.save();
    program.updateInputDriver(settingsWindow);
    settings.general.crashed = false;
    settings.save();
    inputDriverChanged();
  }
}
