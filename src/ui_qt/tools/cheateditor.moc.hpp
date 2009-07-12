class CheatEditorWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      QTreeWidget *list;
      QGridLayout *controlLayout;
        QLabel *descLabel;
        QLineEdit *descEdit;
        QLabel *codeLabel;
        QLineEdit *codeEdit;
      QHBoxLayout *buttonLayout;
        QPushButton *addCode;
        QPushButton *deleteCode;

  QMenu *menu;
    QAction *deleteCodeItem;
    QAction *addCodeItem;

  void setup();
  void syncUi();
  void updateItem(QTreeWidgetItem*);

public slots:
  void popupMenu(const QPoint&);
  void reloadList();
  void listChanged();
  void textEdited();
  void updateCodeStatus();
  void toggleCodeStatus();
  void addNewCode();
  void deleteSelectedCode();
} *cheatEditorWindow;
