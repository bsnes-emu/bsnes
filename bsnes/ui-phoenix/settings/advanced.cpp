AdvancedSettingsWindow advancedSettingsWindow;

void AdvancedSettingsWindow::create() {
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
  inputDriverBox.create  (*this, x + 445, y, 150, Style::ComboBoxHeight); y += Style::ComboBoxHeight;

  setGeometry(0, 0, 605, y + 5);

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
    config.video.driver = list[advancedSettingsWindow.videoDriverBox.selection()];
  };

  audioDriverBox.onChange = []() {
    lstring list;
    list.split(";", audio.driver_list());
    config.audio.driver = list[advancedSettingsWindow.audioDriverBox.selection()];
  };

  inputDriverBox.onChange = []() {
    lstring list;
    list.split(";", input.driver_list());
    config.input.driver = list[advancedSettingsWindow.inputDriverBox.selection()];
  };

  onClose = []() {
    advancedSettingsWindow.setVisible(false);
    return false;
  };
}
