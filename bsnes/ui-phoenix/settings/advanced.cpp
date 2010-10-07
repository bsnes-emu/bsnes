AdvancedSettings advancedSettings;

void AdvancedSettings::create() {
  application.windows.append(this);
  Window::create(0, 0, 256, 256, "Advanced Settings");
  setDefaultFont(application.proportionalFont);

  unsigned x = 5, y = 5;

  driverSelectionLabel.create(*this, x, y, 595, Style::LabelHeight, "Driver Selection :."); y += Style::LabelHeight + 5;
  driverSelectionLabel.setFont(application.proportionalFontBold);

  videoDriverLabel.create(*this, x,       y,  45, Style::ComboBoxHeight, "Video:");
  videoDriverBox.create  (*this, x +  45, y, 150, Style::ComboBoxHeight);
  audioDriverLabel.create(*this, x + 200, y,  45, Style::ComboBoxHeight, "Audio:");
  audioDriverBox.create  (*this, x + 245, y, 150, Style::ComboBoxHeight);
  inputDriverLabel.create(*this, x + 400, y,  45, Style::ComboBoxHeight, "Input:");
  inputDriverBox.create  (*this, x + 445, y, 150, Style::ComboBoxHeight); y += Style::ComboBoxHeight + 5;

  focusPolicyLabel.create(*this, x, y, 595, Style::LabelHeight, "Focus Policy :."); y += Style::LabelHeight + 5;
  focusPolicyLabel.setFont(application.proportionalFontBold);

  focusPolicyPause.create(*this, x, y, 195, Style::CheckBoxHeight, "Pause emulator when inactive");
  focusPolicyIgnore.create(focusPolicyPause, x + 200, y, 195, Style::CheckBoxHeight, "Ignore input when inactive");
  focusPolicyAllow.create(focusPolicyPause, x + 400, y, 195, Style::CheckBoxHeight, "Always allow input"); y += Style::CheckBoxHeight + 5;
  if(config.settings.focusPolicy == 0) focusPolicyPause.setChecked();
  if(config.settings.focusPolicy == 1) focusPolicyIgnore.setChecked();
  if(config.settings.focusPolicy == 2) focusPolicyAllow.setChecked();

  setGeometry(160, 160, 605, y);

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
