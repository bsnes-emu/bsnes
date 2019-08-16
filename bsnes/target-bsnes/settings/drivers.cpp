auto DriverSettings::create() -> void {
  setCollapsible();
  setVisible(false);

  videoLabel.setText("Video").setFont(Font().setBold());
  videoDriverLabel.setText("Driver:");
  videoDriverOption.onChange([&] {
    videoDriverUpdate.setText(videoDriverOption.selected().text() != video.driver() ? "Change" : "Reload");
  });
  videoDriverUpdate.setText("Change").onActivate([&] { videoDriverChange(); });
  videoMonitorLabel.setText("Fullscreen Monitor:").setToolTip(
    "Sets which monitor video is sent to in fullscreen mode."
  );
  videoMonitorOption.onChange([&] { videoMonitorChange(); });
  videoFormatLabel.setText("Format:");
  videoFormatOption.onChange([&] { videoFormatChange(); });
  videoExclusiveToggle.setText("Exclusive").setToolTip(
    "Causes fullscreen mode to take over all monitors.\n"
    "This allows adaptive sync to work better and reduces input latency.\n"
    "However, multi-monitor users should turn this option off.\n"
    "Note: Direct3D exclusive mode also does not honor the requested monitor."
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
    "(OpenGL driver only)\n\n"
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
    audioDriverUpdate.setText(audioDriverOption.selected().text() != audio.driver() ? "Change" : "Reload");
  });
  audioDriverUpdate.setText("Change").onActivate([&] { audioDriverChange(); });
  audioDeviceLabel.setText("Device:");
  audioDeviceOption.onChange([&] { audioDeviceChange(); });
  audioFrequencyLabel.setText("Frequency:");
  audioFrequencyOption.onChange([&] { audioFrequencyChange(); });
  audioLatencyLabel.setText("Latency:");
  audioLatencyOption.onChange([&] { audioLatencyChange(); });
  audioExclusiveToggle.setText("Exclusive mode").setToolTip(
    "(ASIO, WASAPI drivers only)\n\n"
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
    "(OSS, XAudio2, waveOut drivers only)\n\n"
    "Dynamically adjusts the audio frequency by tiny amounts.\n"
    "Use this with video sync enabled, and audio sync disabled.\n\n"
    "This can produce perfectly smooth video and clean audio,\n"
    "but only if your monitor refresh rate is set correctly:\n"
    "60 Hz for NTSC games, and 50 Hz for PAL games."
  ).onToggle([&] {
    settings.audio.dynamic = audioDynamicToggle.checked();
    program.updateAudioDynamic();
  });
  audioSpacer.setColor({192, 192, 192});

  inputLabel.setText("Input").setFont(Font().setBold());
  inputLayout.setSize({2, 1});
  inputDriverLabel.setText("Driver:");
  inputDriverOption.onChange([&] {
    inputDriverUpdate.setText(inputDriverOption.selected().text() != input.driver() ? "Change" : "Reload");
  });
  inputDriverUpdate.setText("Change").setToolTip(
    "A driver reload can be used to detect hotplugged devices.\n"
    "This is useful for APIs that lack auto-hotplug support,\n"
    "such as DirectInput and SDL."
  ).onActivate([&] { inputDriverChange(); });
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
  videoMonitorChanged();
  videoFormatChanged();
  videoExclusiveToggle.setChecked(video.exclusive()).setEnabled(video.hasExclusive());
  videoBlockingToggle.setChecked(video.blocking()).setEnabled(video.hasBlocking());
  videoFlushToggle.setChecked(video.flush()).setEnabled(video.hasFlush());
  setGeometry(geometry());
}

auto DriverSettings::videoDriverChange() -> void {
  auto item = videoDriverOption.selected();
  settings.video.driver = item.text();
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the video driver change now anyway?"
  ).setAlignment(*settingsWindow).question() == "Yes") {
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

auto DriverSettings::videoMonitorChanged() -> void {
  videoMonitorOption.reset();
  for(auto& monitor : Video::hasMonitors()) {
    ComboButtonItem item{&videoMonitorOption};
    item.setText(monitor.name);
    if(monitor.name == video.monitor()) item.setSelected();
  }
  videoMonitorOption.setEnabled(videoMonitorOption.itemCount() > 1);
  setGeometry(geometry());
  videoMonitorChange();
}

auto DriverSettings::videoMonitorChange() -> void {
  auto item = videoMonitorOption.selected();
  settings.video.monitor = item.text();
  program.updateVideoMonitor();
}

auto DriverSettings::videoFormatChanged() -> void {
  videoFormatOption.reset();
  for(auto& format : video.hasFormats()) {
    ComboButtonItem item{&videoFormatOption};
    item.setText(format);
    if(format == video.format()) item.setSelected();
  }
//videoFormatOption.setEnabled(video.hasFormat());
  setGeometry(geometry());
  videoFormatChange();
}

auto DriverSettings::videoFormatChange() -> void {
  auto item = videoFormatOption.selected();
  settings.video.format = item.text();
  program.updateVideoFormat();
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
  setGeometry(geometry());
}

auto DriverSettings::audioDriverChange() -> void {
  auto item = audioDriverOption.selected();
  settings.audio.driver = item.text();
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the audio driver change now anyway?"
  ).setAlignment(*settingsWindow).question() == "Yes") {
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
  setGeometry(geometry());
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
    item.setText({frequency, " Hz"});
    if(frequency == audio.frequency()) item.setSelected();
  }
//audioFrequencyOption.setEnabled(audio->hasFrequency());
  setGeometry(geometry());
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
  setGeometry(geometry());
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
  setGeometry(geometry());
}

auto DriverSettings::inputDriverChange() -> void {
  auto item = inputDriverOption.selected();
  settings.input.driver = item.text();
  if(!emulator->loaded() || item.text() == "None" || MessageDialog(
    "Warning: incompatible drivers may cause bsnes to crash.\n"
    "It is highly recommended you unload your game first to be safe.\n"
    "Do you wish to proceed with the input driver change now anyway?"
  ).setAlignment(*settingsWindow).question() == "Yes") {
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
