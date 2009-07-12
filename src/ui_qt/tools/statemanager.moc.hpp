class StateManagerWindow : public QObject {
  Q_OBJECT

public:
  QWidget *panel;
    QVBoxLayout *layout;
      QLabel *title;
      QTreeWidget *list;
      QHBoxLayout *controlLayout;
        QLabel *descLabel;
        QLineEdit *descEdit;
      QHBoxLayout *buttonLayout;
        QPushButton *loadState;
        QPushButton *saveState;
        QPushButton *createState;
        QPushButton *deleteState;

  QMenu *menu;
    QAction *loadStateItem;
    QAction *saveStateItem;
    QAction *createStateItem;
    QAction *deleteStateItem;

  void setup();
  void syncUi();
  void reloadList();

public slots:
  void popupMenu(const QPoint&);
  void listChanged();
  void textEdited();
  void loadSelectedState();
  void saveSelectedState();
  void createNewState();
  void deleteSelectedState();
} *stateManagerWindow;
