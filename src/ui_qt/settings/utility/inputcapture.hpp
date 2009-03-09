class InputCaptureWindow : public QObject {
  Q_OBJECT

public:
  InputObject *activeObject;

  struct Window : public QWidget {
    void closeEvent(QCloseEvent*);
  } *window;
    QVBoxLayout *layout;
      QLabel *title;
      QGroupBox *axisGroup;
        QHBoxLayout *axisLayout;
          QPushButton *mouseAxisX;
          QPushButton *mouseAxisY;
      QGroupBox *buttonGroup;
        QGridLayout *buttonLayout;
          QPushButton *mouseButton[6];
      QWidget *imageSpacer;
      struct InputImage : public QWidget {
        void paintEvent(QPaintEvent*);
      } *imageWidget;
      QWidget *spacer;

  void setup();
  void activate(InputObject *object);
  void inputEvent(uint16_t code, bool forceAssign = false);
  InputCaptureWindow();

public slots:
  void assignMouseX();
  void assignMouseY();
  void assignMouse0();
  void assignMouse1();
  void assignMouse2();
  void assignMouse3();
  void assignMouse4();
  void assignMouse5();
} *winInputCapture;
