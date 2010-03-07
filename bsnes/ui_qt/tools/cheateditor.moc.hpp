class CheatEditorWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QTreeWidget *list;
  QGridLayout *gridLayout;
  QLabel *codeLabel;
  QLineEdit *codeEdit;
  QLabel *descLabel;
  QLineEdit *descEdit;
  QHBoxLayout *controlLayout;
  QCheckBox *cheatEnableBox;
  QWidget *spacer;
  QPushButton *findButton;
  QPushButton *clearButton;

  void load(const char *filename);
  void save(const char *filename);
  void update();

  void synchronize();
  CheatEditorWindow();

private slots:
  void bind();
  void listChanged();
  void codeEdited();
  void descEdited();
  void toggleCheatEnable();
  void findCheatCodes();
  void clearSelected();

private:
  bool lock;
  xml_element *document;
  friend class CheatImportWindow;
};

class CheatImportWindow : public Window {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *title;
  QTreeWidget *list;
  QHBoxLayout *controlLayout;
  QPushButton *selectAllButton;
  QPushButton *clearAllButton;
  QWidget *spacer;
  QPushButton *okButton;
  QPushButton *cancelButton;

  void refresh(xml_element*);

  CheatImportWindow();

private slots:
  void selectAllCodes();
  void clearAllCodes();
  void addSelectedCodes();
};

extern CheatEditorWindow *cheatEditorWindow;
extern CheatImportWindow *cheatImportWindow;
