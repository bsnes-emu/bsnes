class InputSettingsWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QTreeWidget *list;
  QHBoxLayout *controlLayout;
  QLabel *message;
  QPushButton *optionButton;
  QPushButton *assignButton;
  QPushButton *unassignButton;
  QLabel *buttonBox;
  QPushButton *xAxisButton;
  QPushButton *yAxisButton;
  QPushButton *stopButton;

  void inputEvent(uint16_t scancode);
  InputSettingsWindow();

private slots:
  void synchronize();
  void activateAssign();
  void assign();
  void assignGroup();
  void unassign();
  void unassignGroup();
  void xAxisAssign();
  void yAxisAssign();
  void stop();

private:
  QTreeWidgetItem *port1, *port2, *userInterface;
  array<MappedInput*> inputTable;
  MappedInput *activeInput;
  InputGroup *activeGroup;
  unsigned groupIndex;
  unsigned activeMouse;

  void updateList();
  void setAssignment(string);
  void setActiveInput(MappedInput*);
};

extern InputSettingsWindow *inputSettingsWindow;
