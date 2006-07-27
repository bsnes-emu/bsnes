class ColorAdjustWindow : public Window {
public:
Label    Header;
Label    ContrastLabel;
Slider   ContrastSlider;
Label    BrightnessLabel;
Slider   BrightnessSlider;
Label    GammaLabel;
Slider   GammaSlider;
Checkbox GammaRamp;
Checkbox Sepia;
Checkbox Grayscale;
Checkbox Invert;
Button   Restore;

  bool Event(EventInfo &info);

  void Setup();
} wColorAdjust;
