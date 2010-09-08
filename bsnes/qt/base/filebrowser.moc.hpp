class FileBrowser : public FileDialog {
  Q_OBJECT

public:
  function<void (const string&)> onChange;
  function<void (const string&)> onActivate;
  function<void (const string&)> onAccept;

  void chooseFile();
  void chooseFolder();
  enum CartridgeMode { LoadDirect, LoadBase, LoadSlot1, LoadSlot2 } cartridgeMode;
  void loadCartridge(CartridgeMode, signed = -1);

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
  QWidget *previewSpacer;
  QCheckBox *previewApplyPatch;

  string resolveFilename(const string&);
  void onChangeCartridge(const string&);
  void onAcceptCartridge(const string&);

  void acceptNormal(const string &filename);
  void acceptBsx(const string &filename);
  void acceptSufamiTurbo(const string &filename);
  void acceptSuperGameBoy(const string &filename);
};

extern FileBrowser *fileBrowser;
