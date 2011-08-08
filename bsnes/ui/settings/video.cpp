VideoSettings videoSettings;

void VideoSettings::create() {
  title.setText("Video Settings");
  title.setFont(application.titleFont);

  colorAdjustmentLabel.setText("Color Adjustment:");
  colorAdjustmentLabel.setFont(application.proportionalFontBold);
  brightnessLabel.setText("Brightness:");
  brightnessSlider.setLength(201);
  contrastLabel.setText("Contrast:");
  contrastSlider.setLength(201);
  gammaLabel.setText("Gamma:");
  gammaSlider.setLength(201);
  gammaRampCheck.setText("Enable NTSC gamma ramp simulation");
  fullscreenLabel.setText("Fullscreen:");
  fullscreenLabel.setFont(application.proportionalFontBold);
  fullscreenCenter.setText("Center");
  fullscreenScale.setText("Scale");
  fullscreenStretch.setText("Stretch");
  RadioBox::group(fullscreenCenter, fullscreenScale, fullscreenStretch);

  panelLayout.setMargin(5);
  panelLayout.append(panel, SettingsWindow::PanelWidth, ~0, 5);
  panelLayout.append(layout);

  layout.append(title, ~0, 0, 5);

  layout.append(colorAdjustmentLabel,        ~0, 0   );
  brightnessLayout.append(brightnessLabel,   80, 0   );
  brightnessLayout.append(brightnessValue,   50, 0   );
  brightnessLayout.append(brightnessSlider,  ~0, 0   );
  layout.append(brightnessLayout                     );
  contrastLayout.append(contrastLabel,       80, 0   );
  contrastLayout.append(contrastValue,       50, 0   );
  contrastLayout.append(contrastSlider,      ~0, 0   );
  layout.append(contrastLayout                       );
  gammaLayout.append(gammaLabel,             80, 0   );
  gammaLayout.append(gammaValue,             50, 0   );
  gammaLayout.append(gammaSlider,            ~0, 0   );
  layout.append(gammaLayout                          );
  layout.append(gammaRampCheck,              ~0, 0, 5);
  layout.append(fullscreenLabel,             ~0, 0   );
  fullscreenLayout.append(fullscreenCenter,  ~0, 0, 5);
  fullscreenLayout.append(fullscreenScale,   ~0, 0, 5);
  fullscreenLayout.append(fullscreenStretch, ~0, 0   );
  layout.append(fullscreenLayout);

  layout.append(spacer, ~0, ~0);
  settingsWindow.append(panelLayout);

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

  fullscreenCenter.onTick = [] { config.video.fullscreenScale = 0; };
  fullscreenScale.onTick = [] { config.video.fullscreenScale = 1; };
  fullscreenStretch.onTick = [] { config.video.fullscreenScale = 2; };

  adjust();
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
