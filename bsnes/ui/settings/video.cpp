VideoSettings videoSettings;

void VideoSettings::create() {
  Window::create(0, 0, 480, 225, "Video Settings");
  application.addWindow(this, "VideoSettings", "160,160");

  colorAdjustmentLabel.setText("Color Adjustment :.");
  colorAdjustmentLabel.setFont(application.proportionalFontBold);
  brightnessLabel.setText("Brightness:");
  brightnessSlider.setLength(201);
  contrastLabel.setText("Contrast:");
  contrastSlider.setLength(201);
  gammaLabel.setText("Gamma:");
  gammaSlider.setLength(201);
  gammaRampCheck.setText("Enable NTSC gamma ramp simulation");
  fullscreenLabel.setText("Fullscreen :.");
  fullscreenLabel.setFont(application.proportionalFontBold);
  fullscreenCenter.setText("Center");
  fullscreenScale.setText("Scale");
  fullscreenStretch.setText("Stretch");
  fullscreenScale.setParent(fullscreenCenter);
  fullscreenStretch.setParent(fullscreenCenter);
  filterLabel.setText("Video Filter :.");
  filterLabel.setFont(application.proportionalFontBold);
  filterPath.setEditable(false);
  filterPath.setText(config.video.filter);
  filterSelect.setText("...");
  shaderLabel.setText("Pixel Shader :.");
  shaderLabel.setFont(application.proportionalFontBold);
  shaderPath.setEditable(false);
  shaderPath.setText(config.video.shader);
  shaderSelect.setText("...");

  layout.setMargin(5);
  layout.append(colorAdjustmentLabel, 0, Style::LabelHeight);
  brightnessLayout.append(brightnessLabel, 80, Style::SliderHeight);
  brightnessLayout.append(brightnessValue, 50, Style::SliderHeight);
  brightnessLayout.append(brightnessSlider, 0, Style::SliderHeight);
  layout.append(brightnessLayout, 0, Style::SliderHeight);
  contrastLayout.append(contrastLabel, 80, Style::SliderHeight);
  contrastLayout.append(contrastValue, 50, Style::SliderHeight);
  contrastLayout.append(contrastSlider, 0, Style::SliderHeight);
  layout.append(contrastLayout, 0, Style::SliderHeight);
  gammaLayout.append(gammaLabel, 80, Style::SliderHeight);
  gammaLayout.append(gammaValue, 50, Style::SliderHeight);
  gammaLayout.append(gammaSlider, 0, Style::SliderHeight);
  layout.append(gammaLayout, 0, Style::SliderHeight);
  layout.append(gammaRampCheck, 0, Style::CheckBoxHeight, 5);
  layout.append(fullscreenLabel, 0, Style::LabelHeight);
  fullscreenLayout.append(fullscreenCenter, 0, Style::CheckBoxHeight);
  fullscreenLayout.append(fullscreenScale, 0, Style::CheckBoxHeight);
  fullscreenLayout.append(fullscreenStretch, 0, Style::CheckBoxHeight);
  layout.append(fullscreenLayout, 0, Style::CheckBoxHeight, 5);
  layout.append(filterLabel, 0, Style::LabelHeight);
  filterLayout.append(filterPath, 0, Style::TextBoxHeight, 5);
  filterLayout.append(filterClear, Style::TextBoxHeight, Style::TextBoxHeight, 5);
  filterLayout.append(filterSelect, Style::TextBoxHeight, Style::TextBoxHeight);
  layout.append(filterLayout, 0, Style::TextBoxHeight, 5);
  layout.append(shaderLabel, 0, Style::LabelHeight);
  shaderLayout.append(shaderPath, 0, Style::TextBoxHeight, 5);
  shaderLayout.append(shaderClear, Style::TextBoxHeight, Style::TextBoxHeight, 5);
  shaderLayout.append(shaderSelect, Style::TextBoxHeight, Style::TextBoxHeight);
  layout.append(shaderLayout, 0, Style::TextBoxHeight);
  setLayout(layout);

  brightnessSlider.setPosition(config.video.brightness);
  brightnessValue.setText({ config.video.brightness, "%" });

  contrastSlider.setPosition(config.video.contrast);
  contrastValue.setText({ config.video.contrast, "%" });

  gammaSlider.setPosition(config.video.gamma);
  gammaValue.setText({ config.video.gamma, "%" });

  gammaRampCheck.setChecked(config.video.useGammaRamp);

  switch(config.video.fullscreenScale) { default:
    case 0: fullscreenCenter.setChecked(); break;
    case 1: fullscreenScale.setChecked(); break;
    case 2: fullscreenStretch.setChecked(); break;
  }

  contrastSlider.onChange = brightnessSlider.onChange = gammaSlider.onChange = gammaRampCheck.onTick =
  { &VideoSettings::adjust, this };

  fullscreenCenter.onTick = []() { config.video.fullscreenScale = 0; };
  fullscreenScale.onTick = []() { config.video.fullscreenScale = 1; };
  fullscreenStretch.onTick = []() { config.video.fullscreenScale = 2; };

  filterClear.onTick = []() {
    config.video.filter = "";
    videoSettings.filterPath.setText(config.video.filter);
    utility.setFilter();
  };

  filterSelect.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::Filter, [](string filename) {
      config.video.filter = filename;
      videoSettings.filterPath.setText(config.video.filter);
      utility.setFilter();
    });
  };

  shaderClear.onTick = []() {
    config.video.shader = "";
    videoSettings.shaderPath.setText(config.video.shader);
    utility.setShader();
  };

  shaderSelect.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::Shader, [](string filename) {
      config.video.shader = filename;
      videoSettings.shaderPath.setText(config.video.shader);
      utility.setShader();
    });
  };
}

void VideoSettings::adjust() {
  brightnessValue.setText({ brightnessSlider.position(), "%" });
  contrastValue.setText({ contrastSlider.position(), "%" });
  gammaValue.setText({ gammaSlider.position(), "%" });

  config.video.brightness = brightnessSlider.position();
  config.video.contrast = contrastSlider.position();
  config.video.gamma = gammaSlider.position();
  config.video.useGammaRamp = gammaRampCheck.checked();
  palette.update();
}
