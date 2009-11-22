class InputSettingsWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *title;
  QHBoxLayout *comboLayout;
  QComboBox *port;
  QComboBox *device;

  QWidget *selectionWidget;
  QVBoxLayout *selectionLayout;
  QTreeWidget *list;
  QHBoxLayout *selectionControlLayout;
  QPushButton *assignButton;
  QPushButton *assignAllButton;
  QPushButton *unassignButton;
  QPushButton *unassignAllButton;

  QWidget *assignmentWidget;
  QVBoxLayout *assignmentLayout;
  QLabel *assignmentLabel;
  QHBoxLayout *assignmentControlLayout;
  QPushButton *xaxisButton;
  QPushButton *yaxisButton;
  QWidget *spacer;
  QPushButton *helpButton;
  QPushButton *cancelButton;

  void inputEvent(uint16_t scancode);
  void syncUi();
  InputSettingsWindow();

private slots:
  void portChanged();
  void reloadList();
  void listChanged();
  void assign();
  void assignAll();
  void unassign();
  void unassignAll();
  void assignXaxis();
  void assignYaxis();
  void showHelp();
  void cancelAssignment();

private:
  InputGroup *activeGroup;
  MappedInput *activeObject;
  unsigned activeMouse;
  bool multiAssign;
  unsigned multiAssignIndex;

  void setAssignment(string name);
  void digitalInputEvent(uint16_t scancode);
  void analogInputEvent(uint16_t scancode);

  void beginAssignment();
  void assignObject(MappedInput *object);
  void endAssignment();
} *inputSettingsWindow;
