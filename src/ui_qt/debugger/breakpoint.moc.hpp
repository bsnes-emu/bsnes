class BreakpointItem : public QWidget {
  Q_OBJECT

public:
  QHBoxLayout *layout;
  QCheckBox *enabled;
  QLineEdit *addr;
  QLineEdit *data;
  QComboBox *mode;
  QComboBox *source;
  BreakpointItem(unsigned id);

public slots:
  void toggle();

private:
  const unsigned id;
};

class BreakpointEditor : public QObject {
  Q_OBJECT

public:
  QWidget *window;
  QVBoxLayout *layout;
  BreakpointItem *breakpoint[SNES::Debugger::Breakpoints];

  void setup();

public slots:
} *breakpointEditor;
