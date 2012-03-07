AdvancedSettings *advancedSettings = nullptr;

AdvancedSettings::AdvancedSettings() {
  title.setFont(application->titleFont);
  title.setText("Advanced Settings");
  driverLabel.setFont(application->boldFont);
  driverLabel.setText("Driver selection: (changes require restart to take effect)");
  videoLabel.setText("Video:");
  audioLabel.setText("Audio:");
  inputLabel.setText("Input:");
  focusPolicyLabel.setFont(application->boldFont);
  focusPolicyLabel.setText("When emulation window does not have focus:");
  focusPolicy[0].setText("Allow input");
  focusPolicy[1].setText("Ignore input");
  focusPolicy[2].setText("Pause emulation");
  RadioBox::group(focusPolicy[0], focusPolicy[1], focusPolicy[2]);
  focusPolicy[config->input.focusPolicy].setChecked();
  aboutLabel.setFont(application->boldFont);
  aboutLabel.setText("bsnes    author: byuu    license: GPLv3    website: byuu.org");

  lstring list;

  list.split(";", video.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    videoDriver.append(list[n]);
    if(list[n] == config->video.driver) videoDriver.setSelection(n);
    if(list[n] == video.default_driver() && config->video.driver == "") videoDriver.setSelection(n);
  }

  list.split(";", audio.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    audioDriver.append(list[n]);
    if(list[n] == config->audio.driver) audioDriver.setSelection(n);
    if(list[n] == audio.default_driver() && config->audio.driver == "") audioDriver.setSelection(n);
  }

  list.split(";", input.driver_list());
  for(unsigned n = 0; n < list.size(); n++) {
    inputDriver.append(list[n]);
    if(list[n] == config->input.driver) inputDriver.setSelection(n);
    if(list[n] == input.default_driver() && config->input.driver == "") inputDriver.setSelection(n);
  }

  append(title,                              { ~0,  0 }, 5);
  append(driverLabel,                        { ~0,  0 }, 0);
  append(driverLayout,                       { ~0,  0 }, 5);
    driverLayout.append(videoLabel,          {  0,  0 }, 5);
    driverLayout.append(videoDriver,         { ~0,  0 }, 5);
    driverLayout.append(audioLabel,          {  0,  0 }, 5);
    driverLayout.append(audioDriver,         { ~0,  0 }, 5);
    driverLayout.append(inputLabel,          {  0,  0 }, 5);
    driverLayout.append(inputDriver,         { ~0,  0 }, 0);
  append(focusPolicyLabel,                   { ~0,  0 }, 0);
  append(focusPolicyLayout,                  { ~0,  0 }, 5);
    focusPolicyLayout.append(focusPolicy[0], { ~0,  0 }, 5);
    focusPolicyLayout.append(focusPolicy[1], { ~0,  0 }, 5);
    focusPolicyLayout.append(focusPolicy[2], { ~0,  0 }, 0);
  append(spacer,                             { ~0, ~0 }, 0);
  append(aboutLabel,                         { ~0,  0 }, 0);

  videoDriver.onChange = [&] {
    lstring list;
    list.split(";", video.driver_list());
    config->video.driver = list[videoDriver.selection()];
  };

  audioDriver.onChange = [&] {
    lstring list;
    list.split(";", audio.driver_list());
    config->audio.driver = list[audioDriver.selection()];
  };

  inputDriver.onChange = [&] {
    lstring list;
    list.split(";", input.driver_list());
    config->input.driver = list[inputDriver.selection()];
  };

  focusPolicy[0].onActivate = [&] { config->input.focusPolicy = 0; };
  focusPolicy[1].onActivate = [&] { config->input.focusPolicy = 1; };
  focusPolicy[2].onActivate = [&] { config->input.focusPolicy = 2; };
}
