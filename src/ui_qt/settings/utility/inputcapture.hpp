class InputCaptureWindow : public QObject {
  Q_OBJECT

public:
  InputObject *activeObject;

  struct Window : public QWidget {
    void closeEvent(QCloseEvent*);
  } *window;
    QVBoxLayout *layout;
      QHBoxLayout *hlayout;
        QLabel *title;
        QPushButton *mouseAxes;
          QMenu *mouseAxisMenu;
            QAction *mouseAxisX;
            QAction *mouseAxisY;
        QPushButton *mouseButtons;
          QMenu *mouseButtonMenu;
            QAction *mouseButton[8];
      QWidget *imageSpacer;
      struct ImageWidget : public QWidget {
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
  void assignMouse6();
  void assignMouse7();
} *winInputCapture;
