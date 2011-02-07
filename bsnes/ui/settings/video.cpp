VideoSettings videoSettings;

void VideoSettings::create() {
  Window::create(0, 0, 256, 256, "Video Settings");
  application.addWindow(this, "VideoSettings", "160,160");

  unsigned x = 5, y = 5, height = Style::TextBoxHeight;

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

  layout.append(colorAdjustmentLabel, x,                             y, 430,                        Style::LabelHeight);    y += Style::LabelHeight + 5;
  layout.append(brightnessLabel,      x,                             y,  80,                        Style::SliderHeight);
  layout.append(brightnessValue,      x +  80,                       y,  40,                        Style::SliderHeight);
  layout.append(brightnessSlider,     x + 130,                       y, 300,                        Style::SliderHeight);   y += Style::SliderHeight;
  layout.append(contrastLabel,        x,                             y,  80,                        Style::SliderHeight);
  layout.append(contrastValue,        x +  80,                       y,  50,                        Style::SliderHeight);
  layout.append(contrastSlider,       x + 130,                       y, 300,                        Style::SliderHeight);   y += Style::SliderHeight;
  layout.append(gammaLabel,           x,                             y,  80,                        Style::SliderHeight);
  layout.append(gammaValue,           x +  80,                       y,  50,                        Style::SliderHeight);
  layout.append(gammaSlider,          x + 130,                       y, 300,                        Style::SliderHeight);   y += Style::SliderHeight + 5;
  layout.append(gammaRampCheck,       x,                             y, 430,                        Style::CheckBoxHeight); y += Style::CheckBoxHeight + 5;
  layout.append(fullscreenLabel,      x,                             y, 340,                        Style::LabelHeight);    y += Style::LabelHeight + 5;
  layout.append(fullscreenCenter,     x,                             y, 135,                        Style::CheckBoxHeight);
  layout.append(fullscreenScale,      x + 140,                       y, 135,                        Style::CheckBoxHeight);
  layout.append(fullscreenStretch,    x + 280,                       y, 135,                        Style::CheckBoxHeight); y += Style::CheckBoxHeight + 5;
  layout.append(filterLabel,          x,                             y, 340,                        Style::LabelHeight);    y += Style::LabelHeight + 5;
  layout.append(filterPath,           x,                             y, 430 - height - height - 10, height);
  layout.append(filterClear,          x + 430 - height - height - 5, y, height,                     height);
  layout.append(filterSelect,         x + 430 - height,              y, height,                     height);                y += height + 5;
  layout.append(shaderLabel,          x,                             y, 340,                        Style::LabelHeight);    y += Style::LabelHeight + 5;
  layout.append(shaderPath,           x,                             y, 430 - height - height - 10, height);
  layout.append(shaderClear,          x + 430 - height - height - 5, y, height,                     height);
  layout.append(shaderSelect,         x + 430 - height,              y, height,                     height);                y += height + 5;
  setGeometry(0, 0, 440, y);
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
