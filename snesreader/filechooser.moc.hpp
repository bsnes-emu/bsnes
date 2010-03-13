class FileChooser : public QWidget {
  Q_OBJECT

public:
  lstring list;
  string name;
  string exec();

  FileChooser();

private slots:
  void load();

private:
  QVBoxLayout *layout;
  QListWidget *listWidget;
  QHBoxLayout *controlLayout;
  QPushButton *okButton;
  QPushButton *cancelButton;
} *fileChooser;
