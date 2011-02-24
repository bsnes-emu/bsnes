AdvancedSettings advancedSettings;

void AdvancedSettings::create() {
  setTitle("Advanced Settings");
  application.addWindow(this, "AdvancedSettings", "160,160");

  driverSelectionLabel.setText("Driver Selection :.");
  driverSelectionLabel.setFont(application.proportionalFontBold);
  videoDriverLabel.setText("Video:");
  audioDriverLabel.setText("Audio:");
  inputDriverLabel.setText("Input:");
  focusPolicyLabel.setText("Focus Policy :.");
  focusPolicyLabel.setFont(application.proportionalFontBold);
  focusPolicyPause.setText("Pause emulator when inactive");
  focusPolicyIgnore.setText("Ignore input when inactive");
  focusPolicyAllow.setText("Always allow input");
  RadioBox::group(focusPolicyPause, focusPolicyIgnore, focusPolicyAllow);
  if(config.settings.focusPolicy == 0) focusPolicyPause.setChecked();
  if(config.settings.focusPolicy == 1) focusPolicyIgnore.setChecked();
  if(config.settings.focusPolicy == 2) focusPolicyAllow.setChecked();

  layout.setMargin(5);
  layout.append(driverSelectionLabel, 0, Style::LabelHeight);
  driverLayout.append(videoDriverLabel, 40, 0, 5);
  driverLayout.append(videoDriverBox,    0, 0, 5);
  driverLayout.append(audioDriverLabel, 40, 0, 5);
  driverLayout.append(audioDriverBox,    0, 0, 5);
  driverLayout.append(inputDriverLabel, 40, 0, 5);
  driverLayout.append(inputDriverBox,    0, 0);
  layout.append(driverLayout, 0, Style::ComboBoxHeight, 5);
  layout.append(focusPolicyLabel, 0, Style::LabelHeight);
  focusPolicyLayout.append(focusPolicyPause,  0, 0, 5);
  focusPolicyLayout.append(focusPolicyIgnore, 0, 0, 5);
  focusPolicyLayout.append(focusPolicyAllow,  0, 0);
  layout.append(focusPolicyLayout, 0, Style::CheckBoxHeight);

  setGeometry({ 0, 0, 640, layout.minimumHeight() });
  append(layout);

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

  videoDriverBox.onChange = []() {
    lstring list;
    list.split(";", video.driver_list());
    config.video.driver = list[advancedSettings.videoDriverBox.selection()];
  };

  audioDriverBox.onChange = []() {
    lstring list;
    list.split(";", audio.driver_list());
    config.audio.driver = list[advancedSettings.audioDriverBox.selection()];
  };

  inputDriverBox.onChange = []() {
    lstring list;
    list.split(";", input.driver_list());
    config.input.driver = list[advancedSettings.inputDriverBox.selection()];
  };

  focusPolicyPause.onTick = []() { config.settings.focusPolicy = 0; };
  focusPolicyIgnore.onTick = []() { config.settings.focusPolicy = 1; };
  focusPolicyAllow.onTick = []() { config.settings.focusPolicy = 2; };
}
