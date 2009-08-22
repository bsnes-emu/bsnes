#include "breakpoint.moc"
#include "memory.moc"

class Debugger : public QObject {
  Q_OBJECT

public:
  QWidget *window;
  QMenuBar *menu;
    QMenu *tools;
      QAction *tools_breakpoint;
      QAction *tools_memory;
    QMenu *options;
  QVBoxLayout *layout;
    QTextEdit *console;
    QLineEdit *command;

  void setup();
  void syncUi();
  void show();
  void clear();
  void echo(const char *message);
  void event();

public slots:
  void execCommand();
  void showBreakpointEditor();
  void showMemoryEditor();
} *debugger;
