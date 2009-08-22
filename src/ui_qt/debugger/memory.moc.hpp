class MemoryEditor : public QObject {
  Q_OBJECT

public:
  QWidget *window;
  QVBoxLayout *layout;
    QHBoxLayout *controls;
      QWidget *spacer;
      QLineEdit *addr;
      QComboBox *source;
      QPushButton *refresh;
      QCheckBox *autoUpdate;
    QTextEdit *editor;

  void setup();
  void syncUi();

public slots:
  void refreshView();
} *memoryEditor;
