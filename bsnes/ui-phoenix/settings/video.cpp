VideoSettingsWindow videoSettingsWindow;

void VideoSettingsWindow::create() {
  Window::create(0, 0, 256, 256, "Video Settings");
  setDefaultFont(application.font);

  unsigned x = 5, y = 5;

  contrastLabel.create   (*this, x,       y,  80, Style::SliderHeight, "Contrast:");
  contrastValue.create   (*this, x + 80,  y,  50, Style::SliderHeight, "100%");
  contrastSlider.create  (*this, x + 130, y, 300, Style::SliderHeight, 201); y += Style::SliderHeight;

  brightnessLabel.create (*this, x,       y,  80, Style::SliderHeight, "Brightness:");
  brightnessValue.create (*this, x +  80, y,  40, Style::SliderHeight, "100%");
  brightnessSlider.create(*this, x + 130, y, 300, Style::SliderHeight, 201); y += Style::SliderHeight;

  gammaLabel.create      (*this, x,       y,  80, Style::SliderHeight, "Gamma:");
  gammaValue.create      (*this, x +  80, y,  50, Style::SliderHeight, "100%");
  gammaSlider.create     (*this, x + 130, y, 300, Style::SliderHeight, 201); y += Style::SliderHeight + 5;

  gammaRampCheck.create  (*this, x,       y, 430, 15, "Enable NTSC gamma ramp simulation"); y += 15;

  setGeometry(0, 0, 440, y + 5);

  contrastSlider.setPosition(100);
  brightnessSlider.setPosition(100);
  gammaSlider.setPosition(100);
  gammaRampCheck.setChecked();

  contrastSlider.onChange = brightnessSlider.onChange = gammaSlider.onChange = gammaRampCheck.onTick =
  { &VideoSettingsWindow::adjust, this };

  onClose = []() {
    videoSettingsWindow.setVisible(false);
    return false;
  };
}

void VideoSettingsWindow::adjust() {
  contrastValue.setText(string(contrastSlider.position(), "%"));
  brightnessValue.setText(string(brightnessSlider.position(), "%"));
  gammaValue.setText(string(gammaSlider.position(), "%"));

  palette.contrast = contrastSlider.position();
  palette.brightness = brightnessSlider.position();
  palette.gamma = gammaSlider.position();
  palette.useGammaRamp = gammaRampCheck.checked();
  palette.update();
}
