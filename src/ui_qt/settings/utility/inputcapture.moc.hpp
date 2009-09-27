class InputCaptureWindow : public QbWindow {
  Q_OBJECT

public:
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

  void closeEvent(QCloseEvent*);

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
} *winInputCapture;

class InputMouseCaptureWindow : public QbWindow {
  Q_OBJECT

public:
  enum Mode { AxisMode, ButtonMode };

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
} *winInputMouseCapture;

class InputCalibrationWindow : public QbWindow {
  Q_OBJECT

public:
  QVBoxLayout *layout;
    QLabel *info;
    QPushButton *ok;
    QWidget *spacer;

  void activate(unsigned joy);
  InputCalibrationWindow();

  void closeEvent(QCloseEvent*);

public slots:
  void dismiss();

private:
  int activeJoypad;
} *winInputCalibration;
