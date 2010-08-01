class NTSCFilter : public QObject {
  Q_OBJECT

public:
  void bind(configuration&);
  void size(unsigned&, unsigned&, unsigned, unsigned);
  void render(uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
  QWidget* settings();

  NTSCFilter();
  ~NTSCFilter();

private:
  void initialize();
  void loadSettingsFromConfig();
  void syncUiToSettings();

private slots:
  void syncSettingsToUi();
  void setRfPreset();
  void setCompositePreset();
  void setSvideoPreset();
  void setRgbPreset();
  void setMonoPreset();

private:
  QWidget *widget;
  QVBoxLayout *layout;
  QGridLayout *gridLayout;
  QLabel *basicSettings;
  QLabel *hueLabel;
  QLabel *hueValue;
  QSlider *hueSlider;
  QLabel *saturationLabel;
  QLabel *saturationValue;
  QSlider *saturationSlider;
  QLabel *contrastLabel;
  QLabel *contrastValue;
  QSlider *contrastSlider;
  QLabel *brightnessLabel;
  QLabel *brightnessValue;
  QSlider *brightnessSlider;
  QLabel *sharpnessLabel;
  QLabel *sharpnessValue;
  QSlider *sharpnessSlider;
  QLabel *advancedSettings;
  QLabel *gammaLabel;
  QLabel *gammaValue;
  QSlider *gammaSlider;
  QLabel *resolutionLabel;
  QLabel *resolutionValue;
  QSlider *resolutionSlider;
  QLabel *artifactsLabel;
  QLabel *artifactsValue;
  QSlider *artifactsSlider;
  QLabel *fringingLabel;
  QLabel *fringingValue;
  QSlider *fringingSlider;
  QLabel *bleedLabel;
  QLabel *bleedValue;
  QSlider *bleedSlider;
  QCheckBox *mergeFieldsBox;
  QLabel *presets;
  QHBoxLayout *controlLayout;
  QPushButton *rfPreset;
  QPushButton *compositePreset;
  QPushButton *svideoPreset;
  QPushButton *rgbPreset;
  QPushButton *monoPreset;
  QWidget *spacer;
  QPushButton *ok;

  bool blockSignals;

  struct snes_ntsc_t *ntsc;
  snes_ntsc_setup_t setup;
  int burst, burst_toggle;

  //settings
  double hue;
  double saturation;
  double contrast;
  double brightness;
  double sharpness;
  double gamma;
  double resolution;
  double artifacts;
  double fringing;
  double bleed;
  bool mergeFields;
} filter_ntsc;
