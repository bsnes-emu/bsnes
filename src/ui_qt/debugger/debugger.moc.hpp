class Debugger : public QbWindow {
  Q_OBJECT

public:
  QMenuBar *menu;
  QMenu *tools;
  QAction *tools_disassembler;
  QAction *tools_breakpoint;
  QAction *tools_memory;
  QAction *tools_vramViewer;
  QMenu *miscOptions;
  QAction *miscOptions_clear;

  QHBoxLayout *layout;
  QTextEdit *console;
  QVBoxLayout *controlLayout;
  QHBoxLayout *commandLayout;
  QPushButton *runBreak;
  QPushButton *stepInstruction;
  QCheckBox *stepCPU;
  QCheckBox *stepSMP;
  QCheckBox *traceCPU;
  QCheckBox *traceSMP;
  QCheckBox *traceMask;
  QWidget *spacer;

  void modifySystemState(unsigned);
  void echo(const char *message);
  void event();
  void frameTick();
  Debugger();

public slots:
  void showDisassembler();
  void showBreakpointEditor();
  void showMemoryEditor();
  void showVramViewer();
  void clear();
  void synchronize();

  void toggleRunStatus();
  void stepAction();

private:
  unsigned frameCounter;
};

extern Debugger *debugger;
