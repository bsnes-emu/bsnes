class SettingsWindow : public Window {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QTabWidget *tab;

  SettingsWindow();

public slots:
};

extern SettingsWindow *settingsWindow;
