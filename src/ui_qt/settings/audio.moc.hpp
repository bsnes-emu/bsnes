class AudioSettingsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      QHBoxLayout *boxes;
        QLabel *frequencyLabel;
        QComboBox *frequency;
        QLabel *latencyLabel;
        QComboBox *latency;
      QGridLayout *sliders;
        QLabel *volumeLabel;
        QSlider *volume;
        QLabel *frequencySkewLabel;
        QSlider *frequencySkew;
      QWidget *spacer;

  void setup();
  void syncUi();

public slots:
  void frequencyChange(int value);
  void latencyChange(int value);
  void volumeAdjust(int value);
  void frequencySkewAdjust(int value);
} *winAudioSettings;
