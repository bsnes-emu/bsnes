class VideoSettingsWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *displayLabel;
  QCheckBox *autoHideFullscreenMenu;
  QLabel *colorLabel;
  QGridLayout *colorLayout;
  QLabel *contrastLabel;
  QLabel *contrastValue;
  QSlider *contrastSlider;
  QLabel *brightnessLabel;
  QLabel *brightnessValue;
  QSlider *brightnessSlider;
  QLabel *gammaLabel;
  QLabel *gammaValue;
  QSlider *gammaSlider;
  QLabel *scanlineLabel;
  QLabel *scanlineValue;
  QSlider *scanlineSlider;
  QHBoxLayout *options;
  QCheckBox *enableGammaRamp;
  QLabel *cropLabel;
  QGridLayout *cropLayout;
  QLabel *cropLeftLabel;
  QLabel *cropLeftValue;
  QSlider *cropLeftSlider;
  QLabel *cropTopLabel;
  QLabel *cropTopValue;
  QSlider *cropTopSlider;
  QLabel *cropRightLabel;
  QLabel *cropRightValue;
  QSlider *cropRightSlider;
  QLabel *cropBottomLabel;
  QLabel *cropBottomValue;
  QSlider *cropBottomSlider;
  QLabel *pixelShaderLabel;
  QGridLayout *pixelShaderLayout;
  QLineEdit *shaderValue;
  QPushButton *shaderSelect;
  QPushButton *shaderDefault;

  void synchronizePixelShaderSettings();
  void syncUi();
  VideoSettingsWindow();

private slots:
  void autoHideFullscreenMenuToggle();
  void contrastAdjust(int);
  void brightnessAdjust(int);
  void gammaAdjust(int);
  void scanlineAdjust(int);
  void gammaRampToggle(int);
  void cropLeftAdjust(int);
  void cropTopAdjust(int);
  void cropRightAdjust(int);
  void cropBottomAdjust(int);
  void selectShader();
  void defaultShader();

private:
  void assignShader(const string &filename);
};

extern VideoSettingsWindow *videoSettingsWindow;
