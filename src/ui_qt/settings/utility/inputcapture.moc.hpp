class InputCaptureWindow : public QObject {
  Q_OBJECT

public:
  class Window : public QbWindow {
  public:
    void closeEvent(QCloseEvent*);
    Window(string&);
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
} *inputCaptureWindow;

class InputMouseCaptureWindow : public QObject {
  Q_OBJECT

public:
  enum Mode { AxisMode, ButtonMode };

  QbWindow *window;
  QVBoxLayout *layout;
    QLabel *info;
    QLabel *captureBox;
    QHBoxLayout *buttonLayout;
      QPushButton *xAxis;
      QPushButton *yAxis;
    QWidget *spacer;

  void activate(Mode);
  void inputEvent(uint16_t code);
  InputMouseCaptureWindow();

public slots:
  void assignAxisX();
  void assignAxisY();

private:
  Mode activeMode;
  signed activeMouse;
} *inputMouseCaptureWindow;

class InputCalibrationWindow : public QObject {
  Q_OBJECT

public:
  class Window : public QbWindow {
  public:
    void closeEvent(QCloseEvent*);
    Window(string&);
  } *window;
  QVBoxLayout *layout;
    QLabel *info;
    QPushButton *ok;
    QWidget *spacer;

  void activate(unsigned joy);
  InputCalibrationWindow();

public slots:
  void dismiss();

private:
  int activeJoypad;
} *inputCalibrationWindow;
