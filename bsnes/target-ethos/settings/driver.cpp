DriverSettings *driverSettings = nullptr;

DriverSettings::DriverSettings() {
  title.setFont(application->titleFont);
  title.setText("Driver Configuration");
  videoLabel.setText("Video:");
  audioLabel.setText("Audio:");
  inputLabel.setText("Input:");

  lstring list;

  list.split(";", video.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    videoDriver.append(list[n]);
    if(list[n] == config->video.driver) videoDriver.setSelection(n);
  }

  list.split(";", audio.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    audioDriver.append(list[n]);
    if(list[n] == config->audio.driver) audioDriver.setSelection(n);
  }

  list.split(";", input.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    inputDriver.append(list[n]);
    if(list[n] == config->input.driver) inputDriver.setSelection(n);
  }

  append(title, {~0, 0}, 5);
  append(driverLayout, {~0, 0});
    driverLayout.append(videoLabel, {0, 0}, 5);
    driverLayout.append(videoDriver, {~0, 0}, 5);
    driverLayout.append(audioLabel, {0, 0}, 5);
    driverLayout.append(audioDriver, {~0, 0}, 5);
    driverLayout.append(inputLabel, {0, 0}, 5);
    driverLayout.append(inputDriver, {~0, 0});

  videoDriver.onChange = [&] { config->video.driver = videoDriver.text(); };
  audioDriver.onChange = [&] { config->audio.driver = audioDriver.text(); };
  inputDriver.onChange = [&] { config->input.driver = inputDriver.text(); };
}
