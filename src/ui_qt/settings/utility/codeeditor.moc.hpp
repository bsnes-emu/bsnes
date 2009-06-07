class CodeEditorWindow : public QObject {
  Q_OBJECT

public:
  QWidget *window;
    QVBoxLayout *layout;
      QLabel *descLabel;
      QTextEdit *description;
      QLabel *codeLabel;
      QHBoxLayout *codeLayout;
        QListWidget *codeList;
        QVBoxLayout *controls;
          QLineEdit *codeValue;
          QPushButton *codeAdd;
          QPushButton *codeDelete;
          QPushButton *codeDeleteAll;
          QWidget *spacer;
      QCheckBox *enabled;
      QHBoxLayout *finishControls;
        QPushButton *okButton;
        QPushButton *cancelButton;

  void setup();
  void syncUi();
  void addCode();
  void editCode(unsigned code);
  CodeEditorWindow();

public slots:
  void listChanged();
  void codeChanged();
  void addCodeToList();
  void deleteCodeFromList();
  void deleteAllCodesFromList();
  void accept();
  void dismiss();

private:
  signed activeCode;
  void showWindow(const char *title);

  friend class CheatEditorWindow;
} *winCodeEditor;
