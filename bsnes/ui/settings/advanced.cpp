AdvancedSettings advancedSettings;

void AdvancedSettings::create() {
  title.setText("Advanced Settings");
  title.setFont(application.titleFont);

  driverSelectionLabel.setText("Driver Selection:");
  driverSelectionLabel.setFont(application.proportionalFontBold);
  videoDriverLabel.setText("Video:");
  audioDriverLabel.setText("Audio:");
  inputDriverLabel.setText("Input:");
  focusPolicyLabel.setText("When emulation window lacks focus:");
  focusPolicyLabel.setFont(application.proportionalFontBold);
  focusPolicyPause.setText("Pause emulator");
  focusPolicyIgnore.setText("Ignore input");
  focusPolicyAllow.setText("Allow input");
  RadioBox::group(focusPolicyPause, focusPolicyIgnore, focusPolicyAllow);
  if(config.settings.focusPolicy == 0) focusPolicyPause.setChecked();
  if(config.settings.focusPolicy == 1) focusPolicyIgnore.setChecked();
  if(config.settings.focusPolicy == 2) focusPolicyAllow.setChecked();
  compositorPolicyLabel.setText("Disable Compositor:");
  compositorPolicyLabel.setFont(application.proportionalFontBold);
  compositorPolicyNever.setText("Never");
  compositorPolicyFullScreen.setText("Fullscreen");
  compositorPolicyAlways.setText("Always");
  RadioBox::group(compositorPolicyNever, compositorPolicyFullScreen, compositorPolicyAlways);
  if(config.settings.compositorPolicy == 0) compositorPolicyNever.setChecked();
  if(config.settings.compositorPolicy == 1) compositorPolicyFullScreen.setChecked();
  if(config.settings.compositorPolicy == 2) compositorPolicyAlways.setChecked();
  if(config.settings.compositorPolicy == 2) compositor::enable(false);

  panelLayout.setMargin(5);
  panelLayout.append(panel, SettingsWindow::PanelWidth, ~0, 5);
  panelLayout.append(layout);

  layout.append(title, ~0, 0, 5);

  layout.append(driverSelectionLabel, ~0, 0);
  driverLayout.append(videoDriverLabel, 0, 0, 5);
  driverLayout.append(videoDriverBox, ~0, 0, 5);
  driverLayout.append(audioDriverLabel, 0, 0, 5);
  driverLayout.append(audioDriverBox, ~0, 0, 5);
  driverLayout.append(inputDriverLabel, 0, 0, 5);
  driverLayout.append(inputDriverBox, ~0, 0   );
  layout.append(driverLayout, 5);
  layout.append(focusPolicyLabel, ~0, 0   );
  focusPolicyLayout.append(focusPolicyPause, ~0, 0, 5);
  focusPolicyLayout.append(focusPolicyIgnore, ~0, 0, 5);
  focusPolicyLayout.append(focusPolicyAllow, ~0, 0);
  layout.append(focusPolicyLayout, 5);
  layout.append(compositorPolicyLabel, ~0, 0);
  compositorPolicyLayout.append(compositorPolicyNever, ~0, 0, 5);
  compositorPolicyLayout.append(compositorPolicyFullScreen, ~0, 0, 5);
  compositorPolicyLayout.append(compositorPolicyAlways, ~0, 0);
  layout.append(compositorPolicyLayout);

  layout.append(spacer, ~0, ~0);
  settingsWindow.append(panelLayout);

  lstring list;

  list.split(";", video.driver_list());
  for(unsigned i = 0; i < list.size(); i++) {
    videoDriverBox.append(list[i]);
    if(list[i] == config.video.driver) videoDriverBox.setSelection(i);
  }

  list.split(";", audio.driver_list());
  for(unsigned i = 0; i < list.size(); i++) {
    audioDriverBox.append(list[i]);
    if(list[i] == config.audio.driver) audioDriverBox.setSelection(i);
  }

  list.split(";", input.driver_list());
  for(unsigned i = 0; i < list.size(); i++) {
    inputDriverBox.append(list[i]);
    if(list[i] == config.input.driver) inputDriverBox.setSelection(i);
  }

  videoDriverBox.onChange = [this]() {
    lstring list;
    list.split(";", video.driver_list());
    config.video.driver = list[videoDriverBox.selection()];
  };

  audioDriverBox.onChange = [this]() {
    lstring list;
    list.split(";", audio.driver_list());
    config.audio.driver = list[audioDriverBox.selection()];
  };

  inputDriverBox.onChange = [this]() {
    lstring list;
    list.split(";", input.driver_list());
    config.input.driver = list[inputDriverBox.selection()];
  };

  focusPolicyPause.onTick = [] { config.settings.focusPolicy = 0; };
  focusPolicyIgnore.onTick = [] { config.settings.focusPolicy = 1; };
  focusPolicyAllow.onTick = [] { config.settings.focusPolicy = 2; };

  compositorPolicyNever.onTick = [] { config.settings.compositorPolicy = 0; compositor::enable(application.compositorActive); };
  compositorPolicyFullScreen.onTick = [] { config.settings.compositorPolicy = 1; };
  compositorPolicyAlways.onTick = [] { config.settings.compositorPolicy = 2; compositor::enable(false); };
}
