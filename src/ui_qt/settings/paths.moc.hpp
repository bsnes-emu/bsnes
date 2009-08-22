class PathSettingsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      QLabel *gameLabel;
      QHBoxLayout *games;
        QLineEdit *gamePath;
        QPushButton *gameSelect;
        QPushButton *gameDefault;
      QLabel *saveLabel;
      QHBoxLayout *saves;
        QLineEdit *savePath;
        QPushButton *saveSelect;
        QPushButton *saveDefault;
      QLabel *stateLabel;
      QHBoxLayout *states;
        QLineEdit *statePath;
        QPushButton *stateSelect;
        QPushButton *stateDefault;
      QLabel *patchLabel;
      QHBoxLayout *patches;
        QLineEdit *patchPath;
        QPushButton *patchSelect;
        QPushButton *patchDefault;
      QLabel *cheatLabel;
      QHBoxLayout *cheats;
        QLineEdit *cheatPath;
        QPushButton *cheatSelect;
        QPushButton *cheatDefault;
      QLabel *dataLabel;
      QHBoxLayout *datum;
        QLineEdit *dataPath;
        QPushButton *dataSelect;
        QPushButton *dataDefault;
      QWidget *spacer;

  void setup();
  void syncUi();
  void syncPath(QLineEdit *control, const string &path, const char *caption);

public slots:
  void selectGamePath();
  void defaultGamePath();
  void selectSavePath();
  void defaultSavePath();
  void selectStatePath();
  void defaultStatePath();
  void selectPatchPath();
  void defaultPatchPath();
  void selectCheatPath();
  void defaultCheatPath();
  void selectDataPath();
  void defaultDataPath();
} *winPathSettings;
