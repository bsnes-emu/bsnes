struct VideoSettings : Window {
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
  Label shaderLabel;
  TextBox shaderPath;
  Button shaderClear;
  Button shaderSelect;

  void create();
  void adjust();
};

extern VideoSettings videoSettings;
