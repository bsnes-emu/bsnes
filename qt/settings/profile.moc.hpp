class ProfileSettingsWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *profileInfo;
  QRadioButton *profileAccuracy;
  QLabel *profileAccuracyInfo;
  QRadioButton *profileCompatibility;
  QLabel *profileCompatibilityInfo;
  QRadioButton *profilePerformance;
  QLabel *profilePerformanceInfo;

  ProfileSettingsWindow();

private slots:
  void setAccuracyProfile();
  void setCompatibilityProfile();
  void setPerformanceProfile();
};

extern ProfileSettingsWindow *profileSettingsWindow;
