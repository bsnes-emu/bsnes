struct VideoSettings : TopLevelWindow {
  VerticalLayout layout;
  Label colorAdjustmentLabel;

  Label brightnessLabel;
  HorizontalLayout brightnessLayout;
  Label brightnessValue;
  HorizontalSlider brightnessSlider;

  Label contrastLabel;
  HorizontalLayout contrastLayout;
  Label contrastValue;
  HorizontalSlider contrastSlider;

  Label gammaLabel;
  HorizontalLayout gammaLayout;
  Label gammaValue;
  HorizontalSlider gammaSlider;

  CheckBox gammaRampCheck;

  Label fullscreenLabel;
  HorizontalLayout fullscreenLayout;
  RadioBox fullscreenCenter;
  RadioBox fullscreenScale;
  RadioBox fullscreenStretch;

  void create();
  void adjust();
};

extern VideoSettings videoSettings;
