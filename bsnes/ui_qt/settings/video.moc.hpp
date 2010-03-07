class VideoSettingsWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
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
  QLabel *fragmentShaderLabel;
  QLineEdit *fragmentShaderValue;
  QPushButton *fragmentShaderSelect;
  QPushButton *fragmentShaderDefault;
  QLabel *vertexShaderLabel;
  QLineEdit *vertexShaderValue;
  QPushButton *vertexShaderSelect;
  QPushButton *vertexShaderDefault;

  void synchronizePixelShaderSettings();
  void syncUi();
  VideoSettingsWindow();

private slots:
  void contrastAdjust(int);
  void brightnessAdjust(int);
  void gammaAdjust(int);
  void scanlineAdjust(int);
  void gammaRampToggle(int);
  void cropLeftAdjust(int);
  void cropTopAdjust(int);
  void cropRightAdjust(int);
  void cropBottomAdjust(int);
  void selectFragmentShader();
  void selectVertexShader();
  void defaultFragmentShader();
  void defaultVertexShader();

private:
  void assignFragmentShader(const string &filename);
  void assignVertexShader(const string &filename);
};

extern VideoSettingsWindow *videoSettingsWindow;
