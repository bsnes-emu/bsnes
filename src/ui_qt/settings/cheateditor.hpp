class CheatEditorWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
    QLabel *title;
    QTreeWidget *list;
    QHBoxLayout *controls;
      QPushButton *addCode;
      QPushButton *editCode;
      QPushButton *deleteCode;

  void setup();
  void syncUi();
  void reloadList();
  void updateList();

public slots:
  void itemChanged(QTreeWidgetItem *item);
  void listChanged();
  void addNewCode();
  void editSelectedCode();
  void deleteSelectedCode();

private:
  array<QTreeWidgetItem*> listItem;
} *winCheatEditor;
