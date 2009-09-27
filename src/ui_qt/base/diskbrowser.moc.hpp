class PathSettingWidget;

class DiskBrowser : public QbDiskBrowser {
  Q_OBJECT

public:
  PathSettingWidget *activePath;

  void chooseFolder(PathSettingWidget*, const char*);
  void loadAnyCartridge();
  void loadBaseCartridge();
  void loadBsxCartridge();
  void loadSufamiTurboCartridge1();
  void loadSufamiTurboCartridge2();
  void loadSuperGameBoyCartridge();

  string queryAbsoluteImageName(const QString&, bool);
  string queryImageInformation(const char*);

  DiskBrowser();

public slots:
  void onActivate(const QString&, bool);
  void onChange(const QString&, bool);
  void onLoad();

private:
  enum BrowseMode {
    Folder,
    AnyCartridge,
    BaseCartridge,
    BsxCartridge,
    SufamiTurboCartridge1,
    SufamiTurboCartridge2,
    SuperGameBoyCartridge,
  } browseMode;

  QVBoxLayout *layout;
  QLabel *info;
  class ScreenshotPreview : public QWidget {
  public:
    string screenshotName;
    void paintEvent(QPaintEvent*);
  } *screenshot;
  QWidget *spacer;
  QCheckBox *applyPatch;

  string imageName;
} *diskBrowser;
