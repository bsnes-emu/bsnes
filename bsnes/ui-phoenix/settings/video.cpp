VideoSettings videoSettings;

void VideoSettings::create() {
  application.windows.append(this);
  Window::create(0, 0, 256, 256, "Video Settings");
  setDefaultFont(application.proportionalFont);

  unsigned x = 5, y = 5;

  colorAdjustmentLabel.create(*this, x, y, 430, Style::LabelHeight, "Color Adjustment :."); y += Style::LabelHeight + 5;
  colorAdjustmentLabel.setFont(application.proportionalFontBold);

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

  setGeometry(160, 160, 440, y + 5);

  contrastSlider.setPosition(config.video.contrast);
  brightnessSlider.setPosition(config.video.brightness);
  gammaSlider.setPosition(config.video.gamma);
  gammaRampCheck.setChecked(config.video.useGammaRamp);

  contrastSlider.onChange = brightnessSlider.onChange = gammaSlider.onChange = gammaRampCheck.onTick =
  { &VideoSettings::adjust, this };
}

void VideoSettings::adjust() {
  contrastValue.setText(string(contrastSlider.position(), "%"));
  brightnessValue.setText(string(brightnessSlider.position(), "%"));
  gammaValue.setText(string(gammaSlider.position(), "%"));

  config.video.contrast = contrastSlider.position();
  config.video.brightness = brightnessSlider.position();
  config.video.gamma = gammaSlider.position();
  config.video.useGammaRamp = gammaRampCheck.checked();
  palette.update();
}
