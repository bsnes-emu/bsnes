AdvancedSettings advancedSettings;

void AdvancedSettings::create() {
  Window::create(0, 0, 256, 256, "Advanced Settings");
  application.addWindow(this, "AdvancedSettings", "160,160");

  unsigned x = 5, y = 5;

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
  focusPolicyIgnore.setParent(focusPolicyPause);
  focusPolicyAllow.setParent(focusPolicyPause);
  if(config.settings.focusPolicy == 0) focusPolicyPause.setChecked();
  if(config.settings.focusPolicy == 1) focusPolicyIgnore.setChecked();
  if(config.settings.focusPolicy == 2) focusPolicyAllow.setChecked();

  layout.append(driverSelectionLabel, x,       y, 595, Style::LabelHeight);    y += Style::LabelHeight + 5;
  layout.append(videoDriverLabel,     x,       y,  45, Style::ComboBoxHeight);
  layout.append(videoDriverBox,       x +  45, y, 150, Style::ComboBoxHeight);
  layout.append(audioDriverLabel,     x + 200, y,  45, Style::ComboBoxHeight);
  layout.append(audioDriverBox,       x + 245, y, 150, Style::ComboBoxHeight);
  layout.append(inputDriverLabel,     x + 400, y,  45, Style::ComboBoxHeight);
  layout.append(inputDriverBox,       x + 445, y, 150, Style::ComboBoxHeight); y += Style::ComboBoxHeight + 5;
  layout.append(focusPolicyLabel,     x,       y, 595, Style::LabelHeight);    y += Style::LabelHeight + 5;
  layout.append(focusPolicyPause,     x,       y, 195, Style::CheckBoxHeight);
  layout.append(focusPolicyIgnore,    x + 200, y, 195, Style::CheckBoxHeight);
  layout.append(focusPolicyAllow,     x + 400, y, 195, Style::CheckBoxHeight); y += Style::CheckBoxHeight + 5;
  setGeometry(0, 0, 605, y);
  setLayout(layout);

  lstring list;

  list.split(";", video.driver_list());
  for(unsigned i = 0; i < list.size(); i++) {
    videoDriverBox.addItem(list[i]);
    if(list[i] == config.video.driver) videoDriverBox.setSelection(i);
  }

  list.split(";", audio.driver_list());
  for(unsigned i = 0; i < list.size(); i++) {
    audioDriverBox.addItem(list[i]);
    if(list[i] == config.audio.driver) audioDriverBox.setSelection(i);
  }

  list.split(";", input.driver_list());
  for(unsigned i = 0; i < list.size(); i++) {
    inputDriverBox.addItem(list[i]);
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
