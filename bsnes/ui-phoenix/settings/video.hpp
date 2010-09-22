struct VideoSettingsWindow : Window {
  Label colorAdjustmentLabel;
  Label contrastLabel;
  Label contrastValue;
  HorizontalSlider contrastSlider;
  Label brightnessLabel;
  Label brightnessValue;
  HorizontalSlider brightnessSlider;
  Label gammaLabel;
  Label gammaValue;
  HorizontalSlider gammaSlider;
  CheckBox gammaRampCheck;

  void create();
  void adjust();
};

extern VideoSettingsWindow videoSettingsWindow;
