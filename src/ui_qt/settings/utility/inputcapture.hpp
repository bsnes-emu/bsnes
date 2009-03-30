class InputCaptureWindow : public QObject {
  Q_OBJECT

public:
  struct Window : public QWidget {
    void closeEvent(QCloseEvent*);
  } *window;
    QVBoxLayout *layout;
      QHBoxLayout *hlayout;
        QLabel *title;
        QPushButton *mouseAxes;
        QPushButton *mouseButtons;
      QWidget *imageSpacer;
      struct ImageWidget : public QWidget {
        void paintEvent(QPaintEvent*);
      } *imageWidget;
      QWidget *spacer;

  void setup();
  void activate(InputObject *object);
  void activate(InputGroup *group);
  void inputEvent(uint16_t code, bool forceAssign = false);
  InputCaptureWindow();

public slots:
  void assignMouseAxis();
  void assignMouseButton();

private:
  InputObject *activeObject;
  InputGroup *activeGroup;
  unsigned groupIndex;
  bool inputLock;

  friend class InputCaptureWindow::Window;
} *winInputCapture;

class InputMouseCaptureWindow : public QObject {
  Q_OBJECT

public:
  enum Mode { AxisMode, ButtonMode };

  QWidget *window;
    QVBoxLayout *layout;
      QLabel *info;
      QLabel *captureBox;
      QHBoxLayout *buttonLayout;
        QPushButton *xAxis;
        QPushButton *yAxis;
      QWidget *spacer;

  void setup();
  void activate(Mode);
  void inputEvent(uint16_t code);

public slots:
  void assignAxisX();
  void assignAxisY();

private:
  Mode activeMode;
  signed activeMouse;
} *winInputMouseCaptureWindow;

class InputCalibrationWindow : public QObject {
  Q_OBJECT

public:
  struct Window : public QWidget {
    void closeEvent(QCloseEvent*);
  } *window;
    QVBoxLayout *layout;
      QLabel *info;
      QPushButton *ok;
      QWidget *spacer;

  void setup();
  void activate(unsigned joy);

public slots:
  void dismiss();

private:
  int activeJoypad;

  friend class InputCalibrationWindow::Window;
} *winInputCalibration;
