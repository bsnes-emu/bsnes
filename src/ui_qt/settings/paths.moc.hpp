class PathSettingWidget : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *label;
  QHBoxLayout *controlLayout;
    QLineEdit *path;
    QPushButton *pathSelect;
    QPushButton *pathDefault;

  string &pathValue;
  string pathDefaultLabel;
  string pathBrowseLabel;
  void selectPath(const string&);
  void updatePath();
  PathSettingWidget(string&, const char*, const char*, const char*);

public slots:
  void selectPath();
  void defaultPath();
};

class PathSettingsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      PathSettingWidget *gamePath;
      PathSettingWidget *savePath;
      PathSettingWidget *statePath;
      PathSettingWidget *patchPath;
      PathSettingWidget *cheatPath;
      PathSettingWidget *dataPath;
      QWidget *spacer;

  PathSettingsWindow();
} *winPathSettings;
