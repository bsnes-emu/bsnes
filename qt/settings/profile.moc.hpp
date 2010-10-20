class ProfileSettingsWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *profileInfo;
  QRadioButton *profileResearch;
  QLabel *profileResearchInfo;
  QRadioButton *profileBaseline;
  QLabel *profileBaselineInfo;
  QRadioButton *profilePerformance;
  QLabel *profilePerformanceInfo;

  ProfileSettingsWindow();

private slots:
  void setResearchProfile();
  void setBaselineProfile();
  void setPerformanceProfile();
};

extern ProfileSettingsWindow *profileSettingsWindow;
