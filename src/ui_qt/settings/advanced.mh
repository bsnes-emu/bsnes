class AdvancedSettingsWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      QGridLayout *driverLayout;
        QLabel *videoLabel;
        QLabel *audioLabel;
        QLabel *inputLabel;
        QComboBox *videoDriver;
        QComboBox *audioDriver;
        QComboBox *inputDriver;
        QLabel *driverInfo;

      QLabel *regionTitle;
      QHBoxLayout *regionLayout;
        QButtonGroup *regionGroup;
        QRadioButton *regionAuto;
        QRadioButton *regionNTSC;
        QRadioButton *regionPAL;

      QLabel *portTitle;
      QHBoxLayout *portLayout;
        QButtonGroup *portGroup;
        QRadioButton *portSatellaview;
        QRadioButton *portNone;
        QWidget *portSpacer;

      QLabel *focusTitle;
      QHBoxLayout *focusLayout;
        QButtonGroup *focusButtonGroup;
        QRadioButton *focusPause;
        QRadioButton *focusIgnore;
        QRadioButton *focusAllow;
      QWidget *spacer;

  void setup();
  void initializeUi();

public slots:
  void videoDriverChange(int index);
  void audioDriverChange(int index);
  void inputDriverChange(int index);
  void setRegionAuto();
  void setRegionNTSC();
  void setRegionPAL();
  void setPortSatellaview();
  void setPortNone();
  void pauseWithoutFocus();
  void ignoreInputWithoutFocus();
  void allowInputWithoutFocus();
} *winAdvancedSettings;
