class VideoSettingsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      QGridLayout *sliders;
        QLabel *lcontrast;
        QSlider *contrast;
        QLabel *lbrightness;
        QSlider *brightness;
        QLabel *lgamma;
        QSlider *gamma;
      QHBoxLayout *options;
        QCheckBox *enableGammaRamp;
        QCheckBox *enableNtscMergeFields;
      QWidget *spacer;

  void syncUi();
  VideoSettingsWindow();

public slots:
  void gammaRampToggle(int);
  void ntscFieldsToggle(int);
  void contrastAdjust(int);
  void brightnessAdjust(int);
  void gammaAdjust(int);
} *winVideoSettings;
