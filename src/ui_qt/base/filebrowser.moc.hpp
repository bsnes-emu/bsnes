class FileBrowser : public FileDialog {
  Q_OBJECT

public:
  function<void (const string&)> onChange;
  function<void (const string&)> onActivate;
  function<void (const string&)> onAccept;

  void chooseFolder();
  void loadCartridge();

  FileBrowser();

private slots:
  void change(const string&);
  void activate(const string&);
  void accept(const string&);
  void toggleApplyPatch();

private:
  QVBoxLayout *previewLayout;
  QLabel *previewInfo;
  QWidget *previewImage;
  QCheckBox *previewApplyPatch;

  string resolveFilename(const string&);
  void onChangeCartridge(const string&);
  void onAcceptCartridge(const string&);
};

extern FileBrowser *fileBrowser;
